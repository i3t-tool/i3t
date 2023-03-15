#include "ColorShader.h"

using namespace Vp;

ColorShader::ColorShader(GLuint id) : ObjectShader(id)
{
	useSingleColorId = glGetUniformLocation(id, "useSingleColor");
	singleColorId = glGetUniformLocation(id, "singleColor");
}

void ColorShader::setUniforms()
{
	glUniform1i(useSingleColorId, m_useSingleColor);
	glUniform3fv(singleColorId, 1, glm::value_ptr(m_singleColor));

	ObjectShader::setUniforms();
}
