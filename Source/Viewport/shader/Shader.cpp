#include "Shader.h"

using namespace Vp;

Shader::Shader(GLuint id) : m_id(id)
{
	init(false);
}

void Shader::init(bool initSuperclass)
{
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

bool Shader::hasUniform(GLint location)
{
	return location != -1;
}

bool Shader::supportsWboit()
{
	return hasUniform(m_wboitFlagId);
}
