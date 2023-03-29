#include "ResourceManager.h"

#include "Logger/Logger.h"

using namespace Core;

#include "magic_enum.hpp"

#include "Core/Resources/Mesh.h"
#include "State/StateManager.h"
#include "Utils/Text.h"

// Used for method overloading to indicate no alias was specified
#define NO_ALIAS "no_alias_QEYjMrxoOB_rm"
// Indicates that no hashid was specified
#define NO_HASHID 0

namespace Core
{
std::optional<std::vector<Resource>> readResources(const rapidjson::Value& resources)
{
	std::vector<Resource> result;

	for (const auto& resource : resources.GetArray())
	{
		const auto name = std::string(resource["name"].GetString(), resource["name"].GetStringLength());
		const auto path = std::string(resource["path"].GetString(), resource["path"].GetStringLength());
		const auto type = std::string(resource["type"].GetString(), resource["type"].GetStringLength());

		const auto maybeType = magic_enum::enum_cast<Core::ResourceType>(type);
		if (!maybeType.has_value())
		{
			LOG_ERROR("Resource {} has unknown type!", resource["name"].GetString());
			continue;
		}

		result.emplace_back(name, path, maybeType.value());
	}

	return result;
}

ResourceManager::~ResourceManager() { dispose(); }

ResourceManager& ResourceManager::instance() { return App::getModule<ResourceManager>(); }

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

GLuint ResourceManager::texture(const std::string& path) { return texture(NO_ALIAS, path); }

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
				std::shared_ptr<Resource> textureResource = std::make_shared<Resource>(alias, path, ResourceType::Texture);
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
	size_t id = hash_string(vertShader, fragShader, geoShader);
	bool success = false;
	auto data = getData(alias, id, ResourceType::Shader, &success);
	if (success)
	{
		if (data.get() == nullptr)
		{
			// Load shader
			GLuint shaderId = loadShader(vertShader, fragShader, geoShader);
			if (shaderId)
			{
				std::string path =
				    "Vert: " + vertShader + ", Frag: " + fragShader + (geoShader.empty() ? "" : (", Geo: " + geoShader));
				std::shared_ptr<Resource> shaderResource = std::make_shared<Resource>(alias, path, ResourceType::Shader);
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
			return *(std::static_pointer_cast<Mesh*>(data).get());
		}
	}
	return nullptr;
}
Mesh* ResourceManager::mesh(const std::string& path) { return mesh(NO_ALIAS, path); }
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
				std::shared_ptr<Resource> modelResource = std::make_shared<Resource>(alias, path, ResourceType::Model);
				modelResource->hashId = id;
				modelResource->data = std::make_shared<Mesh*>(mesh);
				m_resourceMap.insert(std::make_pair(id, modelResource));
				registerAlias(alias, modelResource);
			}
			return mesh;
		}
		else
		{
			// Return existing model
			return *(std::static_pointer_cast<Mesh*>(data).get());
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
			std::string dataSummary =
			    " type: " + std::to_string(static_cast<int>(primitiveType)) + " vertices: " + std::to_string(nVertices) +
			    (useIndices ? (" indices: " + std::to_string(nIndices)) : "") + " colors: " + std::to_string(nColors);
			LOG_INFO("[MODEL] Loading model '{}' from data:{}", alias, dataSummary);

			Mesh* mesh = nullptr;
			if (useIndices)
			{
				mesh = Mesh::create(primitiveType, verts, nVertices, indices, nIndices, colors, nColors);
			}
			else
			{
				mesh = Mesh::create(primitiveType, verts, nVertices, colors, nColors);
			}

			if (mesh)
			{
				mesh->m_resourceId = id;
				std::string path = std::string("Generated from data") + dataSummary;
				std::shared_ptr<Resource> modelResource = std::make_shared<Resource>(alias, path, ResourceType::Model);
				modelResource->hashId = id;
				modelResource->data = std::make_shared<Mesh*>(mesh);
				m_resourceMap.insert(std::make_pair(id, modelResource));
				registerAlias(alias, modelResource);
			}
			return mesh;
		}
		else
		{
			// Return existing model
			return *(std::static_pointer_cast<Mesh*>(data).get());
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

void ResourceManager::importResource(const fs::path& path)
{
	const auto resourcePath = path.string();
	Resource resource(path.stem().string(), resourcePath, ResourceType::Model);

	if (Application::getModule<StateManager>().hasScene())
	{
		LOG_ERROR("Scene must be saved before import.");
		return;
	}

	/// \todo Copy new file to the scene location.

	if (mesh(resource.alias, resource.path))
	{
		m_importedResources.insert(resource.alias);
	}
}

Memento ResourceManager::getState()
{
	Memento state;

	rapidjson::Value resources(rapidjson::kObjectType);

	resources.AddMember("imported", rapidjson::Value(rapidjson::kArrayType), state.GetAllocator());
	for (const auto& importedAlias : m_importedResources)
	{
		rapidjson::Value resource(rapidjson::kObjectType);

		auto& path = m_aliasMap.at(importedAlias).lock()->path;

		resource.AddMember("name", rapidjson::Value(importedAlias, state.GetAllocator()), state.GetAllocator());
		resource.AddMember("path", rapidjson::Value(path, state.GetAllocator()), state.GetAllocator());
		resource.AddMember("type", rapidjson::Value("Mesh", state.GetAllocator()), state.GetAllocator());

		resources["imported"].PushBack(std::move(resource), state.GetAllocator());
	}

	return state;
}

void ResourceManager::setState(const Memento& memento, bool newSceneLoaded)
{
	if (newSceneLoaded)
	{
		if (auto resources = readResources(memento["resources"]["imported"]))
		{
			for (const auto& resource : *resources)
			{
				if (mesh(resource.alias, resource.path))
				{
					m_importedResources.insert(resource.alias);
				}
			}
		}
	}
}

std::shared_ptr<void> ResourceManager::getData(const std::string& alias, const size_t id, ResourceType type,
                                               bool* success)
{
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

void ResourceManager::createDefaultResources(const std::vector<Resource>& defaultResources)
{
	for (const auto& resource : defaultResources)
	{
		switch (resource.resourceType)
		{
		case ResourceType::Shader:
			// TODO: (DR) Cannot load shader from a single path, need multiple or some path convention
			break;
		case ResourceType::Texture:
			if (texture(resource.alias, resource.path))
			{
				registerDefault(resource.alias);
			}
			break;
		case ResourceType::Model:
			if (mesh(resource.alias, resource.path))
			{
				registerDefault(resource.alias);
			}
			break;
		}
	}
}

GLuint ResourceManager::loadTexture(const std::string& path)
{
	std::string absPath = path;
	LOG_INFO("[TEXTURE] Loading texture file: {}", absPath);
	GLuint id = pgr::createTexture(absPath);
	if (id == 0)
	{
		LOG_ERROR("[TEXTURE] Failed to load texture!");
	}
	return id;
}

GLuint ResourceManager::loadShader(const std::string& vertShader, const std::string& fragShader,
                                   const std::string& geoShader)
{
	std::vector<GLuint> shaderList;

	std::string absVert = vertShader;
	std::string absFrag = fragShader;
	if (geoShader.empty())
	{
		LOG_INFO("[SHADER] Loading mesh: vert: {}, frag: {}", absVert, absFrag);
		shaderList.push_back(pgr::createShaderFromFile(GL_VERTEX_SHADER, absVert));
		shaderList.push_back(pgr::createShaderFromFile(GL_FRAGMENT_SHADER, absFrag));
	}
	else
	{
		std::string absGeo = geoShader;
		LOG_INFO("[SHADER] Loading mesh: vert: {}, frag: {}, geo: {}", absVert, absFrag, absGeo);
		shaderList.push_back(pgr::createShaderFromFile(GL_VERTEX_SHADER, absVert));
		shaderList.push_back(pgr::createShaderFromFile(GL_FRAGMENT_SHADER, absFrag));
		shaderList.push_back(pgr::createShaderFromFile(GL_GEOMETRY_SHADER, absGeo));
	}

	GLuint id = pgr::createProgram(shaderList);

	if (id == 0)
	{
		LOG_ERROR("[SHADER] Failed to load mesh!");
	}
	return id;
}

Mesh* ResourceManager::loadModel(const std::string& path)
{
	LOG_INFO("[MODEL] Loading model from file: {}", path);
	Mesh* mesh = Mesh::load(path);
	if (mesh == nullptr)
	{
		LOG_ERROR("[MODEL] Failed to load model!");
	}
	return mesh;
}

void ResourceManager::disposeTexture(GLuint id) { glDeleteTextures(1, &id); }

void ResourceManager::disposeShader(GLuint id) { pgr::deleteProgramAndShaders(id); }

void ResourceManager::disposeModel(Mesh* mesh) { mesh->dispose(); }

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
		disposeModel(*(std::static_pointer_cast<Mesh*>(resource->data).get()));
		break;
	default:
		LOG_WARN("[RESOURCE MANAGER] Unknown resource type to dispose! Id: {}, Type: {}, Path: {}", resource->hashId,
		         std::string(magic_enum::enum_name(resource->resourceType)), resource->path);
		break;
	}
}

bool ResourceManager::dispose(ManagedResource* managedResource)
{
	size_t id = managedResource->m_resourceId;
	auto resourceIt = m_resourceMap.find(id);
	if (resourceIt != m_resourceMap.end())
	{
		std::shared_ptr<Resource> resource = resourceIt->second;
		// Remove alias and alternative aliases, remove default aliases too
		m_aliasMap.erase(resource->alias);
		m_defaultResources.erase(resource->alias);
		std::string alternativeAliases;
		for (const auto& alias : resource->alternativeAliases)
		{
			alternativeAliases += "|" + alias;
			m_aliasMap.erase(alias);
			m_defaultResources.erase(alias);
		}

		// Remove resource
		disposeResource(resource);
		m_resourceMap.erase(resourceIt);

		LOG_INFO("[RESOURCE MANAGER] Deleted resource id: {}, alias: {}{}.", id, resource->alias, alternativeAliases);
		return true;
	}
	else
	{
		LOG_WARN("[RESOURCE MANAGER] Cannot dispose resource! Resource with id {} does not exist.", id);
		return false;
	}
}
} // namespace Core
