#include "AbstractCamera.h"

#include "glm/ext/matrix_clip_space.hpp"
#include "glm/gtx/matrix_interpolation.hpp"

#include "Core/Application.h"
#include "Core/Defs.h"

#include "GUI/Elements/Windows/WorkspaceWindow.h"

#include "Viewport/GfxUtils.h"
#include "Viewport/entity/nodes/SceneModel.h"
#include "Viewport/entity/nodes/SceneCamera.h"
#include "Viewport/scene/Scene.h"

using namespace Vp;
using namespace std::chrono;

void AbstractCamera::size(int width, int height)
{
	this->m_width = width;
	this->m_height = height;
}

glm::mat4 AbstractCamera::createProjectionMatrix(bool nonShrinking) const
{
	if (nonShrinking)
	{
		// Non shrinking resizing
		// Method 1
		float scale = m_zNear * tan(glm::radians(m_fov / 2));
		float l, r, b, t;
		r = t = scale;
		l = b = -scale;
		float aspect = std::min(m_width, m_height) / (float)std::max(m_width, m_height);
		if (m_width > m_height)
		{
			t *= aspect;
			b *= aspect;
		}
		else
		{
			l *= aspect;
			r *= aspect;
		}
		return glm::frustum(l, r, b, t, m_zNear, m_zFar);
		//		// Method 2
		//		if (width > height)
		//		{
		//			glm::mat4 projection = glm::perspective(glm::radians(fov), height / (float)width, zNear, zFar);
		//			float tmp = projection[0][0];
		//			projection[0][0] = projection[1][1];
		//			projection[1][1] = tmp;
		//			return projection;
		//		}
		//		else
		//		{
		//			return glm::perspective(glm::radians(fov), width / (float)height, zNear, zFar);
		//		}
	}
	else
	{
		return glm::perspective(glm::radians(m_fov), m_width / (float)m_height, m_zNear, m_zFar);
	}
}

void AbstractCamera::viewpoint(AbstractCamera::Viewpoint viewpoint) {}

void AbstractCamera::interpolate(glm::mat4 from, glm::mat4 to)
{
	// If already interpolating pickup from the current location
	float progress = 0;
	if (isInterpolating(progress))
	{
		interpolationFrom = glm::interpolate(interpolationFrom, interpolationTo, progress);
	}
	else
	{
		interpolationFrom = from;
	}
	interpolationTo = to;
	interpolationStart = std::chrono::steady_clock::now();
}

bool AbstractCamera::isInterpolating(float& progress) const
{
	double elapsed =
	    std::chrono::duration_cast<std::chrono::duration<double>>(steady_clock::now() - interpolationStart).count();
	if (elapsed < interpolationPeriod)
	{
		progress = static_cast<float>(elapsed / interpolationPeriod);
		return true;
	}
	return false;
}

void AbstractCamera::centerOnScene(const Scene& scene)
{
	int count = 0;
	std::vector<const GameObject*> objects;
	for (const auto& entity : scene.getEntities())
	{
		DisplayType type = entity->getDisplayType();
		if (type != DisplayType::Default && type != DisplayType::Camera)
		{
			continue;
		}
		// TODO: (DR) GameObject should probably be just turned into Entity, it really serves no purpose and some casts like
		//   this could be avoided
		if (auto gameObject = std::dynamic_pointer_cast<GameObject>(entity))
		{
			objects.push_back(gameObject.get());
			count++;
		}
	}
	if (count == 0)
	{
		centerOnBox({-1, -1, -1}, {1, 1, 1}, true); // Center on world origin
	}
	else
	{
		centerOnObjects(objects);
	}
}

void AbstractCamera::centerOnSelection(const Scene& scene)
{
	// TODO: (DR) FIX THIS, this is a quick workaround for the fact that viewport doesn't keep an updated list of all
	//  selected objects. This feature is important but I don't have time to refine it right now. Fix is to keep a list of
	//  selected objects in the scene which we have access to here.
	std::vector<const GameObject*> selectedObjects;
	for (const auto& modelNode : g_workspaceDiwne->getAllModels())
	{
		if (!modelNode->m_selected)
		{
			continue;
		}
		Ptr<SceneModel> model = modelNode->m_viewportModel.lock();
		DisplayType type = model->getDisplayType();
		if (type != DisplayType::Default && type != DisplayType::Camera)
		{
			continue;
		}
		selectedObjects.push_back(model.get());
	}
	for (const auto& cameraNode : g_workspaceDiwne->getAllCameras())
	{
		if (!cameraNode->m_selected)
		{
			continue;
		}
		Ptr<WorkspaceCamera> cameraPtr = std::static_pointer_cast<WorkspaceCamera>(cameraNode);
		Ptr<SceneCamera> camera = cameraPtr->m_viewportCamera.lock();
		DisplayType type = camera->getDisplayType();
		if (type != DisplayType::Default && type != DisplayType::Camera)
		{
			continue;
		}
		selectedObjects.push_back(camera.get());
	}
	centerOnObjects(selectedObjects);
}

void AbstractCamera::centerOnObjects(const std::vector<const GameObject*> objects)
{
	if (objects.empty())
		return;
	// Convert all bounding box points to world space and encompass them with one axis aligned bounding box
	// (Note: The w coordinate is ignored)
	std::vector<glm::vec3> points;
	for (const auto& object : objects)
	{
		std::vector<glm::vec3> entityPoints = createBoundingBoxWorldPoints(
		    object->m_mesh->m_boundingBoxMin, object->m_mesh->m_boundingBoxMax, object->m_modelMatrix);
		points.insert(points.end(), entityPoints.begin(), entityPoints.end());
	}

	// Create world space axis aligned bounding box containing all the points
	auto aaBox = GfxUtils::createBoundingBox(points);
	centerOnBox(aaBox.first, aaBox.second, true);
}

void AbstractCamera::centerOnBox(glm::vec3 boxMin, glm::vec3 boxMax, bool interpolate) {}

std::vector<glm::vec3> AbstractCamera::createBoundingBoxWorldPoints(glm::vec3 boxMin, glm::vec3 boxMax,
                                                                    glm::mat4 modelMatrix)
{
	// List of all points of the bounding box in world space
	std::vector<glm::vec3> points;
	points.push_back(glm::vec3(modelMatrix * glm::vec4(boxMin, 1.0f)));
	points.push_back(glm::vec3(modelMatrix * glm::vec4(boxMax, 1.0f)));
	points.push_back(glm::vec3(modelMatrix * glm::vec4(boxMin.x, boxMin.y, boxMax.z, 1.0f)));
	points.push_back(glm::vec3(modelMatrix * glm::vec4(boxMin.x, boxMax.y, boxMin.z, 1.0f)));
	points.push_back(glm::vec3(modelMatrix * glm::vec4(boxMax.x, boxMin.y, boxMin.z, 1.0f)));
	points.push_back(glm::vec3(modelMatrix * glm::vec4(boxMax.x, boxMax.y, boxMin.z, 1.0f)));
	points.push_back(glm::vec3(modelMatrix * glm::vec4(boxMax.x, boxMin.y, boxMax.z, 1.0f)));
	points.push_back(glm::vec3(modelMatrix * glm::vec4(boxMin.x, boxMax.y, boxMax.z, 1.0f)));
	return points;
}

glm::mat4 AbstractCamera::getView() const
{
	float progress = 0;
	if (isInterpolating(progress))
	{
		return glm::interpolate(interpolationFrom, interpolationTo, progress);
	}
	return m_view;
}

glm::mat4 AbstractCamera::getProjection() const { return m_projection; }

int AbstractCamera::getWidth() const { return m_width; }
int AbstractCamera::getHeight() const { return m_height; }

glm::vec3 AbstractCamera::getPosition() const { return m_position; }
glm::vec3 AbstractCamera::getDirection() const { return m_direction; }
glm::vec3 AbstractCamera::getUp() const { return m_up; }
glm::vec3 AbstractCamera::getRight() const { return m_right; }

float AbstractCamera::getZNear() const { return m_zNear; }
void AbstractCamera::setZNear(float zNear) { this->m_zNear = zNear; }
float AbstractCamera::getZFar() const { return m_zFar; }
void AbstractCamera::setZFar(float zFar) { this->m_zFar = zFar; }
float AbstractCamera::getFov() const { return m_fov; }
void AbstractCamera::setFov(float fov) { this->m_fov = fov; }
