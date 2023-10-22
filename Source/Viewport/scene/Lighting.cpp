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
#include "Lighting.h"

#include "Viewport/scene/lights/PointLight.h"
#include "Viewport/scene/lights/SpotLight.h"
#include "Viewport/scene/lights/SunLight.h"

#include "Viewport/shader/PhongShader.h"

using namespace Vp;

void Lighting::setUniforms(const PhongShader& shader) const
{
	// TODO: (DR) Do not use glGetShaderLocation in lights
	// TODO: (DR) Refactor this (get rid of dynamic casts) and possibly use UBOs
	int pointLightCount = 0;
	int sunLightCount = 0;
	int spotLightCount = 0;
	for (auto& light : lights)
	{
		if (dynamic_cast<PointLight*>(light.get()))
		{
			light->setUniforms(shader, pointLightCount);
			pointLightCount++;
		}
		else if (dynamic_cast<SunLight*>(light.get()))
		{
			light->setUniforms(shader, sunLightCount);
			sunLightCount++;
		}
		else if (dynamic_cast<SpotLight*>(light.get()))
		{
			light->setUniforms(shader, spotLightCount);
			spotLightCount++;
		}
	}
	glUniform1i(glGetUniformLocation(shader.m_id, "pointLightsCount"), pointLightCount);
	glUniform1i(glGetUniformLocation(shader.m_id, "sunLightsCount"), sunLightCount);
	glUniform1i(glGetUniformLocation(shader.m_id, "spotLightsCount"), spotLightCount);
}

void Lighting::addLight(Light* light)
{
	const auto newLight = std::shared_ptr<Light>(light);
	lights.push_back(newLight);
}
void Lighting::addLight(const std::shared_ptr<Light>& light)
{
	lights.push_back(light);
}
void Lighting::removeLight(std::shared_ptr<Light>& light)
{
	auto it = std::find(lights.begin(), lights.end(), light);
	if (it != lights.end())
	{
		lights.erase(it);
	}
}
std::vector<std::shared_ptr<Light>> Lighting::getLights() const
{
	return lights;
}