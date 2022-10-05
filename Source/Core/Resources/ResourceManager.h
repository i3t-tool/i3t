#pragma once

#include "Core/Defs.h"

#include <map>
#include <string>

#include "sceneGraph/MeshGeometry.h"
#include "sceneGraph/MeshNode.h"
#include "sceneGraph/Resources.h"

#include "Core/Resources/Mesh.h"
#include "Core/Resources/Resource.h"

namespace Core
{
using ModelsMap = std::map<std::string, MeshNode*>;

/**
 * Interface for managing resources
 *
 * ResourceManager uses TextureManager, MeshManager and ShaderManager from
 * pgr-framework.
 */
class ResourceManager : public Singleton<ResourceManager>
{
public:
	~ResourceManager() { destroy(); };

	/**
	 * Creates all resources.
	 *
	 * \param defaultResources List of default resources created from config.
	 */
	void init(const std::vector<Resource>& defaultResources);

	const ModelsMap& defaultModels();

	/// \todo
	// const std::vector<Resource>& customModels();

	/// \throws an exception when key \p name does not exist.
	const MeshNode* getModel(const std::string& name) const
	{
		return m_models.at(name);
	}

	pgr::sg::BasicShaderProgram* getShader(const std::string& name);

private:
	/**
	 * Destroys all resources in correct order.
	 */
	void destroy();

	// std::vector<Resource> m_models;
	ModelsMap m_models;
	std::map<std::string, std::string> m_nameToPath;
};
} // namespace Core
