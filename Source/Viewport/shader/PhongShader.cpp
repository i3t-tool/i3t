#include "PhongShader.h"

#define MAX_DIFFUSE_TEXTURES 1
#define MAX_SPECULAR_TEXTURES 1
#define MAX_NORMAL_TEXTURES 1
#define MAX_AO_TEXTURES 1
#define MAX_EMISSION_TEXTURES 1

using namespace Vp;

PhongShader::PhongShader(GLuint id) : ObjectShader(id)
{
	material_diffuse = glGetUniformLocation(id, "material.diffuse");
	material_specular = glGetUniformLocation(id, "material.specular");
	material_ambient = glGetUniformLocation(id, "material.ambient");
	material_shininess = glGetUniformLocation(id, "material.shininess");

	normalStrength = glGetUniformLocation(id, "normalStrength");

	tintId = glGetUniformLocation(id, "u_tint");
}

void PhongShader::setUniforms()
{
	glUniform3fv(tintId, 1, glm::value_ptr(m_tint));

	// Alpha cutoff
	// glUniform1f(glGetUniformLocation(id, "alphaCutoff"), 0.1f); // Default
	// value

	ObjectShader::setUniforms();
}

void PhongShader::setUniformsPerMeshPart(Core::Mesh::MeshPart& meshPart)
{
	setMaterialUniforms(meshPart.material);
	clearTextures();
	bindTextures(meshPart.textureSet);

	ObjectShader::setUniformsPerMeshPart(meshPart);
}

void PhongShader::setMaterialUniforms(Core::Mesh::Material material)
{
	glUniform3fv(material_diffuse, 1, glm::value_ptr(material.diffuse));
	glUniform3fv(material_specular, 1, glm::value_ptr(material.specular));
	glUniform3fv(material_ambient, 1, glm::value_ptr(material.ambient));
	glUniform1f(material_shininess, material.shininess);
}

void PhongShader::bindTextures(Core::Mesh::TextureSet tSet)
{
	if (tSet.texture != 0)
	{
		bindTexture(tSet.texture, "diffuse", 0, 0);
	}
	if (tSet.specularMap != 0)
	{
		bindTexture(tSet.specularMap, "specular", 0, 1);
	}
	if (tSet.normalMap != 0)
	{
		bindTexture(tSet.normalMap, "normal", 0, 2);
		glUniform1f(normalStrength, tSet.normalStrength);
	}
	if (tSet.aoMap != 0)
	{
		bindTexture(tSet.aoMap, "ao", 0, 3);
	}
	if (tSet.emissionMap != 0)
	{
		bindTexture(tSet.emissionMap, "emission", 0, 4);
	}
}

void PhongShader::bindTexture(GLuint textureID, const std::string& type,
                              int typeIndex, GLuint textureUnit)
{
	// Activate texture unit
	glActiveTexture(GL_TEXTURE0 + textureUnit);
	// Find appropriate sampler and set its texture unit
	glUniform1i(
	    glGetUniformLocation(id, (type + std::to_string(typeIndex)).c_str()),
	    textureUnit);
	// Also enable flag saying this sampler is active
	glUniform1i(glGetUniformLocation(
	                id, (type + std::to_string(typeIndex) + "_active").c_str()),
	            GL_TRUE);
	// Bind the texture to that texture unit
	glBindTexture(GL_TEXTURE_2D, textureID);
}

void PhongShader::clearTextures() const
{
	std::string type;
	type = "diffuse";
	for (int i = 0; i < MAX_DIFFUSE_TEXTURES; i++)
	{
		glUniform1i(glGetUniformLocation(
		                id, (type + std::to_string(i) + "_active").c_str()),
		            GL_FALSE);
	}
	type = "specular";
	for (int i = 0; i < MAX_SPECULAR_TEXTURES; i++)
	{
		glUniform1i(glGetUniformLocation(
		                id, (type + std::to_string(i) + "_active").c_str()),
		            GL_FALSE);
	}
	type = "normal";
	for (int i = 0; i < MAX_NORMAL_TEXTURES; i++)
	{
		glUniform1i(glGetUniformLocation(
		                id, (type + std::to_string(i) + "_active").c_str()),
		            GL_FALSE);
	}
	type = "ao";
	for (int i = 0; i < MAX_AO_TEXTURES; i++)
	{
		glUniform1i(glGetUniformLocation(
		                id, (type + std::to_string(i) + "_active").c_str()),
		            GL_FALSE);
	}
	type = "emission";
	for (int i = 0; i < MAX_EMISSION_TEXTURES; i++)
	{
		glUniform1i(glGetUniformLocation(
		                id, (type + std::to_string(i) + "_active").c_str()),
		            GL_FALSE);
	}
}
