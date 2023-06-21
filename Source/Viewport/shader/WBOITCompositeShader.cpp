#include "WBOITCompositeShader.h"

using namespace Vp;

WBOITCompositeShader::WBOITCompositeShader(GLuint id) : Shader(id)
{
	init(false);
}

void WBOITCompositeShader::init(bool initSuperclass)
{
	if (initSuperclass)
		Shader::init(true);

	accumulationSampler = glGetUniformLocation(m_id, "accum");
	revealageSampler = glGetUniformLocation(m_id, "reveal");
}

void WBOITCompositeShader::setUniforms()
{
	bindTexture2D(0, accumulationTextureID, accumulationSampler);
	bindTexture2D(1, revealageTextureID, revealageSampler);
	Shader::setUniforms();
}
