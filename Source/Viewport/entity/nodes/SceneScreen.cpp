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
#include "SceneScreen.h"

#include "Core/Nodes/GraphManager.h"
#include "Core/Resources/ResourceManager.h"
#include "Viewport/scene/Scene.h"
#include "Viewport/shader/PhongShader.h"
#include "Viewport/shader/Shaders.h"

namespace Vp
{

SceneScreen::SceneScreen()
    : TexturedObject(RMI.mesh("Data/Models/monitor-frame.glb"), SHADERS.getShaderPtr<PhongShader>())
{
	setDisplayType(DisplayType::Tracking);
	m_ignoreReferenceSpace = true;
	m_ignoreUserClipping = true;
	m_backFaceCull = true;
	m_selectable = true;
	m_zOffset = Math::randomf() * 0.01f;
}

void SceneScreen::updateModelTransform(float width, float height, float scaleFactor, bool yUp)
{
	float xScale = -width / scaleFactor;
	float yScale = height / scaleFactor;
	float zScale = xScale;
	m_modelMatrix = glm::mat4(1.0f);
	if (!yUp)
		m_modelMatrix = glm::translate(m_modelMatrix, glm::vec3(0.f, -height / scaleFactor, 0.f));
	m_modelMatrix = glm::translate(m_modelMatrix, glm::vec3(0.f, 0.0f, -m_zOffset));
	m_modelMatrix = glm::scale(m_modelMatrix, glm::vec3(xScale, yScale, zScale));

	auto basePtr = m_screenBase.lock();
	basePtr->m_modelMatrix = m_modelMatrix;
	xScale = -std::max(abs(xScale), abs(yScale));
	yScale = -xScale;
	zScale = xScale;
	basePtr->m_modelMatrix = glm::mat4(1.0f);
	if (!yUp)
		basePtr->m_modelMatrix = glm::translate(basePtr->m_modelMatrix, glm::vec3(0.f, -height / scaleFactor, 0.f));
	basePtr->m_modelMatrix =
	    glm::translate(basePtr->m_modelMatrix, glm::vec3(-(width / 2.f) / scaleFactor, 0.0f, -m_zOffset));
	basePtr->m_modelMatrix = glm::scale(basePtr->m_modelMatrix, glm::vec3(xScale, yScale, zScale));
}

void SceneScreen::update(Scene& scene)
{
	auto basePtr = m_screenBase.lock();
	basePtr->m_visible = m_visible;
	basePtr->m_opaque = m_opaque;
	basePtr->m_opacity = m_opacity;

	TexturedObject::update(scene);
}

void SceneScreen::onSceneAdd(Vp::Scene& scene)
{
	TexturedObject::onSceneAdd(scene);
	auto base =
	    std::make_shared<TexturedObject>(RMI.mesh("Data/Models/monitor-base.glb"), SHADERS.getShaderPtr<PhongShader>());
	base->m_visible = false;
	base->setDisplayType(DisplayType::Tracking);
	base->m_ignoreReferenceSpace = true;
	base->m_ignoreUserClipping = true;
	m_screenBase = scene.addEntity(base);
}

void SceneScreen::onSceneRemove(Vp::Scene& scene)
{
	TexturedObject::onSceneRemove(scene);
	scene.removeEntity(m_screenBase);
}
} // namespace Vp