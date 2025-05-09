/**
 * \file
 * \brief
 * \author Dan Rakušan <rakusan.dan@gmail.com>
 * \copyright Copyright (C) 2016-2023 I3T team, Department of Computer Graphics
 * and Interaction, FEE, Czech Technical University in Prague, Czech Republic
 *
 * This file is part of I3T - An Interactive Tool for Teaching Transformations
 * http://www.i3t-tool.org
 *
 * GNU General Public License v3.0 (see LICENSE.txt or https://www.gnu.org/licenses/gpl-3.0.txt)
 */
#include "ResourceManager.h"

#include "magic_enum.hpp"

#include "Core/Resources/Mesh.h"
#include "Logger/Logger.h"
#include "ModelResourceFiles.h"
#include "State/StateManager.h"
#include "Utils/Format.h"
#include "Utils/GLUtils.h"
#include "Utils/Text.h"

// Used for method overloading to indicate no alias was specified
#define NO_ALIAS "no_alias_QEYjMrxoOB_rm"
// Indicates that no hashid was specified
#define NO_HASHID 0

namespace Core
{

ResourceManager::~ResourceManager()
{
	dispose();
}

ResourceManager& ResourceManager::instance()
{
	return App::getModule<ResourceManager>();
}

bool ResourceManager::ResFilesCmp::operator()(Ptr<Resource> lhs, Ptr<Resource> rhs) const
{
	return *(lhs->resourceFiles) < *(rhs->resourceFiles);
}

bool ResourceManager::resourceExists(size_t id)
{
	auto it = m_resourceMap.find(id);
	return it != m_resourceMap.end();
}

bool ResourceManager::resourceExists(std::string& alias)
{
	auto it = m_aliasMap.find(alias);
	return it != m_aliasMap.end();
}

GLuint ResourceManager::textureByAlias(const std::string& alias)
{
	bool success = false;
	auto data = getData(alias, NO_HASHID, ResourceType::Texture, &success);
	if (success)
	{
		if (data.get() != nullptr)
		{
			return *(std::static_pointer_cast<GLuint>(data).get());
		}
	}
	return 0;
}

GLuint ResourceManager::texture(const std::string& path)
{
	return texture(NO_ALIAS, path);
}

GLuint ResourceManager::texture(const std::string& alias, const std::string& path)
{
	size_t id = hash_string(path);
	bool success = false;
	auto data = getData(alias, id, ResourceType::Texture, &success);
	if (success)
	{
		if (data.get() == nullptr)
		{
			// Load texture
			GLuint textureId = loadTexture(path);
			if (textureId)
			{
				std::shared_ptr<Resource> textureResource =
				    std::make_shared<Resource>(alias, path, ResourceType::Texture);
				textureResource->hashId = id;
				textureResource->data = std::make_shared<GLuint>(textureId);
				m_resourceMap.insert(std::make_pair(id, textureResource));
				registerAlias(alias, textureResource);
			}
			return textureId;
		}
		else
		{
			// Return existing texture
			return *(std::static_pointer_cast<GLuint>(data).get());
		}
	}
	return 0;
}

GLuint ResourceManager::shaderByAlias(const std::string& alias)
{
	bool success = false;
	auto data = getData(alias, NO_HASHID, ResourceType::Shader, &success);
	if (success)
	{
		if (data.get() != nullptr)
		{
			return *(std::static_pointer_cast<GLuint>(data).get());
		}
	}
	return 0;
}

GLuint ResourceManager::shader(const std::string& vertShader, const std::string& fragShader)
{
	return shader(NO_ALIAS, vertShader, fragShader);
}

GLuint ResourceManager::shader(const std::string& alias, const std::string& vertShader, const std::string& fragShader)
{
	return shaderG(alias, vertShader, fragShader, "");
}

GLuint ResourceManager::shaderG(const std::string& vertShader, const std::string& fragShader,
                                const std::string& geoShader)
{
	return shaderG(NO_ALIAS, vertShader, fragShader, geoShader);
}

GLuint ResourceManager::shaderG(const std::string& alias, const std::string& vertShader, const std::string& fragShader,
                                const std::string& geoShader)
{
	return shaderGI(alias, vertShader, fragShader, geoShader, "");
}

GLuint ResourceManager::shaderGI(const std::string& vertShader, const std::string& fragShader,
                                 const std::string& geoShader, const std::string& sourceToInject)
{
	return shaderGI(NO_ALIAS, vertShader, fragShader, geoShader, sourceToInject);
}

GLuint ResourceManager::shaderGI(const std::string& alias, const std::string& vertShader, const std::string& fragShader,
                                 const std::string& geoShader, const std::string& sourceToInject)
{
	size_t id = hash_string(vertShader, fragShader, geoShader, std::to_string(sourceToInject.size()));
	bool success = false;
	auto data = getData(alias, id, ResourceType::Shader, &success);
	if (success)
	{
		if (data.get() == nullptr)
		{
			// Load shader
			GLuint shaderId = loadShader(vertShader, fragShader, geoShader, sourceToInject);
			if (shaderId)
			{
				std::string path = "Vert: " + vertShader + ", Frag: " + fragShader +
				                   (geoShader.empty() ? "" : (", Geo: " + geoShader));
				std::shared_ptr<Resource> shaderResource =
				    std::make_shared<Resource>(alias, path, ResourceType::Shader);
				shaderResource->hashId = id;
				shaderResource->data = std::make_shared<GLuint>(shaderId);
				m_resourceMap.insert(std::make_pair(id, shaderResource));
				registerAlias(alias, shaderResource);
			}
			return shaderId;
		}
		else
		{
			// Return existing shader
			return *(std::static_pointer_cast<GLuint>(data).get());
		}
	}
	return 0;
}

Mesh* ResourceManager::meshByAlias(const std::string& alias)
{
	bool success = false;
	auto data = getData(alias, NO_HASHID, ResourceType::Model, &success);
	if (success)
	{
		if (data.get() != nullptr)
		{
			return std::static_pointer_cast<Mesh>(data).get();
		}
	}
	return nullptr;
}
Mesh* ResourceManager::mesh(const std::string& path)
{
	return mesh(NO_ALIAS, path);
}
Mesh* ResourceManager::mesh(const std::string& alias, const std::string& path)
{
	size_t id = hash_string(path);
	bool success = false;
	auto data = getData(alias, id, ResourceType::Model, &success);
	if (success)
	{
		if (data.get() == nullptr)
		{
			// Load model
			Mesh* mesh = loadModel(path);
			if (mesh)
			{
				mesh->m_resourceId = id;
				mesh->m_path = path;
				std::shared_ptr<Resource> modelResource = std::make_shared<Resource>(alias, path, ResourceType::Model);
				modelResource->hashId = id;
				modelResource->data = std::shared_ptr<Mesh>(mesh);
				m_resourceMap.insert(std::make_pair(id, modelResource));
				registerAlias(alias, modelResource);
			}
			return mesh;
		}
		else
		{
			// Return existing model
			return std::static_pointer_cast<Mesh>(data).get();
		}
	}
	return nullptr;
}

Mesh* ResourceManager::mesh(const std::string& alias, Mesh::PrimitiveType primitiveType, const float* verts,
                            const unsigned int nVertices, const float* colors, const unsigned int nColors)
{
	return mesh(alias, primitiveType, verts, nVertices, nullptr, 0, colors, nColors);
}

Mesh* ResourceManager::mesh(const std::string& alias, Mesh::PrimitiveType primitiveType, const float* verts,
                            const unsigned int nVertices, const unsigned int* indices, const unsigned int nIndices,
                            const float* colors, const unsigned int nColors)
{
	bool useIndices = indices != nullptr && nIndices != 0;

	// TODO: (DR) Compute hash of data arrays as well but only when necessary
	//	Currently meshes from direct data are thus NOT CACHED AT ALL, perhaps I
	// didn't think this through enough

	size_t seed = 0;
	hash_combine(seed, static_cast<int>(primitiveType));
	hash_combine(seed, nVertices);
	if (useIndices)
		hash_combine(seed, nIndices);
	hash_combine(seed, nColors);
	size_t id = seed;

	bool success = false;
	auto data = getData(alias, id, ResourceType::Model, &success);
	if (success)
	{
		if (data.get() == nullptr)
		{
			// Load model
			std::string dataSummary = " type: " + std::to_string(static_cast<int>(primitiveType)) +
			                          " vertices: " + std::to_string(nVertices) +
			                          (useIndices ? (" indices: " + std::to_string(nIndices)) : "") +
			                          " colors: " + std::to_string(nColors);
			LOG_INFO("[MODEL] Loading model '{}' from data:{}", alias, dataSummary);

			Mesh* mesh = nullptr;
			if (useIndices)
			{
				mesh = Mesh::create(primitiveType, verts, nVertices, indices, nIndices, colors, nColors, Mesh::STATIC);
			}
			else
			{
				mesh = Mesh::create(primitiveType, verts, nVertices, colors, nColors, Mesh::STATIC);
			}

			if (mesh)
			{
				mesh->m_resourceId = id;
				std::string path = std::string("Generated from data") + dataSummary;
				std::shared_ptr<Resource> modelResource = std::make_shared<Resource>(alias, path, ResourceType::Model);
				modelResource->hashId = id;
				modelResource->data = std::shared_ptr<Mesh>(mesh);
				m_resourceMap.insert(std::make_pair(id, modelResource));
				registerAlias(alias, modelResource);
			}
			return mesh;
		}
		else
		{
			// Return existing model
			return std::static_pointer_cast<Mesh>(data).get();
		}
	}
	return nullptr;
}

void ResourceManager::registerDefault(const std::string& alias)
{
	auto aliasIt = m_aliasMap.find(alias);
	if (aliasIt != m_aliasMap.end())
	{
		m_defaultResources.insert(alias);
	}
	else
	{
		LOG_ERROR("[RESOURCE MANAGER] Cannot register default, provided alias "
		          "'{}' doesn't exist!",
		          alias);
	}
}

std::vector<Resource> ResourceManager::getDefaultResources(ResourceType type)
{
	std::vector<Resource> resources;
	for (auto& alias : m_defaultResources)
	{
		auto aliasIt = m_aliasMap.find(alias);
		if (aliasIt != m_aliasMap.end())
		{
			auto resource = aliasIt->second.lock();
			if (resource->resourceType == type)
			{
				resources.push_back(*resource);
			}
		}
		else
		{
			LOG_ERROR("[RESOURCE MANAGER] Default resource '{}' doesn't exist!", alias);
		}
	}
	return resources;
}

std::vector<std::string> ResourceManager::getImportedResourceAliases()
{
	return m_importedResources;
}

std::shared_ptr<void> ResourceManager::getData(const std::string& alias, const size_t id, ResourceType type,
                                               bool* success)
{
	if (m_forceReload)
	{
		LOG_INFO("[RESOURCE MANAGER] Force reloading resource - HashID: {} Type: {}", id, n(type));
		*success = true;
		return nullptr;
	}

	if (alias.empty())
	{
		LOG_ERROR("[RESOURCE MANAGER] Cannot retrieve resource under an empty alias! HashID: {} Type: {}", id, n(type));
		*success = false;
		return nullptr;
	}

	if (alias != NO_ALIAS)
	{
		// Resolving alias
		// Check if alias of this type exists
		auto aliasIt = m_aliasMap.find(alias);
		if (aliasIt != m_aliasMap.end())
		{
			// Alias exists
			auto resourcePtr = aliasIt->second.lock();
			if (resourcePtr->resourceType != type)
			{
				LOG_ERROR("[RESOURCE MANAGER] Resource of a different type is already "
				          "aliased under '{}'! (Type {} x {})",
				          alias, n(type), n(resourcePtr->resourceType));
				*success = false;
				return nullptr;
			}
			if (id != NO_HASHID && resourcePtr->hashId != id)
			{
				LOG_ERROR("[RESOURCE MANAGER] Existing resource has a different hash "
				          "under the same alias '{}'! (HashID {} x {})",
				          alias, id, resourcePtr->hashId);
				*success = false;
				return nullptr;
			}
			*success = true;
			return resourcePtr->data;
		}
		else
		{
			// Alias doesn't exist
			// -> try find by id
			auto resourceIt = m_resourceMap.find(id);
			if (resourceIt != m_resourceMap.end())
			{
				// Exists by id but not alias
				// -> Assign alias and return by id
				registerAlias(alias, resourceIt->second);
				*success = true;
				return resourceIt->second->data;
			}
			else
			{
				// No alias No Resource
				// To be created
				*success = true;
				return nullptr;
			}
		}
	}
	else
	{
		// Don't care about alias
		// Find resource by id, return it if exists, create if not
		auto resourceIt = m_resourceMap.find(id);
		if (resourceIt != m_resourceMap.end())
		{
			// Exists by id
			// -> Return by id
			registerAlias(alias, resourceIt->second);
			*success = true;
			return resourceIt->second->data;
		}
		else
		{
			// No Resource
			*success = true;
			return nullptr;
		}
	}
}

void ResourceManager::registerAlias(const std::string& alias, std::shared_ptr<Resource> resource)
{
	if (!alias.empty())
	{
		if (alias != NO_ALIAS)
		{
			if (resource->alias != NO_ALIAS && resource->alias != alias)
			{
				// Resource already has a different alias
				// We register this new one anyway as an alternative
				resource->alternativeAliases.push_back(alias);
				m_aliasMap.insert(std::make_pair(alias, resource));
				LOG_DEBUG("[RESOURCE MANAGER] Registering alternative alias '{}' for '{}'.", alias, resource->alias);
				//				LOG_ERROR("[RESOURCE MANAGER] Failed to register alias '{}'! Resource "
				//				          "already exists under alias '{}'.",
				//				          alias, resource->alias);
			}
			else
			{
				resource->alias = alias;
				m_aliasMap.insert(std::make_pair(alias, resource));
			}
		}
	}
	else
	{
		LOG_ERROR("[RESOURCE MANAGER] Cannot register an empty alias! HashID: {}", resource->hashId);
	}
}

Ptr<Resource> ResourceManager::resourceByAlias(const std::string& alias)
{
	auto aliasIt = m_aliasMap.find(alias);
	if (aliasIt != m_aliasMap.end())
	{
		Ptr<Resource> ptr = aliasIt->second.lock();
		return ptr;
	}
	return Ptr<Resource>();
}


void ResourceManager::loadDefaultResources(rapidjson::Document& doc)
{
	LOG_INFO("[RESOURCE MANAGER] Loading default resources");

	std::vector<ModelSaveEntry> modelEntries = deserializeModels(doc["defaultResources"]);

	for (const auto& modelEntry : modelEntries)
	{
		switch (modelEntry.type)
		{
		case ResourceType::Model:
			if (modelEntry.normalize)
				m_forceModelNormalize = true;
			if (mesh(modelEntry.name, modelEntry.path))
			{
				registerDefault(modelEntry.name);
			}
			m_forceModelNormalize = false;
			break;
		default:
			// Only model resources can be loaded currently
			break;
		}
	}
}

GLuint ResourceManager::loadTexture(const std::string& path)
{
	std::string absPath = path;
	LOG_INFO("[TEXTURE] Loading texture file: {}", absPath);
	GLuint id = GLUtils::createTexture(absPath, true, m_forceClampToEdge);
	if (id == 0)
	{
		LOG_ERROR("[TEXTURE] Failed to load texture!");
	}
	return id;
}

GLuint ResourceManager::loadShader(const std::string& vertShader, const std::string& fragShader,
                                   const std::string& geoShader, const std::string& sourceToInject)
{
	GLuint id = -1;
	std::vector<GLuint> shaderList;

	bool vert = !vertShader.empty();
	bool frag = !fragShader.empty();
	bool geo = !geoShader.empty();

	if (!vert)
	{
		LOG_ERROR("[SHADER] Cannot load shader, no vertex shader specified!");
		return 0;
	}

	LOG_INFO("[SHADER] Loading shader: vert: {}, frag: {}, geo: {}, injected code: {}", vert ? vertShader : "none",
	         frag ? fragShader : "none", geo ? geoShader : "none", sourceToInject.empty() ? "none" : sourceToInject);

	shaderList.push_back(GLUtils::createShaderFromFile(GL_VERTEX_SHADER, vertShader, sourceToInject));
	if (frag)
		shaderList.push_back(GLUtils::createShaderFromFile(GL_FRAGMENT_SHADER, fragShader, sourceToInject));
	if (geo)
		shaderList.push_back(GLUtils::createShaderFromFile(GL_GEOMETRY_SHADER, geoShader, sourceToInject));

	// Check for compilation error
	for (const auto& stage : shaderList)
	{
		if (stage == 0)
		{
			id = 0;
			break;
		}
	}

	// Link if compilation was ok
	if (id != 0)
	{
		id = GLUtils::createProgram(shaderList);
	}

	if (id == 0)
	{
		LOG_ERROR("[SHADER] Failed to load shader!");
	}
	return id;
}

Mesh* ResourceManager::loadModel(const std::string& path)
{
	LOG_INFO("[MODEL] Loading model from file: {}", path);
	Mesh* mesh = Mesh::load(path, m_forceModelNormalize, m_forceMinimumLoad);
	if (mesh == nullptr)
	{
		LOG_ERROR("[MODEL] Failed to load model at '{}'!", path);
	}
	return mesh;
}

void ResourceManager::disposeTexture(GLuint id)
{
	glDeleteTextures(1, &id);
}

void ResourceManager::disposeShader(GLuint id)
{
	pgr::deleteProgramAndShaders(id);
}

void ResourceManager::disposeModel(Mesh* mesh)
{
	mesh->dispose();
}

void ResourceManager::dispose()
{
	for (auto& entry : m_resourceMap)
	{
		disposeResource(entry.second);
	}
}

void ResourceManager::disposeResource(std::shared_ptr<Resource>& resource)
{
	switch (resource->resourceType)
	{
	case ResourceType::Texture:
		LOG_INFO("[TEXTURE] Disposing texture '{}'", resource->path);
		disposeTexture(*(std::static_pointer_cast<GLuint>(resource->data).get()));
		break;
	case ResourceType::Shader:
		LOG_INFO("[SHADER] Disposing shader '{}'", resource->path);
		disposeShader(*(std::static_pointer_cast<GLuint>(resource->data).get()));
		break;
	case ResourceType::Model:
		LOG_INFO("[MODEL] Disposing model '{}'", resource->path);
		disposeModel(std::static_pointer_cast<Mesh>(resource->data).get());
		break;
	default:
		LOG_WARN("[RESOURCE MANAGER] Unknown resource type to dispose! Id: {}, Type: {}, Path: {}", resource->hashId,
		         std::string(magic_enum::enum_name(resource->resourceType)), resource->path);
		break;
	}
}

bool ResourceManager::removeResource(ManagedResource* managedResource, bool force)
{
	if (!managedResource)
	{
		LOG_WARN("[RESOURCE MANAGER] Cannot remove null resource!");
		return false;
	}

	size_t id = managedResource->m_resourceId;
	auto resourceIt = m_resourceMap.find(id);
	if (resourceIt != m_resourceMap.end())
	{
		std::shared_ptr<Resource> resource = resourceIt->second;
		return removeResource(resource, force);
	}
	else
	{
		LOG_WARN("[RESOURCE MANAGER] Cannot dispose resource! Resource with id {} does not exist.", id);
		return false;
	}
}

bool ResourceManager::removeResource(std::shared_ptr<Resource>& resource, bool force)
{
	// If the remove isn't forceful ensure the resource to be removed isn't one of the defaults
	if (!force)
	{
		bool isDefaultResource = false;
		if (m_defaultResources.contains(resource->alias))
		{
			isDefaultResource = true;
		}
		else
		{
			for (const auto& altAlias : resource->alternativeAliases)
			{
				if (m_defaultResources.contains(altAlias))
				{
					isDefaultResource = true;
					break;
				}
			}
		}
		if (isDefaultResource)
		{
			LOG_WARN("[RESOURCE MANAGER] Attempted to remove default resource id '{}'!", resource->hashId);
			return false;
		}
	}

	// Remove alias and alternative aliases, remove default aliases too
	m_aliasMap.erase(resource->alias);
	m_defaultResources.erase(resource->alias);
	m_importedResources.erase(std::remove(m_importedResources.begin(), m_importedResources.end(), resource->alias),
	                          m_importedResources.end());
	std::string alternativeAliases;
	for (const auto& alias : resource->alternativeAliases)
	{
		alternativeAliases += "|" + alias;
		m_aliasMap.erase(alias);
		m_defaultResources.erase(alias);
		m_importedResources.erase(std::remove(m_importedResources.begin(), m_importedResources.end(), alias),
		                          m_importedResources.end());
	}

	disposeResource(resource);

	if (m_resourceMap.erase(resource->hashId) != 1)
	{
		LOG_ERROR("[RESOURCE MANAGER] Failed to erase resource from the resource map, id: '{}'", resource->hashId);
	}
	LOG_INFO("[RESOURCE MANAGER] Deleted resource id: {}, alias: {}{}.", resource->hashId, resource->alias,
	         alternativeAliases);
	resource.reset(); // Ensure there is no dangling pointer
	return true;
}

bool ResourceManager::importModel(const fs::path& path, bool normalize)
{
	StateManager& stateManager = App::getModule<StateManager>();

	LOG_INFO("[IMPORT] Importing model from path '{}'...", path.string());
	std::string modelAlias = path.stem().string();
	if (resourceExists(modelAlias))
	{
		LOG_INFO("[IMPORT] A resource with alias '{}' already exists! Aborting import.", modelAlias);
		return false;
	}

	// Always load models into the tmp directory, they will be copied over next to the scene file during scene save
	const fs::path tmpDirectory = stateManager.fetchTmpDirectory();
	if (tmpDirectory.empty())
	{
		LOG_ERROR("[IMPORT] Failed to fetch tmp directory! Cannot import model.");
		return false;
	}

	// Create a dedicated model subfolder in the tmp directory to hold model files
	// - Note that this means that two models CANNOT share ANY resources among each other
	// - HOWEVER this also prevents any file name conflicts that may arise from importing models with different
	//    resources named the same
	// - Also this makes deleting model resources rather easy, just check if a model is truly unused and delete the
	//    model directory, no resources of other existing models will be affected
	Ptr<ResourceFiles> modelFiles = std::make_shared<ModelResourceFiles>(path, modelAlias);
	if (!modelFiles->fetchFiles())
	{
		LOG_ERROR("[IMPORT] Failed to fetch model files for model at '{}'! Cannot import model.", path.string());
		return false;
	}

	// Move model files to the tmp folder
	fs::path targetFolder = tmpDirectory / modelFiles->m_name;
	LOG_INFO("[IMPORT] Moving loaded model files to temp location '{}'.", targetFolder.string());
	if (!modelFiles->changeLocation(targetFolder, false))
	{
		LOG_ERROR("[IMPORT] Failed to move some model files to the temp location! Some may fail to load.");
	}

	// Load the model from the tmp folder
	LOG_INFO("[IMPORT] Loading model from scene data location ...");
	if (normalize)
		m_forceModelNormalize = true;
	Mesh* newMesh = mesh(modelAlias, modelFiles->m_path.string());
	if (normalize)
		m_forceModelNormalize = false;

	if (newMesh)
	{
		// Save the ResourceFile instance for later
		Ptr<Resource> modelResource = resourceByAlias(modelAlias);
		modelResource->resourceFiles = modelFiles;

		m_importedResources.push_back(modelAlias);
		m_resourcesToAddOnSave.insert(modelResource);

		LOG_INFO("[IMPORT] Imported  model '{}' from '{}'.", modelAlias, modelFiles->m_path.string());
	}
	else
	{
		LOG_ERROR("[IMPORT] Failed to import model '{}' from '{}'!", modelAlias, modelFiles->m_path.string());
		return false;
	}
	return true;
}

bool ResourceManager::removeImportedModel(const std::string& alias)
{
	StateManager& stateManager = App::getModule<StateManager>();

	auto resIt = m_aliasMap.find(alias);
	if (resIt == m_aliasMap.end())
	{
		LOG_ERROR("[IMPORT] Failed to find model '{}'! Cannot remove it!", alias);
		return false;
	}

	Ptr<Resource> modelResource = resIt->second.lock();
	Ptr<ResourceFiles> modelFiles = std::make_shared<ModelResourceFiles>(modelResource->path, alias);
	m_resourcesToAddOnSave.erase(modelResource);

	// Model files will be "garbage collected" later during scene save

	// Remove the model from ResourceManager
	removeResource(modelResource);

	return true;
}

Memento ResourceManager::saveScene(Scene* scene)
{
	// TODO: (DR) I haven't really thought too much how imported models should be handled during undo/redo operations,
	//   right now a load triggered by an undo/redo (a load with a NULL Scene pointer passed) is simply ignored.

	updateModelFiles(scene); // Scene file management

	// Add ResourceManager entry to the scene JSON file
	rapidjson::Document state;
	rapidjson::Document::AllocatorType& a = state.GetAllocator();
	state.SetObject();

	rapidjson::Value resources(rapidjson::kObjectType);
	resources.AddMember("imported", rapidjson::Value(rapidjson::kArrayType), a);
	serializeModels(m_importedResources, scene, resources["imported"], a);
	state.AddMember("resources", resources, a);

	return state;
}

void ResourceManager::updateModelFiles(State::Scene* scene)
{
	if (scene)
	{
		// Check if the scene location has changed compared to the last save
		if (scene->m_sceneLocationChanged)
		{
			// Copy all existing imported resource from the old save location to the new one
			// However disregard any resources that were to be removed and those to be yet added
			for (const auto& importedModel : m_importedResources)
			{
				Ptr<Resource> resource = resourceByAlias(importedModel);
				Ptr<ResourceFiles> resourceFiles = resource->resourceFiles;
				if (m_resourcesToAddOnSave.contains(resource))
					continue;

				resource->changeLocation(scene->m_dataPath / resourceFiles->m_name, false);
			}
		}

		// Copy newly imported models from tmp directory into scene data dir
		// - Update their ResourceFiles location with the scene data dir and delete the tmp files
		// - The models will still be loaded from the tmp directory but since they're already loaded their files are no
		//    longer needed
		bool error = false;
		for (const auto& newResource : m_resourcesToAddOnSave)
		{
			Ptr<ResourceFiles> newModelFiles = newResource->resourceFiles;
			error |= !newResource->changeLocation(scene->m_dataPath / newModelFiles->m_name, true);
		}
		m_resourcesToAddOnSave.clear();

		if (error)
		{
			LOG_ERROR("Failed to copy some newly imported models into the scene data directory!");
		}

		// Remove any unused models
		cleanUpModelFiles(scene);
	}
}

bool ResourceManager::cleanUpModelFiles(Scene* scene)
{
	try
	{
		if (!scene->isSaved())
			return true;

		if (!fs::exists(scene->m_dataPath) || !fs::is_directory(scene->m_dataPath))
			return true;

		// Add all models used by this scene
		std::set<std::string> usedModels(m_importedResources.begin(), m_importedResources.end());

		// Add all models of adjacent scenes, parse their saved scene json
		fs::path scenePath = scene->m_path;
		fs::path sceneRoot = scenePath.parent_path();
		for (const auto& entry : std::filesystem::directory_iterator(sceneRoot))
		{
			if (entry.path().extension() != I3T_SCENE_EXTENSION)
				continue;

			// Ignore the current scene file (The scenePath file can potentially not exist yet!)
			if (FilesystemUtils::anyEquivalent(entry.path(), scenePath))
				continue;

			// Parse the scene file
			rapidjson::Document doc;
			if (!JSON::parse(entry.path(), doc, "Data/Schemas/Scene.schema.json"))
			{
				LOG_ERROR("[RESOURCE CLEANUP]: Failed to parse scene file at '{}'!", entry.path().string());
			}
			else
			{
				if (!doc.HasMember("resources"))
				{
					// Silent fail
					continue;
				}
				for (const auto& modelEntry : deserializeModels(doc["resources"]["imported"]))
				{
					usedModels.insert(modelEntry.name);
				}
			}
		}

		// Delete models that are not used
		bool error = false;
		fs::path sceneDataPath = scene->m_dataPath;
		for (const auto& entry : std::filesystem::directory_iterator(sceneDataPath))
		{
			if (!entry.is_directory())
				continue;

			if (!usedModels.contains(entry.path().filename().string()))
			{
				error |= !FilesystemUtils::deleteFileOrDir(entry.path(), true);
			}
		}
	}
	catch (const std::filesystem::filesystem_error& e)
	{
		FilesystemUtils::reportFilesystemException(e);
		LOG_ERROR("[RESOURCE CLEANUP] An exception occurred during resource cleanup!");
		return false;
	}
	return true;
}

void ResourceManager::loadScene(const Memento& memento, Scene* scene)
{
	// TODO: (DR) I haven't really thought too much how imported models should be handled during undo/redo operations,
	//   right now a load triggered by an undo/redo (a load with a NULL Scene pointer passed) is simply ignored.
	if (scene)
	{
		int counter = 0;
		int failCounter = 0;
		LOG_INFO("[RESOURCE MANAGER] Loading imported resources ...");

		clearScene(false);

		if (!memento.HasMember("resources"))
		{
			LOG_WARN(
			    "[LOAD] Loaded scene does not have a 'resources' entry. Might be an older save. Consider updating it.");
			return;
		}

		std::vector<ModelSaveEntry> modelEntries = deserializeModels(memento["resources"]["imported"]);
		for (const auto& modelEntry : modelEntries)
		{
			// The resource path should be relative to the .scene file's parent folder
			fs::path path = scene->m_path.parent_path() / fs::path(modelEntry.path);

			// Load model resource
			if (modelEntry.normalize)
				RMI.m_forceModelNormalize = true;
			Mesh* newMesh = mesh(modelEntry.name, path.string());
			if (modelEntry.normalize)
				RMI.m_forceModelNormalize = false;

			if (newMesh)
			{
				// Fetch files for the mesh for future use
				Ptr<ResourceFiles> modelFiles = std::make_shared<ModelResourceFiles>(path.string(), modelEntry.name);
				if (!modelFiles->fetchFiles(newMesh))
				{
					LOG_ERROR("[IMPORT] Failed to fetch files for imported model '{}'!", modelEntry.name);
				}
				else
				{
					Ptr<Resource> modelResource = resourceByAlias(modelEntry.name);
					modelResource->resourceFiles = modelFiles;
				}
				counter++;
				m_importedResources.push_back(modelEntry.name);
			}
			else
			{
				LOG_ERROR("[IMPORT] Failed to load imported model '{}'!", modelEntry.name);
				failCounter++;
			}
		}

		LOG_INFO("[RESOURCE MANAGER] Loaded {} imported resources.{}", std::to_string(counter),
		         (failCounter > 0 ? std::string(" Failed to load ") + std::to_string(failCounter) + " resource" +
		                                (failCounter == 1 ? "" : "s") + "."
		                          : ""));
	}
}
void ResourceManager::appendScene(const Memento& memento, State::Scene* scene)
{
	if (scene)
	{
		int counter = 0;
		int failCounter = 0;
		LOG_INFO("[RESOURCE MANAGER] Loading imported resources ...");

		if (!memento.HasMember("resources"))
		{
			LOG_WARN(
			    "[LOAD] Loaded scene does not have a 'resources' entry. Might be an older save. Consider updating it.");
			return;
		}

		std::vector<ModelSaveEntry> modelEntries = deserializeModels(memento["resources"]["imported"]);
		for (const auto& modelEntry : modelEntries)
		{
			// The resource path should be relative to the .scene file's parent folder
			fs::path path = scene->m_path.parent_path() / fs::path(modelEntry.path);

			if (importModel(path, modelEntry.normalize))
			{
				counter++;
			}
			else
			{
				failCounter++;
			}
		}

		LOG_INFO("[RESOURCE MANAGER] Loaded {} imported resources.{}", std::to_string(counter),
		         (failCounter > 0 ? std::string(" Failed to load ") + std::to_string(failCounter) + " resource" +
		                                (failCounter == 1 ? "" : "s") + "."
		                          : ""));
	}
}

void ResourceManager::clearScene(bool newScene)
{
	// Clear imported resources
	std::vector importedResourcesCopy = std::vector(m_importedResources);
	for (auto& alias : importedResourcesCopy)
	{
		auto aliasIt = m_aliasMap.find(alias);
		if (aliasIt != m_aliasMap.end())
		{
			std::shared_ptr<Resource> ptr = aliasIt->second.lock();
			removeResource(ptr);
		}
	}

	m_resourcesToAddOnSave.clear();
}

Memento ResourceManager::saveGlobal()
{
	return emptyMemento();
}
void ResourceManager::loadGlobal(const Memento& memento) {}
void ResourceManager::clearGlobal() {}

void ResourceManager::serializeModels(std::vector<std::string> modelAliases, Scene* scene, rapidjson::Value& targetArr,
                                      rapidjson::Document::AllocatorType& a)
{
	for (const auto& importedAlias : modelAliases)
	{
		// We're assuming all Resources are models right now, as only models can be imported as of now
		rapidjson::Value resource(rapidjson::kObjectType);

		Ptr<Resource> resourcePtr = m_aliasMap.at(importedAlias).lock();
		fs::path path = fs::path(resourcePtr->path);

		if (scene)
		{
			if (FilesystemUtils::isSubpath(path, scene->m_stateManager->getTmpDirectory()))
			{
				// The path is from inside the tmp directory (newly imported model)
				fs::path pathRelToTmp = path.lexically_relative(scene->m_stateManager->getTmpDirectory());
				path = scene->m_dataPath / pathRelToTmp;
			}
			path = path.lexically_relative(scene->m_path.parent_path());
		}
		if (path.empty())
		{
			LOG_ERROR("Failed to resolve path for resource '{}'! Resource path: '{}'", importedAlias,
			          resourcePtr->path);
		}

		resource.AddMember("name", rapidjson::Value(importedAlias, a), a);
		resource.AddMember("path", rapidjson::Value(Utils::toString(path), a), a);
		resource.AddMember("type", rapidjson::Value(EnumUtils::name(ResourceType::Model), a), a);

		Mesh* mesh = std::static_pointer_cast<Mesh>(resourcePtr->data).get();
		resource.AddMember("normalize", rapidjson::Value().SetBool(mesh->m_normalized), a);

		targetArr.PushBack(std::move(resource), a);
	}
}

std::vector<ResourceManager::ModelSaveEntry> ResourceManager::deserializeModels(const rapidjson::Value& modelsEntry)
{
	std::vector<ModelSaveEntry> outList;
	for (const auto& resource : modelsEntry.GetArray())
	{
		const auto resourceName = std::string(resource["name"].GetString(), resource["name"].GetStringLength());
		const auto resourcePath = std::string(resource["path"].GetString(), resource["path"].GetStringLength());
		const auto resourceType = std::string(resource["type"].GetString(), resource["type"].GetStringLength());

		// Model specific
		bool normalize = false;
		if (resource.HasMember("normalize"))
		{
			normalize = resource["normalize"].GetBool();
		}

		const auto maybeType = magic_enum::enum_cast<Core::ResourceType>(resourceType);
		if (!maybeType.has_value())
		{
			LOG_ERROR("Resource {} has unknown type!", resource["name"].GetString());
			continue;
		}

		outList.emplace_back(resourceName, resourcePath, *maybeType, normalize);
	}
	return outList;
}

} // namespace Core
