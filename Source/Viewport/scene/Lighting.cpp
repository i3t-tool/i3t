#include "Lighting.h"

#include "Viewport/scene/lights/PointLight.h"
#include "Viewport/scene/lights/SpotLight.h"
#include "Viewport/scene/lights/SunLight.h"

#include "Viewport/shader/PhongShader.h"

using namespace Vp;

void Lighting::setUniforms(const PhongShader& shader) const
{
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
	glUniform1i(glGetUniformLocation(shader.id, "pointLightsCount"), pointLightCount);
	glUniform1i(glGetUniformLocation(shader.id, "sunLightsCount"), sunLightCount);
	glUniform1i(glGetUniformLocation(shader.id, "spotLightsCount"), spotLightCount);
}

void Lighting::addLight(Light* light)
{
	const auto newLight = std::shared_ptr<Light>(light);
	lights.push_back(newLight);
}
void Lighting::addLight(const std::shared_ptr<Light>& light) { lights.push_back(light); }
void Lighting::removeLight(std::shared_ptr<Light>& light)
{
	auto it = std::find(lights.begin(), lights.end(), light);
	if (it != lights.end())
	{
		lights.erase(it);
	}
}
std::vector<std::shared_ptr<Light>> Lighting::getLights() const { return lights; }