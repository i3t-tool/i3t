#include "TexturedObject.h"

#include "Viewport/shader/ColorShader.h"
#include "Viewport/shader/PhongShader.h"
#include "Viewport/shader/Shaders.h"

using namespace Vp;

TexturedObject::TexturedObject(Core::Mesh* mesh, PhongShader* shader) : GameObject(mesh, shader)
{
	// Empty
}

void TexturedObject::render(Shader* shader, glm::mat4 view, glm::mat4 projection, bool silhouette)
{
	if (silhouette)
	{
		auto colorShader = Shaders::instance().m_colorShader;
		colorShader->m_useSingleColor = true;
		colorShader->m_singleColor = m_highlightColor;
		shader = colorShader.get();
	}
	else
	{
		PhongShader* phongShader = static_cast<PhongShader*>(shader);
		phongShader->m_tint = m_tint;
	}
	GameObject::render(shader, view, projection, silhouette);
}