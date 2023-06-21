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

#include "Core/Nodes/Id.h"
#include "Core/Nodes/Node.h"

#include "Viewport/Manipulators.h"
#include "Viewport/camera/AbstractCamera.h"
#include "Viewport/data/DisplayOptions.h"
#include "Viewport/data/RenderOptions.h"
#include "Viewport/data/ViewportSettings.h"
#include "Viewport/framebuffer/Framebuffer.h"
#include "Viewport/scene/Scene.h"
#include "Viewport/scene/SceneRenderTarget.h"
#include "Viewport/scene/scenes/MainScene.h"
#include "Viewport/scene/scenes/PreviewScene.h"
#include "Viewport/shader/Shaders.h"

namespace Vp
{
class Entity;
class GameObject;
class SceneModel;
class SceneCamera;
class AggregateCamera;

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

public: // TODO: (DR) Finish manipulator public API
	Ptr<Manipulators> m_manipulators;

private:
	// Scenes
	Ptr<MainScene> m_mainScene;
	Ptr<PreviewScene> m_previewScene;

	ViewportSettings m_settings;

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
	void init(ViewportSettings settings);

	/**
	 * Render viewport's main scene into a framebuffer using its own camera.
	 *
	 * @param renderTarget A reference to a shared pointer containing the desired render target.
	 * An empty pointer can be passed and it will be filled with an appropriate render target.
	 * @param width Framebuffer width in pixels
	 * @param height Framebuffer height in pixels
	 * @param renderOptions Optional rendering options. DON'T call this function multiple times with different
	 * renderOptions per frame.
	 * @param displayOptions Optional display options. These can change without restriction.
	 * @return Void. The drawn framebuffer can be retrieved with renderTarget->getOutputFramebuffer().
	 * Use outputFramebuffer.lock()->getColorTexture() to get the resulting texture.
	 */
	void drawViewport(Ptr<SceneRenderTarget>& renderTarget, int width, int height,
	                  const RenderOptions& renderOptions = RenderOptions(),
	                  const DisplayOptions& displayOptions = DisplayOptions());

	/**
	 * Render viewport's main scene into a framebuffer with the specified camera matrices.
	 *
	 * @param renderTarget A reference to a shared pointer containing the desired render target.
	 * An empty pointer can be passed and it will be filled with an appropriate render target.
	 * @param width Framebuffer width in pixels
	 * @param height Framebuffer height in pixels
	 * @param view View matrix of the camera
	 * @param projection Projection matrix of the camera
	 * @param renderOptions Optional rendering options. DON'T call this function multiple times with different
	 * renderOptions per frame.
	 * @param displayOptions Optional display options. These can change without restriction.
	 * @return Void. The drawn framebuffer can be retrieved with renderTarget->getOutputFramebuffer().
	 * Use outputFramebuffer.lock()->getColorTexture() to get the resulting texture.
	 */
	void drawScreen(Ptr<SceneRenderTarget>& renderTarget, int width, int height, glm::mat4 view, glm::mat4 projection,
	                const RenderOptions& renderOptions = RenderOptions(),
	                const DisplayOptions& displayOptions = DisplayOptions());

	/**
	 * Draw a preview of an object into a framebuffer.
	 *
	 * @param renderTarget A reference to a shared pointer containing the desired render target.
	 * An empty pointer can be passed and it will be filled with an appropriate render target.
	 * @param width Framebuffer width in pixels
	 * @param height Framebuffer height in pixels
	 * @param gameObject Game object to preview
	 * @param renderOptions Optional rendering options. DON'T call this function multiple times with different
	 * renderOptions per frame.
	 * @return Void. The drawn framebuffer can be retrieved with renderTarget->getOutputFramebuffer().
	 * Use outputFramebuffer.lock()->getColorTexture() to get the resulting texture.
	 */
	void drawPreview(Ptr<SceneRenderTarget>& renderTarget, int width, int height, WPtr<GameObject> gameObject,
	                 const RenderOptions& renderOptions);

	/**
	 * Update scene logic
	 */
	void update(double dt);

	// TODO: (DR) A little issue arises if this method were to be called multiple
	// 	times per frame. I'm not sure if that ever happens (multiple scene viewports of the same scene?).
	// 	But if so, then this method needs to be called only once per frame as not to speed up controls etc.
	// 	(Eg. mouse movement would be processed twice)
	/**
	 * Process viewport user input. Should be called before draw and only if the
	 * viewport is in focus. InputManager is used to handle input.
	 *
	 * Ensure this methods is only called once per frame.
	 *
	 * @param dt Time since last frame
	 * @param mousePos Current mouse position relative to the window
	 * @param windowSize Current window size
	 */
	void processInput(double dt, glm::vec2 mousePos, glm::ivec2 windowSize);

	void processSelection(Ptr<SceneRenderTarget> renderTarget, glm::vec2 mousePos, glm::ivec2 windowSize);

	/**
	 * Creates a SceneModel in the main scene to represent a Model node.
	 * Can be later removed with removeEntity()
	 * @param id Id of the gui node equivalent.
	 * @return Weak pointer to it.
	 */
	WPtr<SceneModel> createModel(Core::ID guiNodeId);

	/**
	 * Creates a SceneCamera in the main scene to represent a Camera node.
	 * Can be later removed with removeEntity()
	 * @param Id of the gui node equivalent.
	 * @return Weak pointer to it.
	 */
	WPtr<SceneCamera> createCamera(Core::ID guiNodeId);

	/**
	 * Remove an entity from the main scene.
	 */
	template <typename T, typename std::enable_if<std::is_base_of<Entity, T>::value, bool>::type = true>
	void removeEntity(WPtr<T>& entity)
	{
		m_mainScene->removeEntity(entity);
	}

	/**
	 * Returns the viewport camera of the main scene.
	 */
	WPtr<AggregateCamera> getMainViewportCamera();

	ViewportSettings& getSettings();

	WPtr<Scene> getMainScene()
	{
		return m_mainScene;
	};
	WPtr<Scene> getPreviewScene()
	{
		return m_previewScene;
	};

	Manipulators& getManipulators();
};
} // namespace Vp