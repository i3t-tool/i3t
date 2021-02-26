#pragma once
#include "pgr.h"
#include "GameObject.h"
#include "../Rendering/FrameBuffer.h"

class GameObject;
class World2
{
public:
  World2();
  /// Load and compile shader program
  /**
    \param[in] vs_name Vertex shader file
    \param[in] fs_name Fragment shader file
    \return GL shader, 0 if failure
  */
  static GLuint loadShader(const char* vs_name,const char* fs_name);
  /// Initialize render
  /**
    Call before initMeshes.
    Loads textures, shaders, uniforms, initializes GL properties and functions
  */
  /// Initialize scene
  /**
    Creates and initializes default scene
  */
  static World2* loadDefaultScene();
  static World2* tmpAccess;
  /// calls start() on each component in scene
  /**
    called by initMeshes();
  */
  void onStart();
  /// Render scene
  /**
    Called by glut onDisplay event.
    Draws scene, updates global camera matricies
  */
  void onUpdate();
  GameObject* sceneRoot;///<root of scene of this world. Scene is a tree of GameObjects.


  static bool initRender();
  static float width;               ///< current viewport width
  static float height;              ///< current viewport width
  static glm::mat4x4 perspective; ///< Main camera perspective matrix, updated by Callbacks::onResize. Accessed by GameObject::draw(glm::mat4).
  static glm::mat4x4 mainCamera;  ///< Main camera transform, updated by Render::onDisplay. Accessed by GameObject::draw(glm::mat4).
  static glm::vec3 mainCamPos;    ///< Main camera position before rotation. Needed for reflection calculation in fs. Accessed by GameObject::draw(glm::mat4).

  static GLuint shader0; ///< Default shader
  static GLuint shaderTerrain; ///< Terrain shader
  static GLuint shaderSky;      ///< Skybox shader
  static GLuint shaderHandle;   ///< Handle shader
  static GLuint shaderTest;
  //static GLint shaderHandleColor;///<Handle shader color attribute
  static GLuint shaderProj; ///< preview projection matrices
private:
  static bool initializedRender; ///< Was render already initialized?
};