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

#include "GameObject.h"

namespace Vp
{
class FrustumShader;

class GridObject : public GameObject
{
	using Super = GameObject;

public:
	bool isStandard{true};
	glm::mat4 m_modelMatrixInv{1.f};

	bool m_showGrid = true;
	bool m_showXAxis = true;
	bool m_showYAxis = false;
	bool m_showZAxis = true;

	glm::vec3* m_gridColor;
	glm::vec3* m_axisXColor;
	glm::vec3* m_axisYColor;
	glm::vec3* m_axisZColor;

	float m_gridSize{1.0f};
	float m_gridStrength{0.5f};

	float m_lineWidth{1.0f};

	float m_grid1FadeStart{0.0f};
	float m_grid1FadeEnd{0.4f};
	float m_grid2FadeStart{0.1f};
	float m_grid2FadeEnd{1.0f};

	GridObject();

	void setReferenceSpace(const glm::mat4& referenceSpace, const glm::mat4& referenceSpaceInv);

	void prepareRenderContext(RenderContext& context) override;
	void render(const glm::mat4& model, const glm::mat4& view, const glm::mat4& projection,
	            const RenderContext& context) override;

protected:
	void updateGridShader();
};
} // namespace Vp
