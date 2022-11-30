#include "ObjectShader.h"

using namespace Vp;

ObjectShader::ObjectShader(GLuint id) : WorldShader(id)
{
	opacityId = glGetUniformLocation(id, "u_opacity");
}

void ObjectShader::setUniforms()
{
	glUniform1f(opacityId, m_opacity);
	WorldShader::setUniforms();
}
