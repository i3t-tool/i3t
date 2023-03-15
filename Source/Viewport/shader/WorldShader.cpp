#include "WorldShader.h"

using namespace Vp;

WorldShader::WorldShader(GLuint id) : Shader(id)
{
	pvmMatrixId = glGetUniformLocation(id, "pvmMatrix");
	projectionMatrixId = glGetUniformLocation(id, "projectionMatrix");
	modelMatrixId = glGetUniformLocation(id, "modelMatrix");
	viewMatrixId = glGetUniformLocation(id, "viewMatrix");
	normalMatrixId = glGetUniformLocation(id, "normalMatrix");
}

void WorldShader::setUniforms()
{
	glm::mat4 pvm = m_projection * m_view * m_model;
	const glm::mat4 modelRotationMatrix =
	    glm::mat4(m_model[0], m_model[1], m_model[2], glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
	glm::mat4 normalMatrix = glm::transpose(glm::inverse(modelRotationMatrix));

	glUniformMatrix4fv(pvmMatrixId, 1, GL_FALSE, glm::value_ptr(pvm));
	glUniformMatrix4fv(projectionMatrixId, 1, GL_FALSE, glm::value_ptr(m_projection));
	glUniformMatrix4fv(viewMatrixId, 1, GL_FALSE, glm::value_ptr(m_view));
	glUniformMatrix4fv(modelMatrixId, 1, GL_FALSE, glm::value_ptr(m_model));
	glUniformMatrix4fv(normalMatrixId, 1, GL_FALSE, glm::value_ptr(normalMatrix));

	Shader::setUniforms();
}

void WorldShader::setWorldTransform(const glm::mat4& model, const glm::mat4& view, const glm::mat4& projection)
{
	m_model = model;
	m_view = view;
	m_projection = projection;
}
