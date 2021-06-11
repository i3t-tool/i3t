#include "Camera.h"

#include "GraphManager.h"

using namespace Core;

Camera::Camera() : NodeBase(&g_cameraProperties)
{
	m_proj = Builder::createSequence();
	m_view = Builder::createSequence();
}

void Camera::updateValues(int inputIndex)
{
	m_proj->updateValues(0);
	m_view->updateValues(0);

	glm::mat m = m_proj->getData().getMat4() * m_view->getData().getMat4();

	setInternalValue(m, 1);
	setInternalValue(m, 2);
}
