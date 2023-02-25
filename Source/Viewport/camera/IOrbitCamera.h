#pragma once

#include "ICamera.h"

namespace Vp
{
/**
 * Abstract camera that turns around a point (pivot) at a certain distance (radius).
 */
class IOrbitCamera : public ICamera
{
protected:
	glm::vec3 m_pivot = glm::vec3(0.0f, 0.0f, 0.0f);
	float m_radius = 7.0f;

public:
	glm::vec3 getPivot() const;
	void setPivot(const glm::vec3& pivot);

	void setRadius(float radius);
	float getRadius() const;
};

} // namespace Vp
