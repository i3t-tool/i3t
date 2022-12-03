#include "Cubemap.h"

#include <iostream>

using namespace Vp;

Cubemap::Cubemap(std::vector<std::string> textures) { id = loadTextures(textures); }

void Cubemap::bind(GLuint location)
{
	glActiveTexture(GL_TEXTURE0);
	CHECK_GL_ERROR();
	glUniform1i(location, 0);
	CHECK_GL_ERROR();
	glBindTexture(GL_TEXTURE_CUBE_MAP, id);
	CHECK_GL_ERROR();
}

GLuint Cubemap::loadTextures(std::vector<std::string> textures)
{
	GLuint id;
	glGenTextures(1, &id);
	glBindTexture(GL_TEXTURE_CUBE_MAP, id);

	for (int i = 0; i < textures.size(); i++)
	{
		std::cout << "Loading cube map texture: " << textures[i] << std::endl;
		if (!pgr::loadTexImage2D(textures[i], GL_TEXTURE_CUBE_MAP_POSITIVE_X + i))
		{
			pgr::dieWithError("Skybox cube map loading failed!");
		}
	}

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	CHECK_GL_ERROR();

	return id;
}
