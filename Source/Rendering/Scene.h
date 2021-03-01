//---------------------------------------------------------------------------
/**
 * \file	scene.h
 *
 * Declares the scene class
 *
 * \date  	2014/11/16
 * \author 	Michal Folta, CTU Prague
 */
//---------------------------------------------------------------------------

#ifndef _SCENE_H_
#define _SCENE_H_

class Transformation;
class ModelInstance;

// class Object;
// class CameraObject;

#include "Rendering/CameraObject.h"
#include "Rendering/Object.h"

struct RenderGroups;
class Shaper;

class World;

struct Environment;

class Camera;
class ViewPort;

#include <vector>

class Scene
{
public:
	World* world;

	Environment* environment;

	/// camera objects
	std::vector<CameraObject*> cameraObjects;

	/// all objects (render groups, cameras??, free objects ...)
	std::vector<Object*> objects;

	/// render groups
	RenderGroups* renderGroups;

	/// \todo TOFIX get out of scene - collision with 	GeometryMap::connectAllHCToShader() in main.cpp::init()
	/// frustrum model to draw all cameras frustra
	ModelInstance* frustrumObject;

	/// global camera
	Camera* camera; ///< the world camera - probably the camera for the 3D scene preview
	glm::vec3 cameraOrbitCenter;

	ViewPort* viewPort;

	Scene(World* _world);
	~Scene();

	void onReshape(int width, int height);

	Object* addObject(Transformation* t, ObjectDefinition* o);

	void removeObject(Object* object);
	void removeCameraObject(CameraObject* co);

	CameraObject* addCamera(Transformation* p, Transformation* v);

	void update();
	void keyUpdate();

	void render(Camera* _camera);
	void render(glm::mat4 proj, glm::mat4 view, CameraObject* noCamera) const;
	// void renderAlpha(Camera * _camera);

	/**
	 * \brief \notused drawCamera() in never used
	 */
	void drawCamera();

	void debug(Shaper* shaper);

	void setCamToOrbitCenterX() const;
	void setCamToOrbitCenterY();
	void setCamToOrbitCenterZ();

	void setCamToCenterX();
	void setCamToCenterY();
	void setCamToCenterZ();

	void setCamToCenter();
};

#endif
