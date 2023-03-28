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
}

void GridShader::setUniforms()
{
	ObjectShader::setUniforms();

	float nearVal;
	float farVal;
	GfxUtils::extractZNearZFar(m_projection, nearVal, farVal);
	glUniform1f(m_nearId, nearVal);
	glUniform1f(m_farId, farVal);
}
