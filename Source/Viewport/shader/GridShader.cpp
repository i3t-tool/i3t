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
#include "GridShader.h"

#include "Viewport/GfxUtils.h"

using namespace Vp;

GridShader::GridShader(GLuint id) : ObjectShader(id)
{
	m_pvm = false;
	m_pvmSeparate = true;
	m_normal = false;
	init(false);
}

void GridShader::init(bool initSuperclass)
{
	if (initSuperclass)
		Super::init(true);

	m_nearId = glGetUniformLocation(m_id, "u_near");
	m_farId = glGetUniformLocation(m_id, "u_far");

	m_showGridId = glGetUniformLocation(m_id, "u_showGrid");
	m_showXAxisId = glGetUniformLocation(m_id, "u_showXAxis");
	m_showYAxisId = glGetUniformLocation(m_id, "u_showYAxis");
	m_showZAxisId = glGetUniformLocation(m_id, "u_showZAxis");

	m_gridColorId = glGetUniformLocation(m_id, "u_gridColor");
	m_axisXColorId = glGetUniformLocation(m_id, "u_axisColors[0]");
	m_axisYColorId = glGetUniformLocation(m_id, "u_axisColors[1]");
	m_axisZColorId = glGetUniformLocation(m_id, "u_axisColors[2]");

	m_gridSizeId = glGetUniformLocation(m_id, "u_gridSize");
	m_gridStrengthId = glGetUniformLocation(m_id, "u_gridStrength");

	m_lineWidthId = glGetUniformLocation(m_id, "u_lineWidth");

	m_grid1FadeStartId = glGetUniformLocation(m_id, "u_grid1FadeStart");
	m_grid1FadeEndId = glGetUniformLocation(m_id, "u_grid1FadeEnd");
	m_grid2FadeStartId = glGetUniformLocation(m_id, "u_grid2FadeStart");
	m_grid2FadeEndId = glGetUniformLocation(m_id, "u_grid2FadeEnd");
}

void GridShader::setUniforms()
{
	ObjectShader::setUniforms();

	glUniform1i(m_showGridId, m_showGrid);
	glUniform1f(m_showXAxisId, m_showXAxis);
	glUniform1f(m_showYAxisId, m_showYAxis);
	glUniform1f(m_showZAxisId, m_showZAxis);

	glUniform3fv(m_gridColorId, 1, glm::value_ptr(*m_gridColor));
	glUniform3fv(m_axisXColorId, 1, glm::value_ptr(*m_axisXColor));
	glUniform3fv(m_axisYColorId, 1, glm::value_ptr(*m_axisYColor));
	glUniform3fv(m_axisZColorId, 1, glm::value_ptr(*m_axisZColor));

	glUniform1f(m_gridSizeId, m_gridSize);
	glUniform1f(m_gridStrengthId, m_gridStrength);

	glUniform1f(m_lineWidthId, m_lineWidth);

	glUniform1f(m_grid1FadeStartId, m_grid1FadeStart);
	glUniform1f(m_grid1FadeEndId, m_grid1FadeEnd);
	glUniform1f(m_grid2FadeStartId, m_grid2FadeStart);
	glUniform1f(m_grid2FadeEndId, m_grid2FadeEnd);

	float nearVal;
	float farVal;
	GfxUtils::extractZNearZFar(m_projection, nearVal, farVal);
	glUniform1f(m_nearId, nearVal);
	glUniform1f(m_farId, farVal);
}
void GridShaderGeneric::init(bool initSuperclass)
{
	if (initSuperclass)
		Super::init(true);

	modelMatrixInvId = glGetUniformLocation(m_id, "u_modelMatrixInv");
}
void GridShaderGeneric::setUniforms()
{
	Super::setUniforms();

	glUniformMatrix4fv(modelMatrixInvId, 1, GL_FALSE, glm::value_ptr(m_modelInv));
}
