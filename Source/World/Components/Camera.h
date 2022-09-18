#pragma once
//----------------
/**
 * \file Camera.h
 * \author Daniel Gruncl
 * \brief renders scene
 * renders tree of GameObjects in update();
 * Can render in already prepared framebuffer (default configuration),
 * or into framebuffer provided by user.
 * Camera renders by calling render(glm::mat4*,bool) on each component on every
 * GameObject in scene tree (sceneRoot).
 *
 * Zatím cituji èeský popis od Daniela:
 * Co se týèe kamery, kamera nemá objekt (GameObject) jako komponentu. Kamera
 * sama je komponenta, ale je navržena pro dvojí úèel, èímž je její interface
 * trochu matoucí:
 *
 * 1. mód - Pohled do scény - kamera je komponentou gameobjectu, její
 * transformace (view) je inverzi transformace jejího gameobjectu. Projekèní
 * matice se nastavuje každý update() podle obrazovky a m_angle. V tomto módu
 * kamera renderuje na obrazovku.
 *
 * 2. mód - Preview nìjaké geometrie - kamera není pøipnuta k žádnému
 * gameobjectu, je k ní pøipnuta render textura. V tomto módu se projekèní
 * matice nastaví jednou v konstruktoru. Matice view je jednotková. Matici
 * projekce lze zmìnit pøímo dosazením, napø.:
 * camera->m_perspective=glm::ortho...
 *
 * Mód kamery se nastaví podle toho, který konstruktor je použit - konstruktor
 * bez rendertextury je první mód. V obou módech je potøeba dodat kameøe scénu,
 * která se bude renderovat, což je parametr sceneRoot. Gameobject musí mít
 * komponentu renderer, aby ho kamera vykreslila.
 *
 * Kamera má také parametr bool m_processView.
 * Pøi  nastavení na true kamera dodává do shaderu matici view invertovanou a
 * ortonormalizovanou, v opaèném pøípadì ji nijak nemìní.
 */
//---------------
#include "../Component.h"
#include "../GameObject.h"
#include "../RenderTexture.h"

class Camera : public Component
{
public:
	/// Create camera rendering on user-defined framebuffer - rendering into
	/// textures
	Camera(float viewingAngle, GameObject* sceneRoot,
	       RenderTexture* renderTarget);
	/// create camera rendering on default framebuffer
	Camera(float viewingAngle, GameObject* sceneRoot);
	//~Camera();
	void update();
	void start();
	static const char* componentType() { return Camera::s_type; };
	const char* getComponentType() { return Camera::s_type; };

	bool m_processView = true;
	float m_angle = 60.0f;
	RenderTexture* m_fbo = NULL; ///< if (fbo is NULL) then render on the screen,
	                             ///< assuming that the screen fbo is bounded
	glm::mat4x4 m_perspective;   ///< Perspective matrix
	GameObject* m_sceneRoot =
	    NULL; ///< this scene tree will be rendered by this camera

private:
	static const char* s_type;

	void renderRecursive(GameObject* obj, glm::mat4 parent,
	                     bool isTranspartentPass); ///< render scene tree
	bool m_mainCamera; ///< if fbo is NULL, camera renders to screen and sets
	                   ///< World camera settings to its own
};