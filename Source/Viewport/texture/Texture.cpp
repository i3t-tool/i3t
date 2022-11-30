#include "Texture.h"

#include <iostream>

#include "../shader/PhongShader.h"

using namespace Vp;

Texture::Texture(const char* path, std::string type, GLenum target)
    : target(target), type(type)
{
	id = loadTexture(path);
	this->path = path;
}

void Texture::setAnimated(float horizontalStep, float verticalStep, float speed)
{
	this->animated = true;
	this->horizontalStep = horizontalStep;
	this->verticalStep = verticalStep;
	this->animationSpeed = speed;
}

void Texture::setUvTransform(glm::mat3 matrix)
{
	this->transformUvs = true;
	this->uvTransformMatrix = matrix;
}

void Texture::bind() const
{
	glBindTexture(target, id);
	CHECK_GL_ERROR();
}

void Texture::bind(int typeIndex, GLuint textureUnit, PhongShader& shader)
{
	// Activate texture unit
	glActiveTexture(GL_TEXTURE0 + textureUnit);
	// Find appropriate sampler and set its texture unit
	glUniform1i(glGetUniformLocation(shader.id,
	                                 (type + std::to_string(typeIndex)).c_str()),
	            textureUnit);
	// Also enable flag saying this sampler is active
	glUniform1i(
	    glGetUniformLocation(
	        shader.id, (type + std::to_string(typeIndex) + "_active").c_str()),
	    GL_TRUE);
	// Bind the texture to that texture unit

	// Set animation uniforms
	glUniform1i(glGetUniformLocation(shader.id, "animTexture"), animated);
	if (animated)
	{
		glUniform1f(glGetUniformLocation(shader.id, "animHStep"), horizontalStep);
		glUniform1f(glGetUniformLocation(shader.id, "animVStep"), verticalStep);
		glUniform1f(glGetUniformLocation(shader.id, "animSpeed"), animationSpeed);
	}

	// Set UV transform uniforms
	if (transformUvs)
	{
		glUniform1i(glGetUniformLocation(shader.id, "uvMatActive"), true);
		glUniformMatrix3fv(glGetUniformLocation(shader.id, "uvMat"), 1, GL_FALSE,
		                   glm::value_ptr(uvTransformMatrix));
	}

	bind();
}

void Texture::dispose() { glDeleteTextures(1, &id); }

void Texture::setClampToEdge()
{
	this->bind();
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
}

GLuint Texture::loadTexture(const char* path)
{
	std::cout << "[TEXTURE] Loading texture file: " << path << std::endl;
	return pgr::createTexture(path);
}
