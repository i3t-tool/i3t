/**
 * \file
 * \brief
 * \author Martin Herich
 * \copyright Copyright (C) 2016-2023 I3T team, Department of Computer Graphics
 * and Interaction, FEE, Czech Technical University in Prague, Czech Republic
 *
 * This file is part of I3T - An Interactive Tool for Teaching Transformations
 * http://www.i3t-tool.org
 *
 * GNU General Public License v3.0 (see LICENSE.txt or https://www.gnu.org/licenses/gpl-3.0.txt)
 */
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

Camera::Camera() : Node(g_cameraProperties) {}

void Camera::createComponents()
{
	m_viewport = GraphManager::createSequence();
	appendChildNode(m_viewport);

	m_proj = GraphManager::createSequence();
	appendChildNode(m_proj);

	m_view = GraphManager::createSequence();
	appendChildNode(m_view);

	// TODO: Sequences in the camera cannot be connected together as they have the same parent Owner, resulting in
	//  "Err_Loopback", the outputs of the camera node are calculated manually, not using the connection so this was
	//  likely simply forgotten about and it silently kept failing, commented it out for now.
	// ENodePlugResult result = GraphManager::plug(m_proj, m_view);
	// if (result != ENodePlugResult::Ok)
	// {
	// 	LOG_DEBUG("Camera sequences could not be connected ({})!", EnumUtils::name(result));
	// }
}

void Camera::updateValues(int inputIndex)
{
	const auto& projMat = m_proj->data(I3T_OUTPUT1).getMat4();
	const auto& viewMat = m_view->data(I3T_OUTPUT1).getMat4();
	const auto& viewportMat = m_viewport->data(I3T_OUTPUT1).getMat4();

	// TODO: (DR) [T-VIEWPORT] I assume the screen should be passed the viewport transform if enabled?
	setInternalValue(std::make_pair(projMat, viewMat), I3T_CAMERA_OUT_SCREEN);
	setInternalValue(projMat * viewMat, I3T_CAMERA_OUT_MATRIX);
	setInternalValue(glm::mat4(1.f), I3T_CAMERA_OUT_MUL);

	m_projectionMatrix = projMat;
	m_viewMatrix = viewMat;
	m_viewportMatrix = viewportMat;

	Node::updateValues(inputIndex); // Callback
}

bool Camera::isEmpty() const
{
	return m_proj->isEmpty() && m_view->isEmpty() && (!m_viewportEnabled ? true : m_viewport->isEmpty());
}
} // namespace Core
