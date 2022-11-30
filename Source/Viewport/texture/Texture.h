#pragma once

#include "pgr.h"

namespace Vp
{
class PhongShader;

class Texture
{
public:
	GLuint id;
	GLenum target;
	std::string type;
	const char* path;

	// Animation
	bool animated = false;
	float verticalStep;
	float horizontalStep;
	float animationSpeed;

	// UV transformation matrix
	bool transformUvs = false;
	glm::mat3 uvTransformMatrix = glm::mat3(1);

	Texture(const char* path, std::string type, GLenum target);

	void setAnimated(float horizontalStep, float verticalStep, float speed);
	void setUvTransform(glm::mat3 matrix);

	/**
	 * \brief Bind the texture
	 */
	void bind() const;

	/**
	 * \brief Bind the texture and activate texture unit for use with a specific
	 * shader. \param typeIndex Texture index of this texture type \param
	 * textureUnit The texture unit id \param shader Shader which will use the
	 * texture
	 */
	void bind(int typeIndex, GLuint textureUnit, PhongShader& shader);
	void dispose();

	void setClampToEdge();

private:
	GLuint loadTexture(const char* path);
};
} // namespace Vp