#pragma once

#include "Shader.h"

namespace Vp
{
class ScreenOverlayShader : public Shader
{
protected:
	GLint m_sourceSampler;

public:
	GLuint m_sourceTextureId{0};

	explicit ScreenOverlayShader(GLuint id);

	void init(bool initSuperclass) override;
	void setUniforms() override;
};
} // namespace Vp
