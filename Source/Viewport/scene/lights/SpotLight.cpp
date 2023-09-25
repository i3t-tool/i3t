#include "SpotLight.h"

using namespace Vp;

#include "Viewport/shader/PhongShader.h"

void SpotLight::setUniforms(const PhongShader& shader, int index) const
{
	const std::string prefix = "spotLights[" + std::to_string(index) + "]";
	glUniform1f(glGetUniformLocation(shader.m_id, (prefix + ".intensity").c_str()), intensity);
	glUniform3fv(glGetUniformLocation(shader.m_id, (prefix + ".color").c_str()), 1, glm::value_ptr(color));
	glUniform3fv(glGetUniformLocation(shader.m_id, (prefix + ".specular").c_str()), 1, glm::value_ptr(specular));
	glUniform3fv(glGetUniformLocation(shader.m_id, (prefix + ".position").c_str()), 1, glm::value_ptr(pos));
	glUniform3fv(glGetUniformLocation(shader.m_id, (prefix + ".direction").c_str()), 1, glm::value_ptr(direction));
	glUniform1f(glGetUniformLocation(shader.m_id, (prefix + ".radius").c_str()), radius);
	glUniform1f(glGetUniformLocation(shader.m_id, (prefix + ".cutoffAngle").c_str()),
	            glm::radians(90.0f - cutoffAngle));
	glUniform1f(glGetUniformLocation(shader.m_id, (prefix + ".cutoffSoftAngle").c_str()),
	            glm::radians(90.0f - cutoffSoftAngle));
}