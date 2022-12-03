#include "TexturedObject.h"

#include "Viewport/shader/PhongShader.h"

using namespace Vp;

TexturedObject::TexturedObject(Core::Mesh* mesh, PhongShader* shader) : GameObject(mesh, shader)
{
	// Empty
}

void TexturedObject::render(glm::mat4 view, glm::mat4 projection)
{
	PhongShader* phongShader = static_cast<PhongShader*>(m_shader);
	phongShader->m_tint = m_tint;
	GameObject::render(view, projection);
}