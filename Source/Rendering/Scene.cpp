#include "Scene.h"

#include <GUI/Settings.h>
#include <vector>

#include "Core/World.h"

#include "Rendering/Camera.h"
#include "Utils/TMshReader.h"
#include "Utils/TextureLoader.h"

#include "Rendering/Camera.h"

#include "Rendering/CameraObject.h"
#include "Rendering/Object.h"

#include "Rendering/AlphaShader.h"
#include "Rendering/BaseShader.h"

#include "Rendering/ShaderProvider.h"

#include "Environment.h"
#include "Geometry.h"
#include "Rendering/Material.h"
#include "Rendering/ShaperElements.h"
#include "Transformation.h"

Scene::Scene(World* _world) : world(_world)
{
	// camera
	camera = new Camera();
	camera->setPosition(2.0f, 2.0f, 2.0f);
	camera->setLookAt(0.0f, 0.0f, 0.0f);

	camera->setPerspective(static_cast<float>(Config::WIN_WIDTH), static_cast<float>(Config::WIN_HEIGHT));
	camera->update();

	viewPort = new ViewPort(glm::vec3(0.0f, 0.0f, -10.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));

	cameraOrbitCenter = glm::vec3();

	environment = new Environment();

	frustrumObject = new ModelInstance(GeometryMap::getHC("frustrum"), MaterialMap::getHC("frustrum"));

	renderGroups = new RenderGroups();
};

Scene::~Scene()
{
	// clear object array
	for (std::vector<Object*>::iterator it = objects.begin(); it != objects.end(); ++it)
	{
		delete (*it);
	}

	// rest
	delete camera;
	delete environment;
	delete frustrumObject;
	delete renderGroups;
}

void Scene::onReshape(int width, int height)
{
	camera->setPerspective(float(width), float(height));
	// camera->update();
}

Object* Scene::addObject(Transformation* t, ObjectDefinition* o)
{
	// Object * o = new Object(GeometryMap::get("testCube"), MaterialMap::get("testMaterial"), t);
	Object* obj = o->getInstance(t);
	ShaderProvider::addToGroup(obj);
	objects.push_back(obj);
	return obj;
}

void Scene::removeObject(Object* object)
{
	ShaderProvider::removeFromGroup(object);

	for (std::vector<Object*>::iterator iter = objects.begin(); iter != objects.end(); ++iter)
	{
		if (*iter == object)
		{
			objects.erase(iter);
			break;
		}
	}
	delete object;
}

void Scene::removeCameraObject(CameraObject* co)
{
	for (std::vector<CameraObject*>::iterator iter = cameraObjects.begin(); iter != cameraObjects.end(); ++iter)
	{
		if (*iter == co)
		{
			cameraObjects.erase(iter);
			break;
		}
	}
	removeObject(co);
}

// PF2018-03-05
void printMatrix(glm::mat4 m)
{
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			std::cout << m[j][i] << ", ";
		}
		std::cout << std::endl;
	}
}

CameraObject* Scene::addCamera(Transformation* p, Transformation* v)
{
	// PFF Tady se prida material, ktery patri shaderu base (8attr), ale ma texturu 0 d�ve -1
	CameraObject* c = new CameraObject(GeometryMap::getHC("camera"), MaterialMap::getHC("camera"), v, p);
	ShaderProvider::addToGroup(c);

	// PF2018-03-05
	// printMatrix(c->getTrans());

	objects.push_back(c);
	cameraObjects.push_back(c);
	return c;
}

/**
 * \brief Update the camera and all objects
 */
void Scene::update()
{
	camera->update();

	for (unsigned int i = 0; i < objects.size(); i++)
	{
		objects[i]->update();
	}
}

/**
 * \brief Updates camera position and
 */
void Scene::keyUpdate()
{
	// view ports
	if (camera == nullptr)
		return;
	if (!camera->isLerping())
	{
		if (InputManager::isKeyJustPressed(Settings::KeyScene_camTo_orbitX))
			setCamToOrbitCenterX();
		else if (InputManager::isKeyJustPressed(Settings::KeyScene_camTo_orbitY))
			setCamToOrbitCenterY();
		else if (InputManager::isKeyJustPressed(Settings::KeyScene_camTo_orbitZ))
			setCamToOrbitCenterZ();
		else if (InputManager::isKeyJustPressed(Settings::KeyScene_camTo_worldX))
			setCamToCenterX();
		else if (InputManager::isKeyJustPressed(Settings::KeyScene_camTo_worldY))
			setCamToCenterY();
		else if (InputManager::isKeyJustPressed(Settings::KeyScene_camTo_worldZ))
			setCamToCenterZ();
		else if (InputManager::isKeyJustPressed(Settings::KeyScene_camTo_scene))
			setCamToCenter();
		else if (InputManager::isKeyJustPressed(Settings::Key_undo))
		{
			/// \todo Scene undo.
			// UndoRedo::undo();
		}
		else if (InputManager::isKeyJustPressed(Settings::Key_redo))
		{
			/// \todo Scene redo.
			// UndoRedo::redo();
		}
	}
}

void Scene::setCamToOrbitCenterX() const
{
	const float dist = glm::length(camera->getPosition() - cameraOrbitCenter);
	const glm::vec3 pos = cameraOrbitCenter + glm::vec3(1.0f, 0.0f, 0.0f) * dist;
	viewPort->setPosition(pos);
	viewPort->setLookAt(cameraOrbitCenter);
	camera->lerpTo(viewPort, false);
}

void Scene::setCamToOrbitCenterY()
{
	const float dist = glm::length(camera->getPosition() - cameraOrbitCenter);
	const glm::vec3 pos = cameraOrbitCenter + glm::vec3(0.0f, 1.0f, 0.001f) * dist;
	viewPort->setPosition(pos);
	viewPort->setLookAt(cameraOrbitCenter);
	camera->lerpTo(viewPort, false);
}

void Scene::setCamToOrbitCenterZ()
{
	const float dist = glm::length(camera->getPosition() - cameraOrbitCenter);
	const glm::vec3 pos = cameraOrbitCenter + glm::vec3(0.0f, 0.0f, 1.0f) * dist;
	viewPort->setPosition(pos);
	viewPort->setLookAt(cameraOrbitCenter);
	camera->lerpTo(viewPort, false);
}

void Scene::setCamToCenterX()
{
	cameraOrbitCenter = glm::vec3(0.0f, 0.0f, 0.0f);
	const float dist = glm::length(camera->getPosition() - cameraOrbitCenter);
	const glm::vec3 pos = cameraOrbitCenter + glm::vec3(1.0f, 0.0f, 0.0f) * dist;
	viewPort->setPosition(pos);
	viewPort->setLookAt(cameraOrbitCenter);
	camera->lerpTo(viewPort, false);
}

void Scene::setCamToCenterY()
{
	cameraOrbitCenter = glm::vec3(0.0f, 0.0f, 0.0f);
	const float dist = glm::length(camera->getPosition() - cameraOrbitCenter);
	glm::vec3 pos = cameraOrbitCenter + glm::vec3(0.0f, 1.0f, 0.001f) * dist;
	viewPort->setPosition(pos);
	viewPort->setLookAt(cameraOrbitCenter);
	camera->lerpTo(viewPort, false);
}

void Scene::setCamToCenterZ()
{
	cameraOrbitCenter = glm::vec3(0.0f, 0.0f, 0.0f);
	const float dist = glm::length(camera->getPosition() - cameraOrbitCenter);
	const glm::vec3 pos = cameraOrbitCenter + glm::vec3(0.0f, 0.0f, 1.0f) * dist;
	viewPort->setPosition(pos);
	viewPort->setLookAt(cameraOrbitCenter);
	camera->lerpTo(viewPort, false);
}

void Scene::setCamToCenter()
{
	if (objects.empty())
	{
		setCamToCenterZ();
		return;
	}

	glm::vec3 min = glm::vec3(FLT_MAX, FLT_MAX, FLT_MAX);
	glm::vec3 max = glm::vec3(-FLT_MAX, -FLT_MAX, -FLT_MAX);

	glm::mat4 tr;
	glm::vec4 v;
	float* vb;
	unsigned int vsize;

	for (unsigned int i = 0; i < objects.size(); i++)
	{
		tr = camera->getView() * objects[i]->getTrans();
		vb = objects[i]->getGeometry()->getVertices();
		vsize = objects[i]->getGeometry()->getVerticesCount() * objects[i]->getGeometry()->getAttribsPerVertex();

		for (unsigned int k = 0; k < vsize; k += objects[i]->getGeometry()->getAttribsPerVertex())
		{
			v = tr * glm::vec4(vb[k], vb[k + 1], vb[k + 2], 1.0f);

			if (v.x < min.x)
				min.x = v.x;
			if (v.x > max.x)
				max.x = v.x;

			if (v.y < min.y)
				min.y = v.y;
			if (v.y > max.y)
				max.y = v.y;

			if (v.z < min.z)
				min.z = v.z;
			if (v.z > max.z)
				max.z = v.z;
		}
	}

	glm::vec3 nc = glm::vec3(glm::inverse(camera->getView()) * glm::vec4((max + min) * 0.5f, 1.0f));
	glm::vec3 d = camera->getDirection();

	viewPort->setLookAt(nc);
	cameraOrbitCenter = nc;

	float dist = glm::max(abs((max.y - min.y) * 0.5f), abs((max.y - min.y) * 0.5f)) / tan(camera->getFov() * 0.5f);
	viewPort->setPosition(nc - d * dist * 1.5f);

	camera->lerpTo(viewPort, false);
}

void Scene::render(Camera* _camera)
{
	ShaderProvider::renderAll(_camera, environment);

	ShaderProvider::getAlphaShader()->begin();
	for (unsigned int i = 0; i < cameraObjects.size(); i++)
	{
		if (cameraObjects[i]->getDrawFrustum())
		{
			frustrumObject->setTrans(
					glm::inverse(cameraObjects[i]->getProjection() * glm::inverse(cameraObjects[i]->getTrans())));
			ShaderProvider::getAlphaShader()->draw(frustrumObject, _camera, environment);
			/// should be drawn as a hcGeometries[2]
		}
	}
	ShaderProvider::getAlphaShader()->end();
}

void Scene::render(const glm::mat4 proj, const glm::mat4 view, CameraObject* noCamera) const
{
	ShaderProvider::renderAll(proj, view, environment, noCamera);
}

/*
void Scene::renderAlpha(Camera * _camera) {

  // alpha shader group
  world->alphaShader->begin();
  for (int i=0; i<cameraObjects.size(); i++) {
    frustrumObject->setTrans(glm::inverse(cameraObjects[i]->getProjection() *
glm::inverse(cameraObjects[i]->getTrans()))); world->alphaShader->draw(frustrumObject, _camera, environment);
  }
  world->alphaShader->end();
}
*/

void Scene::drawCamera()
{
}

void Scene::debug(Shaper* shaper)
{
	for (unsigned int i = 0; i < objects.size(); i++)
		objects[i]->debug(shaper);

	// ShapersElements::getShaper()->setColor(Color::RED);
	// ShapersElements::gridXY(glm::vec2(), 1, 10);
}
