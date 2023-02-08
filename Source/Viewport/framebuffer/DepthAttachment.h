#pragma once

#include "pgr.h"

namespace Vp
{
class DepthAttachment
{
public:
	bool m_multisampled{false};    ///< Use MSAA texture
	unsigned int m_samples{4}; ///< Number of MSAA samples if multisampled

	GLenum m_internalFormat; ///< Depth format (see OpenGL docs)
	bool m_stencil;          ///< Whether the specified depth format includes a stencil buffer

	int m_width;  ///< Width in pixels
	int m_height; ///< Height in pixels

	bool syncSizeWithFramebuffer{true};

	GLuint m_id{0}; ///< ID of the created object

	DepthAttachment(GLint internalFormat, bool stencil, GLsizei width, GLsizei height);

	void create();
	void resize(int width, int height);
	void bind() const;
	void dispose();
};

} // namespace Vp
