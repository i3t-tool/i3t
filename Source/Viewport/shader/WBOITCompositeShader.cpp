#include "WBOITCompositeShader.h"

using namespace Vp;

WBOITCompositeShader::WBOITCompositeShader(GLuint id) : Shader(id)
{
	accumulationSampler = glGetUniformLocation(id, "accum");
	revealageSampler = glGetUniformLocation(id, "reveal");
}

void WBOITCompositeShader::setUniforms()
{
	bindTexture2D(0, accumulationTextureID, accumulationSampler);
	bindTexture2D(1, revealageTextureID, revealageSampler);
}
