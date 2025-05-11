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
#include "Shader.h"

using namespace Vp;

Shader::Shader(GLuint id) : m_id(id)
{
	Shader::init(false);
}

void Shader::init(bool initSuperclass)
{
	for (int i = 0; i < m_clippingPlanesId.size(); i++)
	{
		m_clippingPlanesId[i] =
		    glGetUniformLocation(m_id, (std::string("u_clippingPlanes[") + std::to_string(i) + "]").c_str());
	}
	m_wboitFlagId = glGetUniformLocation(m_id, "u_wboitFlag");
	m_wboitFuncId = glGetUniformLocation(m_id, "u_wboitFunc");
}

void Shader::use() const
{
	glUseProgram(m_id);
}

void Shader::setUniforms()
{
	if (supportsWboit())
	{
		glUniform1i(m_wboitFlagId, m_wboit ? GL_TRUE : GL_FALSE);
		if (m_wboit)
		{
			glUniform1i(m_wboitFuncId, m_wboitFunc);
		}
	}
	if (supportsUserClipping() && m_clippingPlanes)
	{
		for (int i = 0; i < m_clippingPlanesId.size(); i++)
		{
			glUniform4fv(m_clippingPlanesId[i], 1, glm::value_ptr((*m_clippingPlanes)[i]));
		}
	}
}

void Shader::setUniformsPerMeshPart(Core::Mesh::MeshPart& meshPart)
{
	// Empty
}

void Shader::bindTexture2D(GLuint textureUnit, GLuint textureID, GLint samplerLocation)
{
	glActiveTexture(GL_TEXTURE0 + textureUnit);
	glUniform1i(samplerLocation, textureUnit);
	glBindTexture(GL_TEXTURE_2D, textureID);
}

void Shader::bindTexture2DMS(GLuint textureUnit, GLuint textureID, GLint samplerLocation)
{
	glActiveTexture(GL_TEXTURE0 + textureUnit);
	glUniform1i(samplerLocation, textureUnit);
	glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, textureID);
}

bool Shader::hasUniform(GLint location) const
{
	return location != -1;
}

bool Shader::supportsWboit() const
{
	return hasUniform(m_wboitFlagId);
}

bool Shader::supportsUserClipping() const
{
	return hasUniform(m_clippingPlanesId[0]);
}
