#pragma once
#include <string>
#include <vector>

#include "Texture.h"

namespace Vp
{
class Cubemap
{
public:
	GLuint id;

	Cubemap(std::vector<std::string> textures);

	void bind(GLuint location);

private:
	GLuint loadTextures(std::vector<std::string> textures);
};
} // namespace Vp