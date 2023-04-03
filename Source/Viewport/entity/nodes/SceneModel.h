#pragma once

#include "Core/Nodes/Id.h"

#include "Viewport/entity/TexturedObject.h"

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

	std::weak_ptr<ColoredObject> m_axes;

	SceneModel(Core::Mesh* mesh, PhongShader* shader);

	/**
	 * Load mesh using an alias.
	 */
	SceneModel(std::string modelAlias, PhongShader* shader);

	void setModel(std::string modelAlias);
	std::string getModel();

	void update(Scene& scene) override;

	void onSceneAdd(Scene& scene) override;
	void onSceneRemove(Scene& scene) override;
};
} // namespace Vp
