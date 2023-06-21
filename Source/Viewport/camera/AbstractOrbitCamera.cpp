#include "AbstractOrbitCamera.h"

using namespace Vp;

void AbstractOrbitCamera::centerOnBox(glm::vec3 boxMin, glm::vec3 boxMax, bool interpolate)
{
	glm::vec3 centerVector = 0.5f * (boxMax - boxMin);
	glm::vec3 centerWorldPos = boxMin + centerVector;

	float radiusFactor = 3.0f;
	glm::mat4 from = m_view;
	this->setPivot(centerWorldPos);
	this->setRadius(glm::length(centerVector) * radiusFactor);
	if (interpolate)
	{
		update();
		this->interpolate(from, m_view);
	}
}

glm::vec3 AbstractOrbitCamera::getPivot() const
{
	return m_pivot;
}
void AbstractOrbitCamera::setPivot(const glm::vec3& pivot)
{
	this->m_pivot = pivot;
}

float AbstractOrbitCamera::getRadius() const
{
	return m_radius;
}
void AbstractOrbitCamera::setRadius(float radius)
{
	m_radius = radius;
}
