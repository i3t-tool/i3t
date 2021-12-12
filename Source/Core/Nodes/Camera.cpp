#include "Camera.h"

#include "GraphManager.h"
#include "Logger/Logger.h"

using namespace Core;

Camera::Camera() : NodeBase(&g_cameraProperties) {}

void Camera::createComponents()
{
	m_proj = Builder::createSequence();
	// m_proj->setOwner(getPtr());

	m_view = Builder::createSequence();
	m_view->setOwner(getPtr());

	if (GraphManager::plug(m_proj, m_view) != ENodePlugResult::Ok)
		Log::debug("Components of a sequence could not be connected.");
}

void Camera::updateValues(int inputIndex)
{
	// glm::mat m = m_proj->getData().getMat4() * m_view->getData().getMat4();
	glm::mat m = m_view->getData(I3T_OUTPUT2).getMat4();

	setInternalValue(m, 0);
	setInternalValue(m, 1);
	setInternalValue(m, 2);
}
