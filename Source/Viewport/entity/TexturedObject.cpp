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