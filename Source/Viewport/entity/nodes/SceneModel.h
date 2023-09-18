#pragma once

#include "Core/Nodes/Id.h"

#include "Viewport/entity/TexturedObject.h"

// For debug purposes, enables bounding box drawing
#define SHOW_BOUNDING_BOX 0

namespace Vp
{
class PhongShader;
class ColoredObject;

/**
 * Viewport entity representing a Model node in Core/GUI
 */
class SceneModel : public TexturedObject
{
public:
	Core::ID m_guiNodeId; ///< ID of the gui node object is representing

	bool m_showAxes{true};

	std::string m_modelAlias{};

	std::weak_ptr<ColoredObject> m_axes; ///< Visualisation of the basis vectors

#if SHOW_BOUNDING_BOX
	std::weak_ptr<ColoredObject> m_boundingBox; ///< For debug purposes
#endif

	SceneModel(Core::Mesh* mesh, PhongShader* shader);

	/**
	 * Load mesh using an alias.
	 */
	SceneModel(std::string modelAlias, PhongShader* shader);

	void setModel(std::string modelAlias);
	std::string getModel();

	void render(Shader* shader, glm::mat4 view, glm::mat4 projection, bool silhouette) override;
	void update(Scene& scene) override;

#if SHOW_BOUNDING_BOX
	void updateBoundingBox();
#endif

	void onSceneAdd(Scene& scene) override;
	void onSceneRemove(Scene& scene) override;
};
} // namespace Vp
