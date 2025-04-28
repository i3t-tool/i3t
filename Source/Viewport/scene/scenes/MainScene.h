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

#include "Viewport/data/ViewportSceneSettings.h"
#include "Viewport/scene/Scene.h"

namespace Vp
{
class SunLight;
class GridObject;

class MainScene : public Scene
{
protected:
	SunLight* m_sun1;
	SunLight* m_sun2;

	glm::vec3 m_sun1_dir;
	glm::vec3 m_sun2_dir;

	float m_sun2_intensity;

public:
	/// Grid representing the world space standard basis
	std::shared_ptr<GridObject> m_worldGrid;

	/// Grid representing current local/reference space
	std::shared_ptr<GridObject> m_localGrid;

	bool m_lightFollowsCamera = true;

	explicit MainScene(Viewport* viewport);

	void init() override;
	void update(double dt) override;

	using Scene::draw;

	void draw(int width, int height, const glm::mat4& model, const glm::mat4& view, const glm::mat4& projection,
	          SceneRenderTarget& renderTarget, const DisplayOptions& displayOptions) override;
};
} // namespace Vp
