#include "GridShader.h"

#include "Viewport/GfxUtils.h"

using namespace Vp;

GridShader::GridShader(GLuint id) : ObjectShader(id) { init(false); }

void GridShader::init(bool initSuperclass)
{
	if (initSuperclass)
		ObjectShader::init(true);

	m_nearId = glGetUniformLocation(m_id, "u_near");
	m_farId = glGetUniformLocation(m_id, "u_far");

	m_gridColorId = glGetUniformLocation(m_id, "u_gridColor");
	m_gridSizeId = glGetUniformLocation(m_id, "u_gridSize");
	m_axisWidthId = glGetUniformLocation(m_id, "u_axisWidth");
	m_gridStrengthId = glGetUniformLocation(m_id, "u_gridStrength");
	m_gridFadeOffsetId = glGetUniformLocation(m_id, "u_gridFadeOffset");
}

void GridShader::setUniforms()
{
	ObjectShader::setUniforms();

	glUniform3fv(m_gridColorId, 1, glm::value_ptr(m_gridColor));
	glUniform1f(m_gridSizeId, m_gridSize);
	glUniform1f(m_axisWidthId, m_axisWidth);
	glUniform1f(m_gridStrengthId, m_gridStrength);
	glUniform1f(m_gridFadeOffsetId, m_gridFadeOffset);

	float nearVal;
	float farVal;
	GfxUtils::extractZNearZFar(m_projection, nearVal, farVal);
	glUniform1f(m_nearId, nearVal);
	glUniform1f(m_farId, farVal);
}
