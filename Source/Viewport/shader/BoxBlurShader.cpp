#include "BoxBlurShader.h"

using namespace Vp;

BoxBlurShader::BoxBlurShader(GLuint id) : Shader(id)
{
	m_sourceSampler = glGetUniformLocation(id, "source");
	m_kernelSizeId = glGetUniformLocation(id, "u_kernelSize");
	m_verticalId = glGetUniformLocation(id, "u_vertical");
	m_resolutionId = glGetUniformLocation(id, "u_resolution");
}

void BoxBlurShader::setUniforms()
{
	bindTexture2D(0, m_sourceTextureId, m_sourceSampler);

	glUniform1i(m_kernelSizeId, m_kernelSize);
	glUniform1i(m_verticalId, m_vertical);
	glUniform2fv(m_resolutionId, 1, glm::value_ptr(m_resolution));

	Shader::setUniforms();
}
