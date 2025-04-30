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
#include "WorldShader.h"

#include "Utils/ProjectionUtils.h"
#include "Viewport/GfxUtils.h"

using namespace Vp;

WorldShader::WorldShader(GLuint id) : Shader(id)
{
	init(false);
}

void WorldShader::init(bool initSuperclass)
{
	if (initSuperclass)
		Shader::init(true);

	if (m_pvm)
		pvmMatrixId = glGetUniformLocation(m_id, "pvmMatrix");
	if (m_pvmSeparate)
	{
		projectionMatrixId = glGetUniformLocation(m_id, "projectionMatrix");
		modelMatrixId = glGetUniformLocation(m_id, "modelMatrix");
		viewMatrixId = glGetUniformLocation(m_id, "viewMatrix");
	}
	if (m_normal)
		normalMatrixId = glGetUniformLocation(m_id, "normalMatrix");

	m_wboitNearId = glGetUniformLocation(m_id, "u_wboitNear");
	m_wboitFarId = glGetUniformLocation(m_id, "u_wboitFar");
}

void WorldShader::setUniforms()
{
	if (m_pvm)
	{
		glm::mat4 pvm = m_projection * m_view * m_model;
		glUniformMatrix4fv(pvmMatrixId, 1, GL_FALSE, glm::value_ptr(pvm));
	}
	if (m_pvmSeparate)
	{
		glUniformMatrix4fv(projectionMatrixId, 1, GL_FALSE, glm::value_ptr(m_projection));
		glUniformMatrix4fv(viewMatrixId, 1, GL_FALSE, glm::value_ptr(m_view));
		glUniformMatrix4fv(modelMatrixId, 1, GL_FALSE, glm::value_ptr(m_model));
	}
	if (m_normal)
	{
		const glm::mat4 modelRotationMatrix =
		    glm::mat4(m_model[0], m_model[1], m_model[2], glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
		glm::mat4 normalMatrix = glm::transpose(glm::inverse(modelRotationMatrix));
		glUniformMatrix4fv(normalMatrixId, 1, GL_FALSE, glm::value_ptr(normalMatrix));
	}

	if (supportsWboit() && m_wboit)
	{
		float nearVal;
		float farVal;
		ProjectionUtils::extractZNearZFar(m_projection, nearVal, farVal);
		glUniform1f(m_wboitNearId, nearVal);
		glUniform1f(m_wboitFarId, farVal);
	}

	Shader::setUniforms();
}

void WorldShader::setWorldTransform(const glm::mat4& model, const glm::mat4& view, const glm::mat4& projection)
{
	m_model = model;
	m_view = view;
	m_projection = projection;
}
