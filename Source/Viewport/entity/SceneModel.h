#pragma once

#include "TexturedObject.h"

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
	bool m_showAxes{true};
	bool m_transparent{false}; // TODO: Implement

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
