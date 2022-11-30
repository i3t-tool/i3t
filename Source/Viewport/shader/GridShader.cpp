#include "GridShader.h"

using namespace Vp;

GridShader::GridShader(GLuint id) : ObjectShader(id)
{
	m_nearId = glGetUniformLocation(id, "near");
	m_farId = glGetUniformLocation(id, "far");
}
void GridShader::setUniforms()
{
	WorldShader::setUniforms();

	// Extract near and far values from projection matrix
	float nearVal;
	float farVal;

	if (m_projection[2][3] == 0)
	{
		// Orthographic matrix
		float m32 = m_projection[3][2];
		float m22 = m_projection[2][2];
		nearVal = (m32 + 1) / m22;
		farVal = (m32 - 1) / m22;
	}
	else
	{
		// Perspective matrix
		float m32 = m_projection[3][2];
		float m22 = m_projection[2][2];
		nearVal = m32 / (m22 - 1);
		farVal = m32 / (m22 + 1);
	}
	glUniform1f(m_nearId, nearVal);
	glUniform1f(m_farId, farVal);
}
