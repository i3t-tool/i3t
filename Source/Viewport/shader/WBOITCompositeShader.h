#pragma once

#include "ColorShader.h"

namespace Vp
{
class WBOITCompositeShader : public Shader
{
public:
	GLint accumulationSampler;
	GLint revealageSampler;

	GLuint accumulationTextureID{0};
	GLuint revealageTextureID{0};

	explicit WBOITCompositeShader(GLuint id);

	void setUniforms() override;
};
} // namespace Vp
