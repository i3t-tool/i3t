#pragma once

#include "Shader.h"

namespace Vp
{
/**
 * Two pass box blur with alpha support
 */
class BoxBlurShader : public Shader
{
protected:
	GLint m_sourceSampler;

	GLint m_kernelSizeId;
	GLint m_verticalId;
	GLint m_resolutionId;

public:
	GLuint m_sourceTextureId{0};

	int m_kernelSize{10};   ///< Size of the blur kernel in pixels
	bool m_vertical{true};  ///< Vertical or horizontal blur pass
	glm::vec2 m_resolution; ///< Resolution of the current buffer in pixels

	explicit BoxBlurShader(GLuint id);

	void init(bool initSuperclass) override;
	void setUniforms() override;
};
} // namespace Vp
