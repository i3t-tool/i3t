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
#include "FrustumShader.h"
#include "Utils/Color.h"

using namespace Vp;

FrustumShader::FrustumShader(GLuint id) : ColorShader(id)
{
	init(false);
}

void FrustumShader::init(bool initSuperclass)
{
	if (initSuperclass)
		ColorShader::init(true);

	inverseProjectionMatrixId = glGetUniformLocation(m_id, "projectionInverseMatrix");
	visualizeDepthId = glGetUniformLocation(m_id, "u_visualizeDepth");
}

void FrustumShader::setUniforms()
{
	ColorShader::setUniforms();

	glUniformMatrix4fv(inverseProjectionMatrixId, 1, GL_FALSE, glm::value_ptr(m_projectionViewMatrixInv));
	glUniform1i(visualizeDepthId, m_visualizeDepth ? GL_TRUE : GL_FALSE);
}
