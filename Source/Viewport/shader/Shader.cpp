#include "Shader.h"

using namespace Vp;

Shader::Shader(GLuint id) : id(id) { wboitFlagUniform = glGetUniformLocation(id, "u_wboitFlag"); }

void Shader::use() const { glUseProgram(id); }

void Shader::setUniforms()
{
	if (supportsWboit())
	{
		glUniform1i(wboitFlagUniform, m_wboit ? GL_TRUE : GL_FALSE);
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

bool Shader::hasUniform(GLint location) { return location != -1; }

bool Shader::supportsWboit() { return hasUniform(wboitFlagUniform); }
