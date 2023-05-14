#include "Camera.h"

#include "GraphManager.h"
#include "Logger/Logger.h"

using namespace Core;

Ptr<Camera> Builder::createCamera()
{
	auto ret = std::make_shared<Core::Camera>();
	ret->init();
	ret->createComponents();
	ret->updateValues(0);

	return ret;
}

Camera::Camera() : NodeBase(&g_cameraProperties) {}

void Camera::createComponents()
{
	m_proj = GraphManager::createSequence();
	/// \todo MH Uncomment this and test. (DR: uncommented this because I needed it, seems to be fine)
	m_proj->setOwner(getPtr().get());

	m_view = GraphManager::createSequence();
	m_view->setOwner(getPtr().get());

	if (GraphManager::plug(m_proj, m_view) != ENodePlugResult::Ok)
	{
		LOG_DEBUG("Components of a sequence could not be connected.");
	}

	getOutputPins()[I3T_CAMERA_OUT_MUL].setDisabled(true);
}

Ptr<Node> Camera::clone() { return Builder::createCamera(); }

void Camera::updateValues(int inputIndex)
{
	// glm::mat m = m_proj->getData().getMat4() * m_view->getData().getMat4();
	glm::mat m = m_view->getData(I3T_OUTPUT2).getMat4();

	glm::mat projMat = m_proj->getData(I3T_OUTPUT1).getMat4();
	glm::mat viewMat = m_view->getData(I3T_OUTPUT1).getMat4();

	setInternalValue(std::make_pair(projMat, viewMat), 0); // Screen
	setInternalValue(m, 1);                                // Matrix
	setInternalValue(m, 2);                                // MatrixMult

	m_projectionMatrix = projMat;
	m_viewMatrix = viewMat;

	Node::updateValues(inputIndex); // Callback
}
