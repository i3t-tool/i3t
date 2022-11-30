#pragma once

#include <memory>
#include <vector>

#include "pgr.h"

#include "Viewport/scene/DisplayOptions.h"
#include "Viewport/scene/MainScene.h"
#include "Viewport/scene/Scene.h"

namespace Vp
{
class ColorShader;
class PhongShader;
class GridShader;
class FrustumShader;
class Entity;
class GameObject;
class SceneModel;
class SceneCamera;

/**
 * Scene viewport class.
 *
 * Has simple public interface to be used by GUI
 *
 * Under the hood:
 * Manages "global" resources for drawing like shaders (via ResourceManager)
 *
 * Currently it manages two viewport Scenes
 * - Main scene
 * 		- Scene used for the main viewport and user interaction
 * 		- Used by draw() methods.
 * - Preview scene
 * 		- Scene for small 3D model previews
 * 		- Used by drawPreview()
 *
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

	// Shaders
	std::unique_ptr<PhongShader> m_phongShader;
	std::unique_ptr<ColorShader> m_colorShader;
	std::unique_ptr<GridShader> m_gridShader;
	std::unique_ptr<FrustumShader> m_frustumShader;

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
	 * Render viewport's main scene using its own camera.
	 */
	void draw(int width, int height);

	/**
	 * Render viewport's main scene with the specified camera matrices.
	 */
	void draw(glm::mat4 view, glm::mat4 projection,
	          const DisplayOptions& displayOptions = DisplayOptions());

	/**
	 * Draw a preview of an object.
	 * @param gameObject Game object to preview
	 */
	void drawPreview(std::weak_ptr<GameObject> gameObject, int width, int height);

	void update();

	// TODO: (DR) A little issue arises if this method were to be called multiple
	// times per frame. 	I'm not sure if that ever happens (multiple scene viewports
	//of the same scene?).
	// 	But if so, then this method needs to be called only once per frame as not
	// not speed up controls etc. 	(Eg. mouse movement would be processed twice)
	/**
	 * Process viewport user input. Should be called before draw and only if the
	 * viewport is in focus. InputManager is used to handle input.
	 */
	void processInput();

	/**
	 * Creates a SceneModel in the main scene to represent a Model node.
	 * @return Weak pointer to it.
	 */
	std::weak_ptr<SceneModel> createModel();

	std::weak_ptr<SceneCamera> createCamera();

	template <typename T,
	          typename std::enable_if<std::is_base_of<Entity, T>::value,
	                                  bool>::type = true>
	void removeEntity(std::weak_ptr<T>& entity)
	{
		m_mainScene->removeEntity(entity);
	}

private:
	void initMainScene(Scene& scene);
	void initPreviewScene(Scene& scene);
};
} // namespace Vp