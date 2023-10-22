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
#include "ScreenOverlayShader.h"

using namespace Vp;

ScreenOverlayShader::ScreenOverlayShader(GLuint id) : Shader(id)
{
	init(false);
}

void ScreenOverlayShader::init(bool initSuperclass)
{
	if (initSuperclass)
		Shader::init(true);

	m_sourceSampler = glGetUniformLocation(m_id, "source");
}

void ScreenOverlayShader::setUniforms()
{
	bindTexture2D(0, m_sourceTextureId, m_sourceSampler);

	Shader::setUniforms();
}
