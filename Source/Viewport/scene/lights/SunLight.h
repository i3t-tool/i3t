#pragma once

#include "Light.h"

namespace Vp
{
class SunLight : public Light
{
public:
	glm::vec3 direction = glm::vec3(0.8, 0, -1);

	void setUniforms(const PhongShader& shader, int index) const override;
};
} // namespace Vp