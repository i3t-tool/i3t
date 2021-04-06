#pragma once
//----------------
/**
 * \file Camera2.h
 * \author Daniel Gruncl
 * \brief renders scene
 * renders tree of GameObjects in update();
 * Can render in already prepared framebuffer (default configuration),
 * or into framebuffer provided by user.
 * Camera renders by calling render(glm::mat4*,bool) on each component on every GameObject in scene tree.
 */
//---------------
#include "../GameObject.h"
#include "../Component.h"
#include "../RenderTexture.h"

class Camera2 : public Component{
public:
	///Create camera rendering on user-defined framebuffer - rendering into textures
	Camera2(float viewingAngle, GameObject* sceneRoot,RenderTexture*renderTarget);
	///create camera rendering on default framebuffer
	Camera2(float viewingAngle, GameObject* sceneRoot);
	//~Camera2();
	void update();
	void start();
	static const char* componentType() { return Camera2::s_type; };
	const char* getComponentType() { return Camera2::s_type; };

	float m_angle = 60.0f;
	RenderTexture* m_fbo=NULL;///<if fbo is NULL, then render on screen, assuming that screen fbo is binded
	glm::mat4x4 m_perspective; ///< Perspective matrix
	GameObject* m_sceneRoot = NULL;///< this scene tree will be rendered by camera

private:
  static const char* s_type;

  void renderRecursive(GameObject* obj, glm::mat4 parent,bool isTranspartentPass);///<render scene tree
  bool m_mainCamera;///<if fbo is NULL, camera renders to screen and sets World2 camera settings to its own
};