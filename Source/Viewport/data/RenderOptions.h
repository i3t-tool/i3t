#pragma once

#include "glm/vec3.hpp"

namespace Vp
{
/**
 * Options specifying how a scene should be rendered.
 */
class RenderOptions
{
public:
	bool multisample;
	unsigned int samples;
	bool framebufferAlpha;
	bool wboit;
	int wboitFunc;
	bool selection;

	glm::vec3 clearColor;

	RenderOptions();
};

} // namespace Vp
