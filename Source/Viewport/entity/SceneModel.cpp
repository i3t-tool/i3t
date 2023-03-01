#include "SceneModel.h"

#include <memory>

#include "Viewport/Shaper.h"
#include "Viewport/Viewport.h"
#include "Viewport/entity/ColoredObject.h"
#include "Viewport/scene/Scene.h"
#include "Viewport/shader/ColorShader.h"
#include "Viewport/shader/PhongShader.h"

#include "Core/Resources/ResourceManager.h"

using namespace Vp;

SceneModel::SceneModel(Core::Mesh* mesh, PhongShader* shader) : TexturedObject(mesh, shader)
{
	// Empty
}

SceneModel::SceneModel(std::string modelAlias, PhongShader* shader) : TexturedObject(nullptr, shader)
{
	setModel(modelAlias);
}

void SceneModel::setModel(std::string modelAlias)
{
	auto newMesh = RMI.meshByAlias(modelAlias);
	if (!newMesh)
	{
		LOG_ERROR("SceneModel: Model alias {} does not exist!", modelAlias);
		return;
	}
	m_modelAlias = modelAlias;
	m_mesh = newMesh;
}

std::string SceneModel::getModel() { return m_modelAlias; }

void SceneModel::update(Scene& scene)
{
	TexturedObject::update(scene);
	auto axesPtr = m_axes.lock();
	axesPtr->m_visible = m_showAxes;
	axesPtr->m_modelMatrix = this->m_modelMatrix * glm::scale(glm::mat4(1.0f), glm::vec3(2.0f));
}

void SceneModel::onSceneAdd(Scene& scene)
{
	TexturedObject::onSceneAdd(scene);
	auto axes = std::make_shared<ColoredObject>(RMI.meshByAlias(Shaper::xyzAxes),
	                                            scene.m_viewport->m_shaders->m_colorShader.get());
	axes->setDisplayType(DisplayType::Axes);
	m_axes = scene.addEntity(axes);
}

void SceneModel::onSceneRemove(Scene& scene)
{
	TexturedObject::onSceneRemove(scene);
	scene.removeEntity(m_axes);
}
