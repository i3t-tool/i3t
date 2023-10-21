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
#include "SceneModel.h"

#include <memory>

#include "Core/Resources/ResourceManager.h"

#include "Utils/Format.h"

#include "Viewport/GfxUtils.h"
#include "Viewport/Shaper.h"
#include "Viewport/Viewport.h"
#include "Viewport/entity/ColoredObject.h"
#include "Viewport/scene/Scene.h"
#include "Viewport/shader/ColorShader.h"
#include "Viewport/shader/PhongShader.h"

using namespace Vp;

SceneModel::SceneModel(Core::Mesh* mesh, PhongShader* shader) : TexturedObject(mesh, shader)
{
	m_selectable = true;
	m_backFaceCull = true;
}

SceneModel::SceneModel(std::string modelAlias, PhongShader* shader) : SceneModel(nullptr, shader)
{
	setModel(modelAlias);
}

void SceneModel::setModel(std::string modelAlias)
{
	auto newMesh = RMI.meshByAlias(modelAlias);
	if (!newMesh)
	{
		LOG_ERROR("SceneModel: Model alias '{}' does not exist!", modelAlias);
		return;
	}
	m_modelAlias = modelAlias;
	m_mesh = newMesh;
#if SHOW_BOUNDING_BOX
	updateBoundingBox();
#endif
}

std::string SceneModel::getModel()
{
	return m_modelAlias;
}

void SceneModel::render(Shader* shader, glm::mat4 view, glm::mat4 projection, bool silhouette)
{
	// Ensure the model we're using hasn't been removed from the scene
	if (!RMI.resourceExists(this->m_mesh->m_resourceId))
	{
		// Revert to default cube, assuming its aliased as "Cube"
		m_mesh = RMI.meshByAlias("Cube");
	}

	TexturedObject::render(shader, view, projection, silhouette);
}


void SceneModel::update(Scene& scene)
{
	TexturedObject::update(scene);
	auto axesPtr = m_axes.lock();
	axesPtr->m_visible = m_showAxes;
	axesPtr->m_modelMatrix = this->m_modelMatrix * glm::scale(glm::mat4(1.0f), glm::vec3(2.0f));

#if SHOW_BOUNDING_BOX
	auto boundingBoxPtr = m_boundingBox.lock();
	boundingBoxPtr->m_modelMatrix = glm::mat4(1.0f);
	updateBoundingBox();
#endif
}

void SceneModel::onSceneAdd(Scene& scene)
{
	TexturedObject::onSceneAdd(scene);
	auto axes =
	    std::make_shared<ColoredObject>(RMI.meshByAlias(Shaper::xyzAxes), Shaders::instance().m_colorShader.get());
	axes->setDisplayType(DisplayType::Axes);
	m_axes = scene.addEntity(axes);

#if SHOW_BOUNDING_BOX
	auto boundingBox = std::make_shared<ColoredObject>(nullptr, Shaders::instance().m_colorShader.get());
	boundingBox->setDisplayType(DisplayType::Axes);
	m_boundingBox = scene.addEntity(boundingBox);
	updateBoundingBox();
#endif
}

#if SHOW_BOUNDING_BOX
// Was used for testing, can be removed
void SceneModel::updateBoundingBox()
{
	if (auto box = m_boundingBox.lock())
	{
		std::vector<glm::vec3> points;
		glm::vec3 boxMin = m_mesh->m_boundingBoxMin;
		glm::vec3 boxMax = m_mesh->m_boundingBoxMax;

		// Convert all bounding box points to world space and encompass them with one axis aligned bounding box
		// (Note: The w coordinate is ignored)

		// List of all points of the bounding box in world space
		points.push_back(glm::vec3(this->m_modelMatrix * glm::vec4(boxMin, 1.0f)));
		points.push_back(glm::vec3(this->m_modelMatrix * glm::vec4(boxMax, 1.0f)));
		points.push_back(glm::vec3(this->m_modelMatrix * glm::vec4(boxMin.x, boxMin.y, boxMax.z, 1.0f)));
		points.push_back(glm::vec3(this->m_modelMatrix * glm::vec4(boxMin.x, boxMax.y, boxMin.z, 1.0f)));
		points.push_back(glm::vec3(this->m_modelMatrix * glm::vec4(boxMax.x, boxMin.y, boxMin.z, 1.0f)));
		points.push_back(glm::vec3(this->m_modelMatrix * glm::vec4(boxMax.x, boxMax.y, boxMin.z, 1.0f)));
		points.push_back(glm::vec3(this->m_modelMatrix * glm::vec4(boxMax.x, boxMin.y, boxMax.z, 1.0f)));
		points.push_back(glm::vec3(this->m_modelMatrix * glm::vec4(boxMin.x, boxMax.y, boxMax.z, 1.0f)));
		// Create world space axis aligned bounding box containing the original bounding box
		auto aaBox = GfxUtils::createBoundingBox(points);

		Shaper shaper;
		shaper.setColor(glm::vec3(1.0, 0.0, 1.0));
		shaper.lineBox(aaBox.first, aaBox.second);
		box->m_mesh = shaper.createLineMesh();
	}
}
#endif

void SceneModel::onSceneRemove(Scene& scene)
{
	TexturedObject::onSceneRemove(scene);
	scene.removeEntity(m_axes);

#if SHOW_BOUNDING_BOX
	scene.removeEntity(m_boundingBox);
#endif
}
