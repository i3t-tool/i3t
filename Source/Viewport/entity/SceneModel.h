#pragma once

#include "TexturedObject.h"

namespace Vp
{
class PhongShader;

/**
 * Viewport entity representing a Model node in Core/GUI
 */
class SceneModel : public TexturedObject
{
public:
	bool m_showAxes{true};
	bool m_transparent{false}; // TODO: Implement

	std::weak_ptr<GameObject> m_axes;

	SceneModel(Core::Mesh* mesh, PhongShader* shader);

	void update(Scene& scene) override;

	void onSceneAdd(Scene& scene) override;
	void onSceneRemove(Scene& scene) override;
};
} // namespace Vp
