/**
 * \file
 * \brief
 * \author Dan Rakušan <rakusan.dan@gmail.com>
 * \copyright Copyright (C) 2016-2023 I3T team, Department of Computer Graphics
 * and Interaction, FEE, Czech Technical University in Prague, Czech Republic
 *
 * This file is part of I3T - An Interactive Tool for Teaching Transformations
 * http://www.i3t-tool.org
 *
 * GNU General Public License v3.0 (see LICENSE.txt or https://www.gnu.org/licenses/gpl-3.0.txt)
 */
#include "SceneCamera.h"

#include "Viewport/Shaper.h"
#include "Viewport/entity/FrustumObject.h"
#include "Viewport/shader/FrustumShader.h"

#include "Core/Resources/ResourceManager.h"
#include "Utils/Color.h"
#include "Viewport/scene/Scene.h"

using namespace Vp;

SceneCamera::SceneCamera(Core::Mesh* mesh, PhongShader* shader) : SceneModel(mesh, shader)
{
	setDisplayType(DisplayType::Camera);
}

void SceneCamera::update(Scene& scene)
{
	// Calculate model matrix from the camera's view matrix
	m_modelMatrix = glm::inverse(m_viewMatrix);

	auto frustumPtr = m_frustumOutline.lock();
	frustumPtr->m_visible = m_showFrustum;
	frustumPtr->m_frustumProjectionMatrix = m_projectionMatrix;
	frustumPtr->m_frustumViewMatrix = m_viewMatrix;
	frustumPtr->m_frustumViewMatrixInv = m_modelMatrix;
	frustumPtr->setColor(m_frustumOutlineColor);

	frustumPtr = m_frustum.lock();
	frustumPtr->m_visible = m_showFrustum && m_fillFrustum;
	frustumPtr->m_frustumProjectionMatrix = m_projectionMatrix;
	frustumPtr->m_frustumViewMatrix = m_viewMatrix;
	frustumPtr->m_frustumViewMatrixInv = m_modelMatrix;
	frustumPtr->setColor(m_frustumColor);

	SceneModel::update(scene);
}

void SceneCamera::onSceneAdd(Vp::Scene& scene)
{
	FrustumShader* frustumShader = SHADERS.getShaderPtr<FrustumShader>();

	SceneModel::onSceneAdd(scene);
	auto frustumOutline = std::make_shared<FrustumObject>(RMI.meshByAlias(Shaper::unitLineCube), frustumShader);
	frustumOutline->setColor(m_frustumOutlineColor);
	m_frustumOutline = scene.addEntity(frustumOutline);

	auto frustumFilled = std::make_shared<FrustumObject>(RMI.meshByAlias(Shaper::unitCube), frustumShader);
	frustumFilled->setColor(m_frustumColor);
	frustumFilled->m_opaque = false;
	frustumFilled->m_opacity = 0.22f;
	frustumFilled->m_visible = m_fillFrustum;
	m_frustum = scene.addEntity(frustumFilled);
}

void SceneCamera::onSceneRemove(Vp::Scene& scene)
{
	SceneModel::onSceneRemove(scene);
	scene.removeEntity(m_frustumOutline);
	scene.removeEntity(m_frustum);
}
