#include "ColoredObject.h"

using namespace Vp;

#include "Viewport/shader/ColorShader.h"

ColoredObject::ColoredObject(Core::Mesh* mesh, ColorShader* shader) : GameObject(mesh, shader)
{
	// Empty
}

void ColoredObject::render(Shader* shader, glm::mat4 view, glm::mat4 projection, bool silhouette)
{
	ColorShader* colorShader = static_cast<ColorShader*>(shader);
	colorShader->m_useSingleColor = m_useSingleColor || silhouette;
	colorShader->m_singleColor = silhouette ? m_highlightColor : m_singleColor;
	GameObject::render(shader, view, projection, silhouette);
}

const glm::vec3& ColoredObject::getColor() const { return m_singleColor; }

void ColoredObject::setColor(const glm::vec3* color)
{
	if (color == nullptr)
	{
		m_useSingleColor = false;
	}
	else
	{
		setColor(*color);
	}
}

void ColoredObject::setColor(const glm::vec3& mSingleColor)
{
	m_useSingleColor = true;
	m_singleColor = mSingleColor;
}
