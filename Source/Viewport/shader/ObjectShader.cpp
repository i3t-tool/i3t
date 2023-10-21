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
#include "ObjectShader.h"

using namespace Vp;

ObjectShader::ObjectShader(GLuint id) : WorldShader(id)
{
	init(false);
}

void ObjectShader::init(bool initSuperclass)
{
	if (initSuperclass)
		WorldShader::init(true);

	opacityId = glGetUniformLocation(m_id, "u_opacity");
}

void ObjectShader::setUniforms()
{
	glUniform1f(opacityId, m_opacity);
	WorldShader::setUniforms();
}
