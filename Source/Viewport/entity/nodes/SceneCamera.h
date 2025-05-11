/**
 * \file
 * \brief
 * \author Dan Rakušan <rakusan.dan@gmail.com>
 * \copyright Copyright (C) 2016-2025 I3T team, Department of Computer Graphics
 * and Interaction, FEE, Czech Technical University in Prague, Czech Republic
 *
 * This file is part of I3T - An Interactive Tool for Teaching Transformations
 * http://www.i3t-tool.org
 *
 * GNU General Public License v3.0 (see LICENSE.txt or https://www.gnu.org/licenses/gpl-3.0.txt)
 */
#pragma once

#include "SceneModel.h"

#include "Viewport/shader/PhongShader.h"

namespace Vp
{
class FrustumObject;

/**
 * Viewport entity representing a Camera node in Core/GUI
 *
 * This entity represents the camera model and manages several additional entities to represent various frustum
 * indicators.
 */
class SceneCamera : public SceneModel
{
protected:
	std::weak_ptr<FrustumObject> m_frustumOutline; ///< Opaque frustum bounds outline
	std::weak_ptr<FrustumObject> m_frustum;        ///< The transparent frustum fill

	std::weak_ptr<ColoredObject> m_frustumNear; ///< Lines from the camera to the near plane, hidden during tracking
	std::shared_ptr<Core::Mesh> m_frustumNearMesh;

	std::shared_ptr<Core::Mesh> m_trackedFrustumNearMesh;

	bool m_isTracking{false};

public:
	bool m_showCamera{true}; ///< Show/Hide the camera model, use this instead of this->m_visible.
	bool m_showFrustum{true};
	bool m_fillFrustum{true};

	bool m_visualizeDepth{false};

	glm::vec4 m_frustumColor{0.35f, 0.27f, 0.06f, 0.22f};

	glm::vec3 m_frustumOutlineColor{1.0f, 0.0f, 0.0f};
	const float m_frustumNearLinesOpacity = 0.4f;

	glm::mat4 m_projectionMatrix{1.f};
	glm::mat4 m_viewMatrix{1.f};

	/// When tracking projection, the model of the camera is hidden and is replaced by this model, which ignores
	/// reference space and is positioned in a special manner.
	std::weak_ptr<TexturedObject> m_trackedCameraModel;

	/// Lines from the camera to the near plane, adjusted to ignore reference space, only visible during tracking
	std::weak_ptr<ColoredObject> m_trackedFrustumNear;

	SceneCamera(Core::Mesh* mesh, PhongShader* shader);
	~SceneCamera() override;

	void update(Scene& scene) override;
	void render(const glm::mat4& model, const glm::mat4& view, const glm::mat4& projection,
	            const RenderContext& context) override;

	void onSceneAdd(Scene& scene) override;
	void onSceneRemove(Scene& scene) override;

protected:
	void updateNearFrustumIndicator(const glm::mat4& model, const glm::mat4& projViewInv);
};
} // namespace Vp
