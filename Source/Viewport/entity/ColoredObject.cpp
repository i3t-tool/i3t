#include "ColoredObject.h"

using namespace Vp;

#include "Viewport/shader/ColorShader.h"

ColoredObject::ColoredObject(Core::Mesh* mesh, ColorShader* shader)
    : GameObject(mesh, shader)
{
	// Empty
}

void ColoredObject::render(glm::mat4 view, glm::mat4 projection)
{
	ColorShader* colorShader = static_cast<ColorShader*>(m_shader);
	colorShader->m_useSingleColor = m_useSingleColor;
	colorShader->m_singleColor = m_singleColor;
	GameObject::render(view, projection);
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
