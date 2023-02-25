/**
 * \file        Source/Viewport/Viewport.h
 * \author      Dan Rakušan <rakusdan(at)fit.cvut.cz>
 * \date        27.12.2022
 *
 * Main API class for the 3D viewport
 */

#pragma once

#include <memory>
#include <vector>

#include "pgr.h"

#include "Viewport/scene/DisplayOptions.h"
#include "Viewport/scene/MainScene.h"
#include "Viewport/scene/RenderOptions.h"
#include "Viewport/scene/Scene.h"
#include "Viewport/shader/Shaders.h"

namespace Vp
{
class Entity;
class GameObject;
class SceneModel;
class SceneCamera;

class ColorShader;
class PhongShader;
class GridShader;
class FrustumShader;
class WBOITCompositeShader;

/**
 * Scene viewport class.
 *
 * Has simple public interface to be used by GUI
 *
 * Manages "global" resources for drawing like shaders (via ResourceManager)
 *
 * It also currently manages two viewport Scenes
 * - Main scene
 * 		- Scene used for the main viewport and user interaction
 * 		- Used by drawViewport() and drawScreen().
 * - Preview scene
 * 		- Scene for small 3D model previews
 * 		- Used by drawPreview()
 *
 * Viewport provides three functions to draw:
 * - The main scene from a viewport camera to be used as the main viewport
 * - The main scene from an arbitrary camera specified by view and projection matrices
 * - A preview of a model
 *
 * These very specific functions are provided rather than a generic draw function to avoid managing framebuffer objects
 * outside this class.
 * The scenes are drawn into a SceneRenderTarget, which holds necessary framebuffers.
 * Each function uses a different SceneRenderTarget with potentially different render settings.
 * That means the main scene can be drawn with or without antialiasing for example. The settings are abstracted away
 * using RenderOptions.
 *
 * Further, DisplayOptions are used to toggle visibility of certain objects.
 */
class Viewport
{
private:
	friend class Scene;
	friend class MainScene;
	friend class SceneModel;
	friend class SceneCamera;

	// Scenes
	std::unique_ptr<MainScene> m_mainScene;
	std::unique_ptr<Scene> m_previewScene;

	// Scene render targets (buffers scenes are rendered into)
	Ptr<SceneRenderTarget> viewportRenderTarget;
	Ptr<SceneRenderTarget> screenRenderTarget;
	Ptr<SceneRenderTarget> previewRenderTarget;

	// Shaders
	// TODO: (DR) Should perhaps be a static or singleton class
	std::unique_ptr<Shaders> m_shaders;

public:
	/**
	 * Creates an empty viewport.
	 * Before any other calls it should be initialized using init()
	 */
	Viewport();
	~Viewport();

	/**
	 * Initializes scenes and loads assets.
	 */
	void init();

	/**
	 * Render viewport's main scene into a framebuffer using its own camera.
	 *
	 * @param width Framebuffer width in pixels
	 * @param height Framebuffer height in pixels
	 * @param renderOptions Optional rendering options. DON'T call this function multiple times with different
	 * renderOptions per frame.
	 * @param displayOptions Optional display options. These can change without restriction.
	 * @return Weak pointer to a Framebuffer object. Use getColorTexture() to get the resulting texture.
	 */
	WPtr<Framebuffer> drawViewport(int width, int height, const RenderOptions& renderOptions = RenderOptions(),
	                               const DisplayOptions& displayOptions = DisplayOptions());

	/**
	 * Render viewport's main scene into a framebuffer with the specified camera matrices.
	 *
	 * @param width Framebuffer width in pixels
	 * @param height Framebuffer height in pixels
	 * @param view View matrix of the camera
	 * @param projection Projection matrix of the camera
	 * @param renderOptions Optional rendering options. DON'T call this function multiple times with different
	 * renderOptions per frame.
	 * @param displayOptions Optional display options. These can change without restriction.
	 * @return Weak pointer to a Framebuffer object. Use getColorTexture() to get the resulting texture.
	 */
	WPtr<Framebuffer> drawScreen(int width, int height, glm::mat4 view, glm::mat4 projection,
	                             const RenderOptions& renderOptions = RenderOptions(),
	                             const DisplayOptions& displayOptions = DisplayOptions());

	/**
	 * Draw a preview of an object into a framebuffer.
	 *
	 * @param width Framebuffer width in pixels
	 * @param height Framebuffer height in pixels
	 * @param gameObject Game object to preview
	 * @return Weak pointer to a Framebuffer object. Use getColorTexture() to get the resulting texture.
	 */
	WPtr<Framebuffer> drawPreview(int width, int height, std::weak_ptr<GameObject> gameObject);

	/**
	 * Update scene logic
	 */
	void update();

	// TODO: (DR) A little issue arises if this method were to be called multiple
	// 	times per frame. I'm not sure if that ever happens (multiple scene viewports of the same scene?).
	// 	But if so, then this method needs to be called only once per frame as not
	// 	not speed up controls etc. 	(Eg. mouse movement would be processed twice)
	/**
	 * Process viewport user input. Should be called before draw and only if the
	 * viewport is in focus. InputManager is used to handle input.
	 *
	 * Ensure this methods is only called once per frame.
	 *
	 * @param mousePos Current mouse position relative to the window
	 * @param windowSize Current window size
	 */
	void processInput(glm::vec2 mousePos, glm::ivec2 windowSize);

	/**
	 * Creates a SceneModel in the main scene to represent a Model node.
	 * Can be later removed with removeEntity()
	 * @return Weak pointer to it.
	 */
	std::weak_ptr<SceneModel> createModel();

	/**
	 * Creates a SceneCamera in the main scene to represent a Camera node.
	 * Can be later removed with removeEntity()
	 * @return Weak pointer to it.
	 */
	std::weak_ptr<SceneCamera> createCamera();

	/**
	 * Remove an entity from the main scene.
	 */
	template <typename T, typename std::enable_if<std::is_base_of<Entity, T>::value, bool>::type = true>
	void removeEntity(std::weak_ptr<T>& entity)
	{
		m_mainScene->removeEntity(entity);
	}

private:
	void initPreviewScene(Scene& scene);
};
} // namespace Vp