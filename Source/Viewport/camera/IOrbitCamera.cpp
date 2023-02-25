#include "IOrbitCamera.h"

using namespace Vp;

glm::vec3 IOrbitCamera::getPivot() const { return m_pivot; }
void IOrbitCamera::setPivot(const glm::vec3& pivot) { this->m_pivot = pivot; }

float IOrbitCamera::getRadius() const { return m_radius; }
void IOrbitCamera::setRadius(float radius) { m_radius = radius; }