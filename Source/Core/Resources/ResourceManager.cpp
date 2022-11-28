#include "ResourceManager.h"

#include "sceneGraph/ShaderProgram.h"

#include "Logger/Logger.h"

namespace Core
{
void ResourceManager::init(const std::vector<Resource>& defaultResources)
{
	using MeshManager = pgr::sg::MeshManager;
	using ShaderManager = pgr::sg::ShaderManager;

	// get default mesh shader
	for (const auto& resource : defaultResources)
	{
		if (resource.Type == EResourceType::Shader)
		{
			const auto vs = resource.Path.string() + ".vert";
			const auto fs = resource.Path.string() + ".frag";

			m_nameToPath.insert({resource.ID, resource.Path.string()});
			// ShaderManager::Instance()->get(resource.ID);

			GLuint shaderList[] = {pgr::createShaderFromFile(GL_VERTEX_SHADER, vs),
			                       pgr::createShaderFromFile(GL_FRAGMENT_SHADER, fs),
			                       0};

			auto* program =
			    new pgr::sg::MeshShaderProgram(pgr::createProgram(shaderList));

			ShaderManager::Instance()->insert(m_nameToPath.at(resource.ID), program);
		}
	}

	for (const auto& resource : defaultResources)
	{
		// load resource
		if (resource.Type == EResourceType::Model)
		{
			m_nameToPath.insert({resource.ID, resource.Path.string()});

			MeshNode* model = new MeshNode();
			pgr::sg::MeshGeometry* mesh_p =
			    (pgr::sg::MeshGeometry*)MeshManager::Instance()->get(
			        m_nameToPath.at(resource.ID));
			model->setGeometry(mesh_p);
			m_models.insert({resource.ID, model});
		}
	}
}

void ResourceManager::destroy()
{
	for (const auto [_, mesh] : m_models)
	{
		delete mesh;
	}
}

const ModelsMap& ResourceManager::defaultModels() { return m_models; }

pgr::sg::BasicShaderProgram* ResourceManager::getShader(const std::string& name)
{
	using ShaderManager = pgr::sg::ShaderManager;

	return ShaderManager::Instance()->get(m_nameToPath.at(name));
}

} // namespace Core
