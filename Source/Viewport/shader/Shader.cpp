#include "Shader.h"

using namespace Vp;

Shader::Shader(GLuint id) : id(id)
{
	// Empty
}

void Shader::use() { glUseProgram(id); }

void Shader::dispose() { pgr::deleteProgramAndShaders(id); }
