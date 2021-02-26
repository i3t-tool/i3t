#pragma once
#include "../GameObject.h"
#include "../Component.h"
#include "../RenderTexture.h"

class Camera2 : public Component{
public:
  Camera2(float viewingAngle, GameObject* sceneRoot,RenderTexture*renderTarget);
  Camera2(float viewingAngle, GameObject* sceneRoot);
  //~Camera2();
  void update();
  void start();
  static const char* componentType() { return Camera2::typeStatic; };
  const char* getComponentType() { return Camera2::typeStatic; };

  float angle = 60.0f;
  RenderTexture* fbo=NULL;///<if fbo is NULL, then render on screen, assuming that screen fbo is binded
  glm::mat4x4 perspective; ///< Perspective matrix
  GameObject* sceneRoot = NULL;///< this scene tree will be rendered by camera

private:
  static const char* typeStatic;

  void renderRecursive(GameObject* obj, glm::mat4 parent,bool isTranspartentPass);
  bool mainCamera;///<if fbo is NULL, camera renders to screen and sets World2 camera settings to its own
};