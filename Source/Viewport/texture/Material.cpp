#include "Material.h"

using namespace Vp;

void Material::setUniforms(PhongShader& shader) const
{
	glUniform3fv(glGetUniformLocation(shader.id, "material.diffuse"), 1,
	             glm::value_ptr(diffuse));
	glUniform3fv(glGetUniformLocation(shader.id, "material.specular"), 1,
	             glm::value_ptr(specular));
	glUniform3fv(glGetUniformLocation(shader.id, "material.ambient"), 1,
	             glm::value_ptr(ambient));
	glUniform1f(glGetUniformLocation(shader.id, "material.shininess"), shininess);
	CHECK_GL_ERROR();
}
