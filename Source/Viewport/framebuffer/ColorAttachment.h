#pragma once

#include "pgr.h"

namespace Vp
{
/**
 * Color attachment using a 2D Texture
 */
class ColorAttachment
{
public:
	unsigned int m_index{0}; ///< Index of the color attachment

	bool m_multisampled{false}; ///< Use MSAA texture
	unsigned int m_samples{4};  ///< Number of MSAA samples if multisampled

	GLint m_format;         ///< Pixel data format (see OpenGL docs)
	GLint m_internalFormat; ///< Pixel data internal format (see OpenGL docs)

	int m_width;  ///< Texture width in pixels
	int m_height; ///< Texture height in pixels

	GLenum m_type; ///< Pixel data type (see OpenGL docs)

	GLint m_minFilter{GL_NEAREST};
	GLint m_magFilter{GL_NEAREST};

	GLint m_textureWrapS{GL_REPEAT};
	GLint m_textureWrapT{GL_REPEAT};

	// TODO: (DR) Test / Implement, generally its easier to use a second FBO
	bool m_syncSize{true};

	GLuint m_texture{0}; ///< ID of the created texture

	/**
	 * Creates a color attachment with index 0.
	 * Parameters mimic an OpenGL image texture color attachment.
	 */
	ColorAttachment(GLint internalFormat, GLint format, GLsizei width, GLsizei height, GLenum type);

	/**
	 * Copy constructor
	 */
	ColorAttachment(const ColorAttachment& attchmt);

	void create();
	void resize(int width, int height);
	void bind() const;
	void dispose();
};

} // namespace Vp
