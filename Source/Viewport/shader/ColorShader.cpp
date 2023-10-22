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
#include "ColorShader.h"

using namespace Vp;

ColorShader::ColorShader(GLuint id) : ObjectShader(id)
{
	init(false);
}

void ColorShader::init(bool initSuperclass)
{
	if (initSuperclass)
		ObjectShader::init(true);

	useSingleColorId = glGetUniformLocation(m_id, "useSingleColor");
	singleColorId = glGetUniformLocation(m_id, "singleColor");
}

void ColorShader::setUniforms()
{
	glUniform1i(useSingleColorId, m_useSingleColor);
	glUniform3fv(singleColorId, 1, glm::value_ptr(m_singleColor));

	ObjectShader::setUniforms();
}
