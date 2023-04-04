#pragma once

#include "Shader.h"

namespace Vp
{
class WBOITCompositeShader : public Shader
{
protected:
	GLint accumulationSampler;
	GLint revealageSampler;

public:
	GLuint accumulationTextureID{0};
	GLuint revealageTextureID{0};

	explicit WBOITCompositeShader(GLuint id);

	void init(bool initSuperclass) override;
	void setUniforms() override;
};
} // namespace Vp
