#include "ColorShader.h"

using namespace Vp;

ColorShader::ColorShader(GLuint id) : ObjectShader(id) { init(false); }

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
