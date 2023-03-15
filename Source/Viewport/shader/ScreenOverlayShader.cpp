#include "ScreenOverlayShader.h"

using namespace Vp;

ScreenOverlayShader::ScreenOverlayShader(GLuint id) : Shader(id)
{
	m_sourceSampler = glGetUniformLocation(id, "source");
}

void ScreenOverlayShader::setUniforms()
{
	bindTexture2D(0, m_sourceTextureId, m_sourceSampler);

	Shader::setUniforms();
}
