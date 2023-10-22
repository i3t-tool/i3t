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
#include "BoxBlurShader.h"

using namespace Vp;

BoxBlurShader::BoxBlurShader(GLuint id) : Shader(id)
{
	init(false);
}

void BoxBlurShader::init(bool initSuperclass)
{
	if (initSuperclass)
		Shader::init(true);

	m_sourceSampler = glGetUniformLocation(m_id, "source");
	m_kernelSizeId = glGetUniformLocation(m_id, "u_kernelSize");
	m_verticalId = glGetUniformLocation(m_id, "u_vertical");
	m_resolutionId = glGetUniformLocation(m_id, "u_resolution");
}

void BoxBlurShader::setUniforms()
{
	bindTexture2D(0, m_sourceTextureId, m_sourceSampler);

	glUniform1i(m_kernelSizeId, m_kernelSize);
	glUniform1i(m_verticalId, m_vertical);
	glUniform2fv(m_resolutionId, 1, glm::value_ptr(m_resolution));

	Shader::setUniforms();
}
