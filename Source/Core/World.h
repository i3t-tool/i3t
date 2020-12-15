/**
 *  \file world.h
 *
 *  Header for representing the 3D world (scene, camera, trackball control)
 *
 *  \date   2014/11/16
 *  \author Michal Folta, CTU Prague
 */
#pragma once

#include "InputController.h"
#include "Rendering/Scene.h"

class Shaper;
class Camera;
struct ObjectDefinitionTree;
class Transformation;

/**
 * \class	World
 *
 * \brief	Class for holding all the parts, such as a 3D scene, the workspace, the logic
 */

class World
{
private:
  bool cameraOrbitControl; ///< Rotation style: true - camera orbit control, false - cameraTrackball

  bool cameraIsControlled; ///< True after mouse click in 3D view, false after mouse release

public:
  glm::vec2 storeMouse; ///< store mouse coordinate before free look

  Keys::Code key_ToggleFreeLook;

  bool debugged; ///< True if line elements (coordinate frames, frustra) are drawn /todo rename

  Scene* scene; ///< The 3D scene preview (the upper window part)

  Shaper* worldFrameShaper; ///< The shaper (geometry and OpenGL buffers) for drawing the world axis
  Shaper* objectsShaper;    ///< The shaper (geometry and OpenGL buffers) for drawing other object axes and frustrum

  ObjectDefinitionTree*
      objectDefinitionTree; ///< The object menu tree (menu and submenus) \image html MenuObjects.png

  // Transformation * transformation;

  /**
   * \brief	Default constructor
   */
  World();

  /**
   * \brief	Destructor
   */
  ~World();

  /**
   * \brief	Callback - reshapes the scene (updates the perspective projection)
   *
   * \param	width 	The new width.
   * \param	height	The new height.
   */
  void onReshape(const int width, const int height) const
  {
    scene->onReshape(width, height); // updates the perspective projection
  }

  /**
   * \brief	get active camera
   *
   * \return	Null if it fails, else the active camera.
   */
  [[nodiscard]] Camera* getActiveCamera() const { return scene->camera; }

  /**
   * \brief	regenerate the scene
   */
  void reset();

  ////// ROTATION CONTROL
  /**
   * \brief	Sets camera control according to TabsConfig::CAMERA_ORBIT
   */
  void setConfigCameraControl();

  /**
   * \brief	Sets to orbit rotation control
   */
  void setToOrbitControl();

  /**
   * \brief	Sets to trackball rotation control
   */
  void setToTrackballControl();

  /**
   * \brief	Query if this object is camera control as orbit
   *
   * \return	True if camera control as orbit, false if not.
   */

  // bool isCameraControlAsOrbit() const
  //{
  //  return cameraOrbitControl;
  //}

  /**
   * \brief Update the world (in each step).
   *
   * Either update tabGroup events or perform camera interaction using
   * trackball or camera orbit rotation. Than updates the scene->keys
   * and finally the scene itself.
   */
  void update();

  /**
   * \brief	Render the scene and wire-frame objects (call debug()).
   */
  void render() const;

  /**
   * \brief Debug (=draw all reference coordinate frames and wire-frame objects) world
   * (called from render() in each timer step. Uses shaper to debug the scene.
   * \todo rename to something like drawWireframeObjects
   */
  void debug() const;

private:
  /**
   * \brief Construct the Scene object and set scene->renderGroups
   */
  void constructScene();
};
