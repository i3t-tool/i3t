/**
 * \file
 * \brief
 * \author Dan Rakušan <rakusan.dan@gmail.com>
 * \copyright Copyright (C) 2016-2023 I3T team, Department of Computer Graphics
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

	void update(Scene& scene) override;

	void onSceneAdd(Scene& scene) override;
	void onSceneRemove(Scene& scene) override;
};
} // namespace Vp
