#include "GameObject.h"

#include <glm/glm.hpp>

#include "Viewport/shader/ObjectShader.h"
#include "Viewport/shader/Shaders.h"

using namespace Vp;

GameObject::GameObject(Core::Mesh* mesh, ObjectShader* shader) : m_mesh(mesh)
{
	m_shader = shader;
}

void GameObject::render(Shader* shader, glm::mat4 view, glm::mat4 projection, bool silhouette)
{
	auto objectShader = static_cast<ObjectShader*>(shader);
	objectShader->use();
	objectShader->m_wboit = m_wboit;
	objectShader->m_wboitFunc = m_wboitFunc;
	objectShader->m_opacity = m_opaque || silhouette ? 1.0f : m_opacity;
	objectShader->setWorldTransform(m_modelMatrix, view, projection);
	objectShader->setUniforms();

	glBindVertexArray(m_mesh->m_vao);
	for (auto& meshPart : m_mesh->m_meshParts)
	{
		objectShader->setUniformsPerMeshPart(meshPart);
		m_mesh->renderMeshPart(meshPart);
	}
	glBindVertexArray(0);
}

void GameObject::update(Scene& scene)
{
	// Empty
}

void GameObject::dispose()
{
	// Empty
}
