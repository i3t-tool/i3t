#include "Shader.h"

using namespace Vp;

Shader::Shader(GLuint id) : id(id) { wboitFlagUniform = glGetUniformLocation(id, "u_wboitFlag"); }

void Shader::use() const { glUseProgram(id); }

void Shader::bindTexture2D(GLuint textureUnit, GLuint textureID, GLint samplerLocation)
{
	glActiveTexture(GL_TEXTURE0 + textureUnit);
	glUniform1i(samplerLocation, textureUnit);
	glBindTexture(GL_TEXTURE_2D, textureID);
}

bool Shader::hasUniform(GLint location) { return location != -1; }
