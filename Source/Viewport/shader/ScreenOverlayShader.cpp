#include "ScreenOverlayShader.h"

using namespace Vp;

ScreenOverlayShader::ScreenOverlayShader(GLuint id) : Shader(id)
{
	init(false);
}

void ScreenOverlayShader::init(bool initSuperclass)
{
	if (initSuperclass)
		Shader::init(true);

	m_sourceSampler = glGetUniformLocation(m_id, "source");
}

void ScreenOverlayShader::setUniforms()
{
	bindTexture2D(0, m_sourceTextureId, m_sourceSampler);

	Shader::setUniforms();
}
