/**
 * \file        Source\Viewport\Framebuffer.h
 * \author      Dan Rakušan <rakusdan@fit.cvut.cz>
 * \date        25.10.2022
 *
 * A utility class for managing an OpenGL FrameBufferObject.
 */

#pragma once

#include <memory>

#include "pgr.h"

// TODO: (DR) Probably add option to not use stencil buffer
// TODO: (DR) The MSAA intermediate buffer probably doesn't even need the depth
// buffer

namespace Vp
{
/**
 * \brief A utility class for managing an OpenGL FrameBufferObject.
 *
 * Usage:\n
 * - //Create fbo object\n
 * - fbo.start(); //The fbo is lazily initialized here if necessary\n
 * - //Render stuff\n
 * - fbo.end(); //<b>The fbo unbound by BINDING THE DEFAULT (0)
 * FRAMEBUFFER!</b>\n
 * - //Get color texture using fbo.getColorTexture()\n
 * <br>
 * Supports multi sampled antialiasing (MSAA) which can be enabled in the
 * constructor. When MSAA is enabled, the framebuffer creates a second
 * intermediate framebuffer instance which is used to "resolve" the multisampled
 * framebuffer into a regular one.
 */
class Framebuffer
{
private:
	int m_width;              ///< Buffer pixel width
	int m_height;             ///< Buffer pixel height
	bool m_multisample;       ///< Enable MultiSample AntiAliasing
	int m_samples{4};         ///< MSAA sample count
	bool m_alpha;             ///< Store alpha component
	GLuint m_colorTexture;    ///< Texture attached to GL_COLOR_ATTACHMENT0
	GLuint m_depthStencilRbo; ///< DepthStencil renderbuffer
	GLuint m_fbo{0};          ///< The created framebuffer object

	/// An intermediate single sample FBO used for resolving the multisampled one
	std::unique_ptr<Framebuffer> m_multisampleResolveFBO;

public:
	Framebuffer(int width, int height);
	Framebuffer(int width, int height, bool multisample);
	Framebuffer(int width, int height, bool multisample, bool alpha);
	~Framebuffer();

	/**
	 * Binds the FBO, clears it and resizes glViewport.
	 * The FBO is also initialised if necessary.
	 * Uses previously set width and height.
	 */
	void start();

	/**
	 * Binds the FBO and resizes glViewport.
	 * The FBO is also initialised if necessary.\n
	 * <b>Don't forget to call glClear after this call</b>
	 */
	void start(int width, int height);

	/**
	 * Finalizes the FBO and unbinds it.\n
	 * <b>The DEFAULT (0) frame buffer is bound after this call!</b>\n
	 * If multisample is enabled this call resolves the multisampled buffer
	 * into an intermediate regular (single sampled) one. Whose color texture is
	 * then returned via getColorTexture()
	 */
	void end();

	bool isInitialized() const;

	/**
	 * @return The id of the color texture
	 */
	GLuint getColorTexture() const;

	/**
	 * @return The id of the Frame Buffer Object
	 */
	GLuint getId() const;

private:
	/**
	 * Initialise the fbo. Does NOT check if initialised previously.
	 */
	void init(int width, int height);

	/**
	 * Resize the fbo IF necessary.
	 */
	void resize(int width, int height);

	void createColorTexture(int width, int height);
	void createDepthStencilRenderBuffer(int width, int height);

	bool checkFramebuffer();
};
} // namespace Vp
