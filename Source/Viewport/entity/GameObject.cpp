#include "GameObject.h"

#include <glm/glm.hpp>

#include "Viewport/Camera.h"
#include "Viewport/shader/ObjectShader.h"

using namespace Vp;

GameObject::GameObject(Core::Mesh* mesh, ObjectShader* shader) : m_mesh(mesh), m_shader(shader)
{
	// Empty
}

void GameObject::render(glm::mat4 view, glm::mat4 projection)
{
	m_shader->use();

	m_shader->m_opacity = m_opaque ? 1.0f : m_opacity;
	m_shader->setWorldTransform(m_modelMatrix, view, projection);

	m_shader->setUniforms();
	glBindVertexArray(m_mesh->m_vao);
	for (auto& meshPart : m_mesh->m_meshParts)
	{
		m_shader->setUniformsPerMeshPart(meshPart);
		m_mesh->renderMeshPart(meshPart);
	}
	glBindVertexArray(0);
	CHECK_GL_ERROR();
}

void GameObject::update(Scene& scene)
{
	// Empty
}

void GameObject::dispose()
{
	// Empty
}
