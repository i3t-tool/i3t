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
