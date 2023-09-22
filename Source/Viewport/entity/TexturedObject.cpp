#include "TexturedObject.h"

#include "Utils/HSLColor.h"

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
		glm::vec3 color;
		glm::vec3 hsl;
		rgbToHsl(m_tint.r, m_tint.g, m_tint.b, &hsl.x, &hsl.y, &hsl.z);
		hsl.z = (hsl.z * m_tintStrength) + (1.0f - m_tintStrength);
		hslToRgb(hsl.x, hsl.y, hsl.z, &color.r, &color.g, &color.b);
		phongShader->m_tint = color;
	}
	GameObject::render(shader, view, projection, silhouette);
}