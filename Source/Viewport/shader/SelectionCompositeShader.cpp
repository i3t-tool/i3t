#include "SelectionCompositeShader.h"

using namespace Vp;

SelectionCompositeShader::SelectionCompositeShader(GLuint id) : Shader(id)
{
	m_sourceSampler = glGetUniformLocation(id, "source");
	m_resolutionId = glGetUniformLocation(id, "u_resolution");
	m_cutoffId = glGetUniformLocation(id, "u_cutoff");
}

void SelectionCompositeShader::setUniforms()
{
	bindTexture2D(0, m_sourceTextureId, m_sourceSampler);

	glUniform2fv(m_resolutionId, 1, glm::value_ptr(m_resolution));
	glUniform1f(m_cutoffId, m_cutoff);

	Shader::setUniforms();
}
