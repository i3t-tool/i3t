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
#pragma once

#include <algorithm>
#include <set>
#include <unordered_map>
#include <vector>

#include "pgr.h"

#include "Core/Application.h"
#include "Core/Defs.h"
#include "Core/Module.h"
#include "Core/Resources/Mesh.h"
#include "Core/Resources/Resource.h"
#include "ResourceFiles.h"
#include "State/Stateful.h"
#include "Utils/JSON.h"

#define RM Core::ResourceManager
#define RMI Core::ResourceManager::instance()

class StateManager;

namespace Core
{
class Resource;

// TODO: (DR) Move away from Core
// TODO: (DR) Add clear method
// TODO: (DR) Add const char* and fs::path argument alternatives
// TODO: (DR) Maybe use std::filesystem::abs/relative path methods to resolve paths
// TODO: (DR) Two relative paths may load the same file twice.
// 	Config::getAbsolutePath doesn't quite work.
// 	Any provided path is used as a key, even if the file was loaded from a different path already.
// TODO: (DR) Unicode support?
// TODO: (DR) How to resolve alias conflict? Exception? Silent fail?

/**
 * Resource manager for loading resources from the filesystem.
 * Can be accessed from a singleton ResourceManager::instance()
 *
 * Currently loads and manages models, shaders and textures.
 *
 * Models are loaded using assimp or created directly from vertex arrays.
 *
 * Resources are uniquely identified using a hash. Usually a hash of their file
 * path string. Each resource can have an alias. A single string using which it
 * can be retrieved. Aliases cannot be reassigned and point to a single resource
 * type.
 *
 * Finally a resource can be marked as default and returned using
 * getDefaultResources().
 *
 * An RM define is provided to shorten "Core::ResourceManager"
 * An RMI define shortens "Core::ResourceManager::instance()"
 */
class ResourceManager : public Module, public IStateful
{
private:
	/// Map of hashIds and owning resource pointers [hashid, resource]
	std::unordered_map<size_t, std::shared_ptr<Resource>> m_resourceMap;
	/// Map of aliases and weak resource pointers [alias, resource]
	std::unordered_map<std::string, std::weak_ptr<Resource>> m_aliasMap;

	/// List of default resource aliases [alias]
	std::set<std::string> m_defaultResources;

	/// List of resources imported into the current scene [alias]
	std::vector<std::string> m_importedResources;

	// Comparator for ResourceFiles sets
	struct ResFilesCmp
	{
		bool operator()(Ptr<Resource> lhs, Ptr<Resource> rhs) const;
	};
	/// List of lists of files of newly added resources whose files should be moved to data dir on next save
	std::set<Ptr<Resource>, ResFilesCmp> m_resourcesToAddOnSave;

public:
	// NOTE: That these flags are NOT atomic
	bool m_forceReload{false};         /// If true any resource fetches will not be cached
	bool m_forceMinimumLoad{false};    /// If true just the bare minimum of resources is loaded
	bool m_forceModelNormalize{false}; /// If true loaded models will be normalized

	~ResourceManager();

	static ResourceManager& instance();

	bool resourceExists(size_t id);
	bool resourceExists(std::string& alias);

	std::vector<std::string> getImportedResourceAliases();

	/**
	 * Dispose the specified resource and remove it from the manager.
	 * @param managedResource The actual resource returned by fetch methods.
	 * @return false on failure
	 */
	bool removeResource(ManagedResource* managedResource, bool force = false);

private:
	/**
	 * Remove resource = Dispose resource and remove it from all applicable data structures
	 * Dispose resource = Just deallocate the resource's memory
	 * @param force Whether to remove default resources
	 */
	bool removeResource(std::shared_ptr<Resource>& resource, bool force = false);

public:
	/**
	 * Get existing texture using an alias
	 * @return OpenGL id of the texture object
	 */
	GLuint textureByAlias(const std::string& alias);

	/**
	 * Get or create texture for file path.
	 * @return OpenGL id of the texture object
	 */
	GLuint texture(const std::string& path);

	/**
	 * Get or create texture by alias or path.
	 * Alias is assigned if it doesn't exist.
	 * @return OpenGL id of the texture object
	 */
	GLuint texture(const std::string& alias, const std::string& path);

	/**
	 * Get existing shader using an alias.
	 * @return OpenGL id of the shader object
	 */
	GLuint shaderByAlias(const std::string& alias);

	/**
	 * Get or create shader from files.
	 * @return OpenGL id of the shader object
	 */
	GLuint shader(const std::string& vertShader, const std::string& fragShader);

	/**
	 * Get or create shader by alias or files.
	 * Alias is assigned if it doesn't exist.
	 * @return OpenGL id of the shader object
	 */
	GLuint shader(const std::string& alias, const std::string& vertShader, const std::string& fragShader);

	/**
	 * Get or create shader from files.
	 * @return OpenGL id of the shader object
	 */
	GLuint shaderG(const std::string& vertShader, const std::string& fragShader, const std::string& geoShader);

	/**
	 * Get or create shader by alias or files.
	 * Alias is assigned if it doesn't exist.
	 * @return OpenGL id of the shader object
	 */
	GLuint shaderG(const std::string& alias, const std::string& vertShader, const std::string& fragShader,
	               const std::string& geoShader);

	/**
	 * Get existing model using an alias.
	 * @return Pointer to the Mesh object.
	 */
	Mesh* meshByAlias(const std::string& alias);

	/**
	 * Get or create model from file.
	 * Necessary textures might be loaded as well.
	 * @return Pointer to the Mesh object.
	 */
	Mesh* mesh(const std::string& path);

	/**
	 * Get or create model by alias or file.
	 * Necessary textures might be loaded as well.
	 * Alias is assigned if it doesn't exist.
	 * @return Pointer to the Mesh object.
	 */
	Mesh* mesh(const std::string& alias, const std::string& path);

	/**
	 * Get model by alias or create it using provided vertex data.
	 * Alias is assigned if it doesn't exist.
	 * @param alias Resource alias
	 * @param primitiveType Mesh primitive type
	 * @param verts Pointer to vertex position array
	 * @param nVertices Vertex position array size
	 * @param colors Pointer to color array
	 * @param nColors Color array size
	 * @return Pointer to the Mesh object.
	 */
	Mesh* mesh(const std::string& alias, Mesh::PrimitiveType primitiveType, const float* verts,
	           const unsigned int nVertices, const float* colors, const unsigned int nColors);

	/**
	 * Get model by alias or create it using provided indexed vertex data.
	 * Alias is assigned if it doesn't exist.
	 * @param alias Resource alias
	 * @param primitiveType Mesh primitive type
	 * @param verts Pointer to vertex position array
	 * @param nVertices Vertex position array size
	 * @param indices Pointer to indices array
	 * @param nIndices Indices array size
	 * @param colors Pointer to color array
	 * @param nColors Color array size
	 * @return Pointer to the Mesh object.
	 */
	Mesh* mesh(const std::string& alias, Mesh::PrimitiveType primitiveType, const float* verts,
	           const unsigned int nVertices, const unsigned int* indices, const unsigned int nIndices,
	           const float* colors, const unsigned int nColors);

	//////////////////////////////////////////////////////
	// MODEL LOAD/SAVE
	//////////////////////////////////////////////////////

	struct ModelSaveEntry
	{
		std::string name;
		std::string path;
		Core::ResourceType type;
		bool normalize;
	};

	bool importModel(const fs::path& path, bool normalize);
	bool removeImportedModel(const std::string& alias);

	std::vector<ModelSaveEntry> deserializeModels(const rapidjson::Value& modelsEntry);
	void serializeModels(std::vector<std::string> modelAliases, Scene* scene, rapidjson::Value& targetArr,
	                     rapidjson::Document::AllocatorType& a);

	//////////////////////////////////////////////////////
	// SCENE LOAD/SAVE
	//////////////////////////////////////////////////////

	Memento saveScene(State::Scene* scene) override;
	/// Ensure that model files required by the scene are in the right place
	void updateModelFiles(State::Scene* scene);
	/// Ensure there are no unused model files in the scene_data directory of the passed scene.
	bool cleanUpModelFiles(State::Scene* scene);

	void loadScene(const Memento& memento, State::Scene* scene) override;
	void appendScene(const Memento& memento, State::Scene* scene) override;
	void clearScene() override;

	Memento saveGlobal() override;
	void loadGlobal(const Memento& memento) override;
	void clearGlobal() override;

	//////////////////////////////////////////////////////
	// DEFAULT RESOURCES
	//////////////////////////////////////////////////////

	/**
	 * Loads resources from the a json document and marks them as default.
	 * The json document must contain an "defaultResources" entry.
	 */
	void loadDefaultResources(rapidjson::Document& doc);

	/**
	 * Register an alias as a "default" resource.
	 * The aliased resource must exist.
	 */
	void registerDefault(const std::string& alias);

	/**
	 * Returns default resources of type.
	 */
	std::vector<Resource> getDefaultResources(ResourceType type);

private:
	/**
	 * Return value:
	 * 		- false -> Error, data pointer is null, abort
	 * 		- true -> Data ptr != null -> data ptr is the requested data
	 * 				  Data ptr == null -> data needs to be created/loaded and alias
	 * registered afterwards!
	 */
	std::shared_ptr<void> getData(const std::string& alias, const size_t id, ResourceType type, bool* success);

	/**
	 * Assumes alias doesn't exist and the resource does
	 */
	void registerAlias(const std::string& alias, std::shared_ptr<Resource> resource);

	Ptr<Resource> resourceByAlias(const std::string& alias);

	GLuint loadTexture(const std::string& path);

	/**
	 * @param vertShader Path to vertex shader
	 * @param fragShader Path to fragment shader
	 * @param geoShader Optional path to geometry shader, or an empty string
	 * @return OpenGL id of created shader program
	 */
	GLuint loadShader(const std::string& vertShader, const std::string& fragShader, const std::string& geoShader);

	Mesh* loadModel(const std::string& path);

	void disposeTexture(GLuint id);
	void disposeShader(GLuint id);
	void disposeModel(Mesh* mesh);

	void dispose();
	void disposeResource(std::shared_ptr<Resource>& resource);
};
} // namespace Core
