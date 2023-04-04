#include "BoxBlurShader.h"

using namespace Vp;

BoxBlurShader::BoxBlurShader(GLuint id) : Shader(id) { init(false); }

void BoxBlurShader::init(bool initSuperclass)
{
	if (initSuperclass)
		Shader::init(true);

	m_sourceSampler = glGetUniformLocation(m_id, "source");
	m_kernelSizeId = glGetUniformLocation(m_id, "u_kernelSize");
	m_verticalId = glGetUniformLocation(m_id, "u_vertical");
	m_resolutionId = glGetUniformLocation(m_id, "u_resolution");
}

void BoxBlurShader::setUniforms()
{
	bindTexture2D(0, m_sourceTextureId, m_sourceSampler);

	glUniform1i(m_kernelSizeId, m_kernelSize);
	glUniform1i(m_verticalId, m_vertical);
	glUniform2fv(m_resolutionId, 1, glm::value_ptr(m_resolution));

	Shader::setUniforms();
}
