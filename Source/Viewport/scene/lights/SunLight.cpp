#include "SunLight.h"

using namespace Vp;

#include "Viewport/shader/PhongShader.h"

void SunLight::setUniforms(const PhongShader& shader, int index) const
{
	const std::string prefix = "sunLights[" + std::to_string(index) + "]";
	glUniform1f(glGetUniformLocation(shader.m_id, (prefix + ".intensity").c_str()), intensity);
	glUniform3fv(glGetUniformLocation(shader.m_id, (prefix + ".color").c_str()), 1, glm::value_ptr(color));
	glUniform3fv(glGetUniformLocation(shader.m_id, (prefix + ".specular").c_str()), 1, glm::value_ptr(specular));
	glUniform3fv(glGetUniformLocation(shader.m_id, (prefix + ".direction").c_str()), 1, glm::value_ptr(direction));
}
