#pragma once

#include "glm/glm.hpp"

namespace Vp
{
class PhongShader;

class Light
{
public:
	virtual ~Light() = default;

	glm::vec3 pos{glm::vec3(0.0f)};
	glm::vec3 color{glm::vec3(1)};
	glm::vec3 specular{glm::vec3(1)};
	float intensity{0.6f};
	virtual void setUniforms(const PhongShader& shader, int index) const = 0;
};
} // namespace Vp