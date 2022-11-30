#include "SceneCamera.h"

#include "Viewport/Shaper.h"
#include "Viewport/Viewport.h"
#include "Viewport/entity/FrustumObject.h"
#include "Viewport/shader/FrustumShader.h"

#include "Core/Resources/ResourceManager.h"
#include "Utils/Color.h"

using namespace Vp;

SceneCamera::SceneCamera(Core::Mesh* mesh, PhongShader* shader)
    : SceneModel(mesh, shader)
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
	frustumPtr->setColor(m_frustumOutlineColor);

	frustumPtr = m_frustum.lock();
	frustumPtr->m_visible = m_showFrustum && m_fillFrustum;
	frustumPtr->m_frustumProjectionMatrix = m_projectionMatrix;
	frustumPtr->m_frustumViewMatrix = m_viewMatrix;
	frustumPtr->setColor(m_frustumColor);

	SceneModel::update(scene);
}

void SceneCamera::render(glm::mat4 view, glm::mat4 projection)
{
	SceneModel::render(view, projection);
}

void SceneCamera::onSceneAdd(Vp::Scene& scene)
{
	SceneModel::onSceneAdd(scene);
	auto frustumPtr =
	    std::make_shared<FrustumObject>(RMI.meshByAlias(Shaper::unitCube),
	                                    scene.m_viewport->m_frustumShader.get());
	frustumPtr->setColor(m_frustumOutlineColor);
	m_frustumOutline = scene.addEntity(frustumPtr);

	auto frustumFilledPtr =
	    std::make_shared<FrustumObject>(RMI.meshByAlias(Shaper::unitCubeFilled),
	                                    scene.m_viewport->m_frustumShader.get());
	frustumFilledPtr->setColor(m_frustumColor);
	frustumFilledPtr->m_opaque = false;
	frustumFilledPtr->m_opacity = 0.22f;
	frustumFilledPtr->m_visible = m_fillFrustum;
	m_frustum = scene.addEntity(frustumFilledPtr);
}

void SceneCamera::onSceneRemove(Vp::Scene& scene)
{
	SceneModel::onSceneRemove(scene);
	scene.removeEntity(m_frustumOutline);
	scene.removeEntity(m_frustum);
}
