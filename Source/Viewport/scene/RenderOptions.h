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
	bool multisample{false};
	unsigned int samples{4};
	bool framebufferAlpha{false};
	bool wboit{false};

	glm::vec3 clearColor{0.2f, 0.2f, 0.2f};

	RenderOptions() = default;
	RenderOptions(bool multisample, int samples, bool framebufferAlpha, bool wboit);
};

} // namespace Vp
