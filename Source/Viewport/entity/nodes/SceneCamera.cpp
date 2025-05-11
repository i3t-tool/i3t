/**
 * \file
 * \brief
 * \author Dan Rakušan <rakusan.dan@gmail.com>
 * \copyright Copyright (C) 2016-2025 I3T team, Department of Computer Graphics
 * and Interaction, FEE, Czech Technical University in Prague, Czech Republic
 *
 * This file is part of I3T - An Interactive Tool for Teaching Transformations
 * http://www.i3t-tool.org
 *
 * GNU General Public License v3.0 (see LICENSE.txt or https://www.gnu.org/licenses/gpl-3.0.txt)
 */
#include "SceneCamera.h"

#include "Core/Nodes/GraphManager.h"
#include "Viewport/Shaper.h"
#include "Viewport/entity/FrustumObject.h"
#include "Viewport/shader/FrustumShader.h"

#include "Core/Resources/ResourceManager.h"
#include "Utils/Color.h"
#include "Utils/ProjectionUtils.h"
#include "Viewport/scene/Scene.h"

using namespace Vp;

SceneCamera::SceneCamera(Core::Mesh* mesh, PhongShader* shader) : SceneModel(mesh, shader)
{
	setDisplayType(DisplayType::Camera);
}
SceneCamera::~SceneCamera()
{
	if (m_frustumNearMesh)
		m_frustumNearMesh->dispose();
	if (m_trackedFrustumNearMesh)
		m_trackedFrustumNearMesh->dispose();
}

void SceneCamera::update(Scene& scene)
{
	// Calculate model matrix from the camera's view matrix
	glm::mat4 viewInv = glm::inverse(m_viewMatrix);
	glm::mat4 projInv = glm::inverse(m_projectionMatrix);
	glm::mat4 projViewInv = viewInv * projInv;

	m_modelMatrix = viewInv;

	auto frustumPtr = m_frustumOutline.lock();
	frustumPtr->m_visible = m_showFrustum;
	frustumPtr->m_frustumProjectionViewMatrixInv = projViewInv;
	frustumPtr->m_frustumViewMatrixInv = viewInv;
	frustumPtr->setColor(m_frustumOutlineColor);

	frustumPtr = m_frustum.lock();
	frustumPtr->m_visible = m_showFrustum && m_fillFrustum;
	frustumPtr->m_frustumProjectionViewMatrixInv = projViewInv;
	frustumPtr->m_frustumViewMatrixInv = viewInv;
	frustumPtr->setColor(m_frustumColor);

	auto frustumFillPtr = m_frustum.lock();
	frustumFillPtr->m_opacity = m_frustumColor.w;

	// Show tracked camera model only when this is the tracked camera
	m_isTracking = false;
	if (Core::GraphManager::isTracking())
	{
		if (auto& trackedCam = Core::GraphManager::getTracker()->getTrackedCamera())
		{
			if (!trackedCam->node.expired())
				m_isTracking = trackedCam->node.lock()->getId() == m_guiNodeId;
		}
	}
	m_trackedCameraModel.lock()->m_visible = m_isTracking && m_visible;
	m_trackedFrustumNear.lock()->m_visible = m_isTracking && m_showFrustum;

	SceneModel::update(scene);
}
void SceneCamera::render(const glm::mat4& model, const glm::mat4& view, const glm::mat4& projection,
                         const RenderContext& context)
{
	auto frustumOutlinePtr = m_frustumOutline.lock();
	updateNearFrustumIndicator(model, frustumOutlinePtr->m_frustumProjectionViewMatrixInv);

	if (!context.displayOptions || !context.displayOptions->showTracking)
	{
		m_frustum.lock()->m_visualizeDepth = false;
		m_frustumNear.lock()->m_visible = m_showFrustum;
		m_axes.lock()->m_visible = m_showAxes;
		// Only render the camera when not tracking
		if (m_showCamera) // Avoid rendering of itself if m_showCamera is false
			SceneModel::render(model, view, projection, context);
	}
	else
	{
		m_frustum.lock()->m_visualizeDepth = m_isTracking && m_visualizeDepth;
		m_frustumNear.lock()->m_visible = false;
		m_axes.lock()->m_visible = false;
	}
}

void SceneCamera::updateNearFrustumIndicator(const glm::mat4& model, const glm::mat4& projViewInv)
{
	auto frustumNear = m_frustumNear.lock();
	frustumNear->m_modelMatrix = glm::mat4(1.0f);

	// Construct a new mesh and replace the old one
	glm::vec3 worldCameraPos = m_modelMatrix[3];

	glm::vec4 ntl, ntr, nbr, nbl;
	if (m_coordinateSystem.clipRange == Core::ClipRange::MinusOneToOne)
	{
		ntl = glm::vec4(-1, m_coordinateSystem.yUp ? 1 : -1, -1, 1.f);
		ntr = glm::vec4(1, m_coordinateSystem.yUp ? 1 : -1, -1, 1.f);
		nbr = glm::vec4(1, m_coordinateSystem.yUp ? -1 : 1, -1, 1.f);
		nbl = glm::vec4(-1, m_coordinateSystem.yUp ? -1 : 1, -1, 1.f);
	}
	else if (m_coordinateSystem.clipRange == Core::ClipRange::ZeroToOne)
	{
		ntl = glm::vec4(-1, m_coordinateSystem.yUp ? 1 : -1, 0, 1.f);
		ntr = glm::vec4(1, m_coordinateSystem.yUp ? 1 : -1, 0, 1.f);
		nbr = glm::vec4(1, m_coordinateSystem.yUp ? -1 : 1, 0, 1.f);
		nbl = glm::vec4(-1, m_coordinateSystem.yUp ? -1 : 1, 0, 1.f);
	}
	else
	{
		throw std::runtime_error("Not implemented");
	}

	ntl = ProjectionUtils::divide(projViewInv * ntl);
	ntr = ProjectionUtils::divide(projViewInv * ntr);
	nbr = ProjectionUtils::divide(projViewInv * nbr);
	nbl = ProjectionUtils::divide(projViewInv * nbl);

	Shaper worldShaper;
	worldShaper.setColor(m_frustumOutlineColor);
	worldShaper.line(worldCameraPos, ntr);
	worldShaper.line(worldCameraPos, nbr);
	worldShaper.line(worldCameraPos, nbl);
	worldShaper.line(worldCameraPos, ntl);

	if (!m_frustumNearMesh)
		m_frustumNearMesh = std::shared_ptr<Core::Mesh>(worldShaper.createLineMesh("", Core::Mesh::STREAM));
	else
		worldShaper.updateLineMesh(m_frustumNearMesh.get());

	frustumNear->m_mesh = m_frustumNearMesh.get();

	if (m_isTracking)
	{
		glm::vec3 ndcCameraPos = m_trackedCameraModel.lock()->m_modelMatrix[3];

		auto trackedFrustumNear = m_trackedFrustumNear.lock();

		Shaper ndcShaper;
		ndcShaper.setColor(m_frustumOutlineColor);
		ndcShaper.line(ndcCameraPos, ProjectionUtils::divide(model * ntr));
		ndcShaper.line(ndcCameraPos, ProjectionUtils::divide(model * nbr));
		ndcShaper.line(ndcCameraPos, ProjectionUtils::divide(model * nbl));
		ndcShaper.line(ndcCameraPos, ProjectionUtils::divide(model * ntl));

		if (!m_trackedFrustumNearMesh)
			m_trackedFrustumNearMesh = std::shared_ptr<Core::Mesh>(ndcShaper.createLineMesh("", Core::Mesh::STREAM));
		else
			ndcShaper.updateLineMesh(m_trackedFrustumNearMesh.get());

		trackedFrustumNear->m_mesh = m_trackedFrustumNearMesh.get();
	}
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
	frustumFilled->m_opacity = m_frustumColor.w;
	frustumFilled->m_visible = m_fillFrustum;
	m_frustum = scene.addEntity(frustumFilled);

	{
		auto frustumNear = std::make_shared<ColoredObject>(nullptr, SHADERS.getShaderPtr<ColorShader>());
		frustumNear->setDisplayType(DisplayType::Frustum);
		frustumNear->m_opaque = false;
		frustumNear->m_opacity = m_frustumNearLinesOpacity;
		m_frustumNear = scene.addEntity(frustumNear);
	}

	{
		auto trackedFrustumNear = std::make_shared<ColoredObject>(nullptr, SHADERS.getShaderPtr<ColorShader>());
		trackedFrustumNear->setDisplayType(DisplayType::Tracking);
		trackedFrustumNear->m_ignoreReferenceSpace = true;
		trackedFrustumNear->m_opaque = false;
		trackedFrustumNear->m_opacity = m_frustumNearLinesOpacity;
		m_trackedFrustumNear = scene.addEntity(trackedFrustumNear);
	}

	{
		auto trackedCamera = std::make_shared<TexturedObject>(m_mesh, SHADERS.getShaderPtr<PhongShader>());
		trackedCamera->setDisplayType(DisplayType::Tracking);
		trackedCamera->m_ignoreReferenceSpace = true; // Tracked camera cannot be affected by the referenece space
		m_trackedCameraModel = scene.addEntity(trackedCamera);
	}

	updateNearFrustumIndicator(glm::mat4(1.0f), glm::mat4(1.0f));
}

void SceneCamera::onSceneRemove(Vp::Scene& scene)
{
	SceneModel::onSceneRemove(scene);
	scene.removeEntity(m_frustumOutline);
	scene.removeEntity(m_frustum);

	scene.removeEntity(m_frustumNear);

	scene.removeEntity(m_trackedFrustumNear);
	scene.removeEntity(m_trackedCameraModel);
}

void SceneCamera::setCoordinateSystem(Core::CameraCoordSystem& coordinateSystem)
{
	m_coordinateSystem = coordinateSystem;
	auto frustumPtr = m_frustum.lock();
	auto frustumOutlinePtr = m_frustumOutline.lock();

	if (coordinateSystem.clipRange == Core::ClipRange::MinusOneToOne)
	{
		frustumPtr->m_mesh = RMI.meshByAlias(Shaper::unitCube);
		frustumOutlinePtr->m_mesh = RMI.meshByAlias(Shaper::unitLineCube);
	}
	else if (coordinateSystem.clipRange == Core::ClipRange::ZeroToOne)
	{
		frustumPtr->m_mesh = RMI.meshByAlias(Shaper::vulkanNdcCube);
		frustumOutlinePtr->m_mesh = RMI.meshByAlias(Shaper::vulkanNdcLineCube);
	}
}
