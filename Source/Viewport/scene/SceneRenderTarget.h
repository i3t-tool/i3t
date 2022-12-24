#pragma once

#include <memory>
#include <vector>

#include "Viewport/framebuffer/Framebuffer.h"
#include "Viewport/scene/RenderOptions.h"

namespace Vp
{
/**
 * A set of framebuffers and their settings into which a Scene can draw.
 * This is a separate class to allow rendering a single scene with different settings
 * (requiring framebuffer(s) with different settings)
 */
class SceneRenderTarget
{
private:
	std::vector<std::shared_ptr<Framebuffer>> m_framebuffers;
	std::weak_ptr<Framebuffer> outputFramebuffer;

	RenderOptions renderOptions;

public:
	std::weak_ptr<Framebuffer> getOutputFramebuffer();
	void setOutputFramebuffer(std::weak_ptr<Framebuffer> framebuffer);

	std::weak_ptr<Framebuffer> getFramebuffer(unsigned int index);
	void addFramebuffer(std::shared_ptr<Framebuffer> framebuffer);

	const RenderOptions& getRenderOptions() const;
	void setRenderOptions(const RenderOptions& renderOptions);
};

} // namespace Vp
