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

void ColoredObject::prepareRenderContext(RenderContext& context)
{
	GameObject::prepareRenderContext(context);

	switch (context.m_renderType)
	{
	case RenderType::SILHOUETTE:
	case RenderType::NORMAL:
	{
		assert(dynamic_cast<ColorShader*>(context.m_shader) != nullptr);
		bool silhouette = context.m_renderType == RenderType::SILHOUETTE;
		ColorShader* colorShader = static_cast<ColorShader*>(context.m_shader);
		colorShader->m_useSingleColor = m_useSingleColor || silhouette;
		colorShader->m_singleColor = silhouette ? m_highlightColor : m_singleColor;
	}
	break;
	default:
		break;
	}
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
