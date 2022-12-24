/**
 * \file        Source/Viewport/framebuffer/Framebuffer.h
 * \author      Dan Rakušan <rakusdan(at)fit.cvut.cz>
 * \date        25.10.2022
 *
 * A utility class for managing an OpenGL FrameBuffer Object.
 */

#pragma once

#include <memory>
#include <optional>
#include <vector>

#include "pgr.h"

#include "ColorAttachment.h"
#include "Viewport/framebuffer/DepthAttachment.h"

#define FBO_DEFAULT_WIDTH 100
#define FBO_DEFAULT_HEIGHT 100

namespace Vp
{
/**
 * \brief A utility class for managing an OpenGL FrameBufferObject.
 *
 * Usage:\n
 * - //Create fbo object\n
 * - fbo.start(); //The fbo is lazily initialized here if necessary\n
 * - //Render stuff\n
 * - fbo.end(); //<b>The fbo is unbound by BINDING THE DEFAULT (0)
 * FRAMEBUFFER!</b>\n
 * - //Multisampling is resolved here unless fbo.end(false) is called\n
 * - //Get color texture using fbo.getColorTexture()\n
 * <br>
 * Supports multi sampled antialiasing (MSAA) which can be enabled in the
 * constructor. When MSAA is enabled, the framebuffer creates a second intermediate framebuffer instance which is used
 * to "resolve" the multisampled framebuffer into a regular one.
 * <br><br>
 * Multisampled framebuffers with multiple color
 * attachments are also supported. In that case the intermediate framebuffer creates an equivalent singlesampled color
 * attachment for each multisampled one.
 * <br><br>
 * Note that the framebuffer sampling settings overwrite any attachment
 * settings as all attachments must have the same sample count and either be all multisampled or none.
 */
class Framebuffer
{
private:
	int m_width{FBO_DEFAULT_WIDTH};   ///< Buffer pixel width
	int m_height{FBO_DEFAULT_HEIGHT}; ///< Buffer pixel height
	bool m_multisample{false};        ///< Enable MultiSample AntiAliasing
	unsigned int m_samples{4};        ///< MSAA sample count

	GLuint m_fbo{0}; ///< The created framebuffer object

	/// An intermediate single sample FBO used for resolving the multisampled one
	std::unique_ptr<Framebuffer> m_multisampleResolveFBO;

	/// List of all attached color attachments
	std::vector<ColorAttachment> m_colorAttachments;

	/// Depth attachment, can contain a stencil buffer
	std::optional<DepthAttachment> m_depthAttachment;

public:
	/**
	 * Create a basic framebuffer with a single color attachment and a depth-stencil buffer.
	 *
	 * @param multisample Whether MSAA should be enabled
	 * @param samples Amount of MSAA samples
	 * @param alpha Whether the color buffer should have an alpha component
	 * @return Raw unmanaged pointer to the newly created Framebuffer.
	 */
	static Framebuffer* createDefault(bool multisample, unsigned int samples, bool alpha);

	/**
	 * Create a basic framebuffer with a single color attachment and a depth-stencil buffer.
	 * @param width Width of the framebuffer in pixels
	 * @param height Height of the framebuffer in pixels
	 * @param multisample Whether MSAA should be enabled
	 * @param samples Amount of MSAA samples
	 * @param alpha Whether the color buffer should have an alpha component
	 * @return Raw unmanaged pointer to the newly created Framebuffer.
	 */
	static Framebuffer* createDefault(int width, int height, bool multisample, unsigned int samples, bool alpha);

	/**
	 * Create an empty framebuffer.
	 */
	Framebuffer();

	/**
	 * Create an empty framebuffer.
	 * @param multisample Whether MSAA should be enabled
	 * @param samples Amount of MSAA samples
	 */
	Framebuffer(bool multisample, unsigned int samples);

	/**
	 * Create an empty framebuffer.
	 * @param width Width of the framebuffer in pixels
	 * @param height Height of the framebuffer in pixels
	 */
	Framebuffer(int width, int height);

	/**
	 * Create an empty framebuffer.
	 * @param width Width of the framebuffer in pixels
	 * @param height Height of the framebuffer in pixels
	 * @param multisample Whether MSAA should be enabled
	 * @param samples Amount of MSAA samples
	 */
	Framebuffer(int width, int height, bool multisample, unsigned int samples);
	~Framebuffer();

	/**
	 * Binds the FBO, clears it and resizes glViewport.
	 * The FBO is also initialised if necessary.
	 * Uses previously set width and height.
	 * <b>Don't forget to call glClear after this call!</b>
	 * <b>glViewport is set to the fbo's dimensions.</b>
	 */
	void start();

	/**
	 * Binds the FBO and resizes glViewport.
	 * The FBO is also initialised if necessary.\n
	 * <b>Don't forget to call glClear after this call!</b>
	 * <b>glViewport is set to the fbo's dimensions.</b>
	 */
	void start(int width, int height);

	/**
	 * Finalizes the FBO and unbinds it.\n
	 * <b>The DEFAULT (0) frame buffer is bound after this call!</b>\n
	 * \param resolveMultisample If multisample and this argument is true this call resolves the multisampled buffer
	 * into an intermediate regular (single sampled) one. Whose color texture is then returned via getColorTexture()
	 */
	void end(bool resolveMultisample = true);

	/**
	 * Ensure the fbo has the specified size.
	 * Fbo is initialized and/or resized accordingly.
	 * Has no effect if fbo is already initialized and sized as requested.
	 */
	void update(int width, int height);

	/**
	 * Resize the fbo to the specified dimensions.
	 * Has no effect if the fbo is already that size.
	 */
	void resize(int width, int height);

	void bind() const;

	/**
	 * Returns the id of the color texture.
	 * \param index Optional index of the color attachment
	 * \param multisampled Optional flag specifying if the multisampled or resolved single sampled texture should be
	 * returned. If multisampling is disabled the single sampled texture will be always returned.
	 */
	GLuint getColorTexture(unsigned int index = 0, bool multisampled = true) const;

	/**
	 * @return The id of the Frame Buffer Object
	 */
	GLuint getId() const;

	bool isInitialized() const;

	bool isMultisampled();
	void setMultisampled(bool multisample, unsigned int samples);

	// TODO: (DR) Docs
	// MSAA Resolve methods
	void multisampleResolveColors();
	void multisampleResolveColor(unsigned int colorAttachmentIndex);

	/**
	 * Add a color attachment.
	 * Color attachment index is set to its current order in the color attachments list.
	 * Multisampling settings are synced with the framebuffer multisampling settings.
	 * @param colorAttachment
	 */
	void addColorAttachment(ColorAttachment colorAttachment);

	ColorAttachment& getColorAttachment(unsigned int index);
	void removeAllColorAttachments();

	/**
	 * Set a depth attachment.
	 * Multisampling settings are synced with the framebuffer multisampling settings.
	 * @param depthAttachment
	 */
	void setDepthAttachment(DepthAttachment depthAttachment);
	std::optional<DepthAttachment>& getDepthAttachment();
	void removeDepthAttachment();

	/**
	 * Frees up all OpenGL resources used by the framebuffer.
	 * The framebuffer is considered uninitialized after this call.
	 */
	void dispose();

	/**
	 * Completely reinitializes the framebuffer.
	 * All associated OpenGL resources are freed and can no longer be used.
	 * New resources are allocated.
	 */
	void recreate();

private:
	/**
	 * Initialise the fbo. Does NOT check if initialised previously.
	 */
	void initImpl(int width, int height);

	void setDrawBuffers();
	bool checkFramebuffer();
};
} // namespace Vp
