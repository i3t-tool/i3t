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
 * Zat�m cituji �esk� popis od Daniela:
 * Co se t��e kamery, kamera nem� objekt (GameObject) jako komponentu. Kamera
 * sama je komponenta, ale je navr�ena pro dvoj� ��el, ��m� je jej� interface
 * trochu matouc�:
 *
 * 1. m�d - Pohled do sc�ny - kamera je komponentou gameobjectu, jej�
 * transformace (view) je inverzi transformace jej�ho gameobjectu. Projek�n�
 * matice se nastavuje ka�d� update() podle obrazovky a m_angle. V tomto m�du
 * kamera renderuje na obrazovku.
 *
 * 2. m�d - Preview n�jak� geometrie - kamera nen� p�ipnuta k ��dn�mu
 * gameobjectu, je k n� p�ipnuta render textura. V tomto m�du se projek�n�
 * matice nastav� jednou v konstruktoru. Matice view je jednotkov�. Matici
 * projekce lze zm�nit p��mo dosazen�m, nap�.:
 * camera->m_perspective=glm::ortho...
 *
 * M�d kamery se nastav� podle toho, kter� konstruktor je pou�it - konstruktor
 * bez rendertextury je prvn� m�d. V obou m�dech je pot�eba dodat kame�e sc�nu,
 * kter� se bude renderovat, co� je parametr sceneRoot. Gameobject mus� m�t
 * komponentu renderer, aby ho kamera vykreslila.
 *
 * Kamera m� tak� parametr bool m_processView.
 * P�i  nastaven� na true kamera dod�v� do shaderu matici view invertovanou a
 * ortonormalizovanou, v opa�n�m p��pad� ji nijak nem�n�.
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