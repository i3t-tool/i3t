#include "IOrbitCamera.h"

using namespace Vp;

void IOrbitCamera::centerOnBox(glm::vec3 boxMin, glm::vec3 boxMax)
{
	glm::vec3 centerVector = 0.5f * (boxMax - boxMin);
	glm::vec3 centerWorldPos = boxMin + centerVector;

	float radiusFactor = 2.0f;
	glm::mat4 from = m_view;
	this->setPivot(centerWorldPos);
	this->setRadius(glm::length(centerVector) * radiusFactor);
	update();
	interpolate(from, m_view);
}

glm::vec3 IOrbitCamera::getPivot() const { return m_pivot; }
void IOrbitCamera::setPivot(const glm::vec3& pivot) { this->m_pivot = pivot; }

float IOrbitCamera::getRadius() const { return m_radius; }
void IOrbitCamera::setRadius(float radius) { m_radius = radius; }
