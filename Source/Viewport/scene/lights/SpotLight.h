#pragma once

#include "Light.h"

namespace Vp
{
class SpotLight : public Light
{
public:
	float radius = 70.0f;
	float cutoffAngle = 10;
	float cutoffSoftAngle = 6;
	glm::vec3 direction = glm::vec3(0, 0, -1);

	void setUniforms(const PhongShader& shader, int index) const override;
};
} // namespace Vp