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

#include "SceneSelectable.h"
#include "Viewport/entity/TexturedObject.h"

namespace Vp
{
class PhongShader;

/**
 * Viewport entity representing a screen node. Only visible when visualizing screen space.
 * Monitor 3D model credits: LiveToWin34 (https://skfb.ly/otsCS) (CC Attribution 4.0) (modified)
 */
class SceneScreen : public TexturedObject, public SceneSelectable
{
	float m_zOffset{0.f}; ///< Tiny random z offset to prevent z-fighting

public:
	std::weak_ptr<TexturedObject> m_screenBase;

	float m_scale{1.f};          ///< Optional scaling factor of the model, eg. an implicit model transformation.
	float m_programOpacity{1.f}; ///< Runtime opacity factor

	SceneScreen();

	void updateModelTransform(float width, float height, float scaleFactor);

	void update(Scene& scene) override;

	void onSceneAdd(Scene& scene) override;
	void onSceneRemove(Scene& scene) override;
};

} // namespace Vp
