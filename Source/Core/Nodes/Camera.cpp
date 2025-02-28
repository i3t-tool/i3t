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

void Camera::updateValues(int inputIndex)
{
	const auto& projMat = m_proj->data(I3T_OUTPUT1).getMat4();
	const auto& viewMat = m_view->data(I3T_OUTPUT1).getMat4();

	setInternalValue(std::make_pair(projMat, viewMat), I3T_CAMERA_OUT_SCREEN);
	setInternalValue(projMat * viewMat, I3T_CAMERA_OUT_MATRIX);
	setInternalValue(viewMat, I3T_CAMERA_OUT_MUL);

	m_projectionMatrix = projMat;
	m_viewMatrix = viewMat;

	Node::updateValues(inputIndex); // Callback
}
} // namespace Core
