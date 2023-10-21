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

const glm::vec3& ColoredObject::getColor() const
{
	return m_singleColor;
}

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
