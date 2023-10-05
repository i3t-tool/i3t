#include "Camera.h"

#include "GraphManager.h"
#include "Logger/Logger.h"

namespace Core
{
Ptr<Camera> Builder::createCamera()
{
	auto ret = std::make_shared<Core::Camera>();
	ret->init();
	ret->createComponents();
	ret->updateValues(0);

	return ret;
}

Camera::Camera() : Node(&g_cameraProperties) {}

void Camera::createComponents()
{
	m_proj = GraphManager::createSequence();
	appendChildNode(m_proj);

	m_view = GraphManager::createSequence();
	appendChildNode(m_view);

	if (GraphManager::plug(m_proj, m_view) != ENodePlugResult::Ok)
	{
		LOG_DEBUG("Components of a sequence could not be connected.");
	}

	getOutputPins()[I3T_CAMERA_OUT_MUL].setDisabled(true);
}

Ptr<Node> Camera::clone()
{
	return Builder::createCamera();
}

void Camera::updateValues(int inputIndex)
{
	const auto& projMat = m_proj->getData(I3T_OUTPUT1).getMat4();
	const auto& viewMat = m_view->getData(I3T_OUTPUT1).getMat4();

	setInternalValue(std::make_pair(projMat, viewMat), I3T_CAMERA_OUT_SCREEN);
	setInternalValue(projMat * viewMat, I3T_CAMERA_OUT_MATRIX);
	setInternalValue(viewMat, I3T_CAMERA_OUT_MUL);

	m_projectionMatrix = projMat;
	m_viewMatrix = viewMat;

	Node::updateValues(inputIndex); // Callback
}
} // namespace Core
