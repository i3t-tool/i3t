#pragma once

#include "SceneModel.h"

#include "Viewport/shader/PhongShader.h"

namespace Vp
{
class FrustumObject;

/**
 * Viewport entity representing a Camera node in Core/GUI
 */
class SceneCamera : public SceneModel
{
protected:
	std::weak_ptr<FrustumObject> m_frustumOutline;
	std::weak_ptr<FrustumObject> m_frustum;

public:
	bool m_showFrustum{true};
	bool m_fillFrustum{true};
	glm::vec3 m_frustumColor{0.35f, 0.27f, 0.06f};
	glm::vec3 m_frustumOutlineColor{1.0f, 0.0f, 0.0f};

	glm::mat4 m_projectionMatrix{1};
	glm::mat4 m_viewMatrix{1};

	SceneCamera(Core::Mesh* mesh, PhongShader* shader);

	void render(glm::mat4 view, glm::mat4 projection) override;
	void update(Scene& scene) override;

	void onSceneAdd(Scene& scene) override;
	void onSceneRemove(Scene& scene) override;
};
} // namespace Vp
