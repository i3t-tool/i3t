#include "World.h"

#include "Config.h"
#include "Core/Application.h"
#include "Core/Defs.h"
#include "Core/Input/InputManager.h"
#include "GUI/Settings.h"
#include "Logger/LoggerInternal.h"
#include "Rendering/AlphaShader.h"
#include "Rendering/BaseShader.h"
#include "Rendering/RedShader.h"
#include "Rendering/ShaderProvider.h"
#include "Rendering/Shaper.h"
#include "Rendering/ShaperElements.h"
#include "Utils/Color.h"
#include "Utils/TextureLoader.h"

World::World() : debugged(true), scene(NULL), cameraIsControlled(false), cameraOrbitControl(true)
{
	// OpFloatVal o(OpValuePlugType::INPUT);

	key_ToggleFreeLook = Keys::mouseRight;

	//----------------- World Coordinate axes  ---------------------------------
	worldFrameShaper = new Shaper(); // for World coordinate axes

	// static debug
	ShaperElements::setShaper(worldFrameShaper);                       // switch to this shaper
	ShaperElements::coordinateAxes(glm::vec3(0.0f, 0.0f, 0.0f), 1.0f); // prepare coordinates
	worldFrameShaper->rebuildVaos(); // create buffers with the coordinate axes frame, do not delete the prepared
	                                 // geometry (stored in vertices and colors)
	// ---------------------- ^^^^^^^-----------------

	// objects definitions
	// objectDefinitionTree = new ObjectDefinitionTree();

	// dynamic debug

	//----------------- All other wireframe geometry ---------------------------------
	// switch to dShaper
	objectsShaper = new Shaper();
	ShaperElements::setShaper(objectsShaper);

	// construct scene
	constructScene();
}

World::~World()
{
	// logic
	delete scene;

	// batchers
	delete objectsShaper;
	delete worldFrameShaper;
}

void World::reset()
{
	delete scene;
	constructScene();
}

void World::constructScene()
{
  // scene
  scene = new Scene(this);
  ShaderProvider::setRenderGroups(scene->renderGroups);


  ModelInstance*mi = new ModelInstance(GeometryMap::getHC("testCube"), MaterialMap::getHC("testMaterial"));
  //mi->getTrans();
  mi->scale(3.0f, 3.0f, 3.0f);
  //ShaderProvider::addToGroup(mi);
  //here construct scene
}

void World::setConfigCameraControl()
{
	if (Settings::CameraOrbit)
		setToOrbitControl();
	else
		setToTrackballControl();
}

void World::setToOrbitControl()
{
	scene->viewPort->setPosition(scene->camera->getPosition());
	scene->viewPort->setLookAt(scene->camera->getLookAt());
	scene->viewPort->setUp(0.0f, 1.0f, 0.0f);

	scene->camera->lerpTo(scene->viewPort, false);

	cameraOrbitControl = true;
}

void World::setToTrackballControl()
{
	cameraOrbitControl = false;
}

void World::update()
{
	// CAMERA ROTATION
	// orbit or trackball
	if (cameraOrbitControl)
	{
		if (InputManager::isKeyJustPressed(Settings::KeyWorld_mouseRotate)) // ORBIT camera rotation
		{
			cameraIsControlled = true;
			InputManager::beginCameraControl();
		}
		if (cameraIsControlled && InputManager::isKeyPressed(Settings::KeyWorld_mouseRotate))
		{
			scene->camera->orbit(scene->cameraOrbitCenter);
		}
		if (InputManager::isKeyJustUp(Settings::KeyWorld_mouseRotate))
		{
			cameraIsControlled = false;
			InputManager::endCameraControl();
		}
	}
	else // TRACKBALL camera rotation
	{
		if (InputManager::isKeyJustPressed(Settings::KeyWorld_mouseRotate))
		{
			cameraIsControlled = true;
			InputManager::beginCameraControl();
		}
		if (InputManager::isKeyPressed(Settings::KeyWorld_mouseRotate))
		{
			scene->camera->trackball(scene->cameraOrbitCenter, 2.0f);
		}
		if (InputManager::isKeyJustUp(Settings::KeyWorld_mouseRotate))
		{
			cameraIsControlled = false;
			InputManager::endCameraControl();
		}
	}

	// CAMERA PANNING - set a new orbit center
	if (InputManager::isKeyJustPressed(Keys::mouseMiddle))
	{
		cameraIsControlled = true;
		InputManager::beginCameraControl();
	}
	if (cameraIsControlled && InputManager::isKeyPressed(Settings::KeyWorld_mousePan))
	{
		scene->camera->moveOrbitCenter((float)-InputManager::m_mouseXDelta, (float)-InputManager::m_mouseYDelta,
		                               Config::CAM_MOTION_SENSITIVITY, scene->cameraOrbitCenter);
	}
	if (InputManager::isKeyJustUp(Settings::KeyWorld_mousePan))
	{
		cameraIsControlled = false;
		InputManager::endCameraControl();
	}

	// CAMERA ZOOM
	if (InputManager::isKeyJustUp(Keys::mouseScrlUp))
		scene->camera->pedal(Config::CAM_SCROLL_SENSITIVITY);
	if (InputManager::isKeyJustUp(Keys::mouseScrlDown))
		scene->camera->pedal(-Config::CAM_SCROLL_SENSITIVITY);

	/*
	// freeLook
	if (InputController::isKeyJustPressed(key_ToggleFreeLook)) {
	  InputController::setMouseVisible(false);
	  //storeMouse = glm::vec2(InputController::mouseX, InputController::mouseY);
	  InputController::centerMouse();
	}

	// freeLook
	if (InputController::isKeyPressed(key_ToggleFreeLook)) camera->freeLook();
	*/

	// -----------------------------------------------------------------------------------------
	/*
	// start freeLook
	if (InputController::isKeyJustPressed(key_ToggleFreeLook)) {
	  InputController::setMouseVisible(false);
	  //storeMouse = glm::vec2(InputController::mouseX, InputController::mouseY);
	  InputController::centerMouse();
	}

	// procede freeLook
	if (InputController::isKeyPressed(key_ToggleFreeLook)) scene->camera->freeLook();

	// end freeLook
	if (InputController::isKeyJustUp(key_ToggleFreeLook)) {
	  //InputController::setMousePosition(storeMouse.x, storeMouse.y);
	  InputController::setMouseVisible(true);
	}
	*/

	scene->update();

	UPDATE_LOGGER();

	// scene->camera->WSAD(Keys::w, Keys::s, Keys::a, Keys::d, Config::CAM_MOTION_SENSITIVITY);

	/*
	for (int i=0; i<TabSpace::transformationSpaceForm->transformationForms.size(); i++) {
	  scene->objects[i]->setTrans(TabSpace::transformationSpaceForm->transformationForms[i]->getTransformation()->getOut());
	}
	*/

	// cout << scene->camera->getPosition().x << ", " << scene->camera->getPosition().y << ", " <<
	// scene->camera->getPosition().z << std::endl;
}

void World::render() const
{
	scene->render(getActiveCamera()); // 3D scene models
	debug();                          // 3D scene wire-frame objects (axes, frustrum,...)
																		// scene->renderAlpha(getActiveCamera());
}

void World::debug() const
{
	if (!debugged)
		return;

	// PF ShaperElements::setShaper(dShaper); // set to active \todo kill - it is reset in each object  in
	// scene->debug()

	// PFXX
	// PF ShaperElements::setShaper(dShaper); // was set twice???? \todo kill - it is reset in each object in
	// scene->debug()
	scene->debug(objectsShaper);

    objectsShaper->drawOnceAndDestroy(getActiveCamera()); // move prepared geometry to buffers, draw them, and destroy afterwards

	worldFrameShaper->drawAndPreserve(getActiveCamera()); // world axes: only draw
}
