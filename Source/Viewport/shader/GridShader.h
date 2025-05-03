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

#include "ObjectShader.h"

namespace Vp
{
/**
 * Shader for the infinite grid of the standard basis
 */
class GridShader : public ObjectShader
{
	using Super = ObjectShader;

protected:
	GLint m_nearId;
	GLint m_farId;

	GLint m_showGridId;
	GLint m_showXAxisId;
	GLint m_showYAxisId;
	GLint m_showZAxisId;

	GLint m_gridColorId;
	GLint m_axisXColorId;
	GLint m_axisYColorId;
	GLint m_axisZColorId;

	GLint m_gridSizeId;
	GLint m_gridStrengthId;

	GLint m_lineWidthId;

	GLint m_grid1FadeStartId;
	GLint m_grid1FadeEndId;
	GLint m_grid2FadeStartId;
	GLint m_grid2FadeEndId;

	GLint modelMatrixInvId;

public:
	bool m_showGrid = true;
	bool m_showXAxis = true;
	bool m_showYAxis = true;
	bool m_showZAxis = true;

	const float* m_gridColor;  // vec4
	const float* m_axisXColor; // vec4
	const float* m_axisYColor; // vec4
	const float* m_axisZColor; // vec4

	float m_gridSize{1.0f};
	float m_gridStrength{0.5f};

	float m_lineWidth{1.0f};

	float m_grid1FadeStart{0.0f};
	float m_grid1FadeEnd{0.4f};
	float m_grid2FadeStart{0.1f};
	float m_grid2FadeEnd{1.0f};

	glm::mat4 m_modelInv;

public:
	explicit GridShader(GLuint id);

	void init(bool initSuperclass) override;
	void setUniforms() override;
};

/**
 * Shader for a generic infinite grid representing an affine space with any basis.
 */
class GridShaderGeneric : public GridShader
{
	using Super = GridShader;

public:
	explicit GridShaderGeneric(GLuint id) : GridShader(id)
	{
		init(false);
	}

	void init(bool initSuperclass) override;
	void setUniforms() override;
};

} // namespace Vp