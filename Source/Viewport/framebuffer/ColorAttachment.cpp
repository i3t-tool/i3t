#include "ColorAttachment.h"

#include "Logger/Logger.h"

namespace Vp
{
ColorAttachment::ColorAttachment(unsigned int index, GLint internalFormat, GLint format, GLsizei width, GLsizei height,
                                 GLenum type, bool multisampled, unsigned int samples)
    : m_index(index), m_multisampled(multisampled), m_samples(samples), m_format(format),
      m_internalFormat(internalFormat), m_width(width), m_height(height), m_type(type)
{
	// Empty
}

ColorAttachment::ColorAttachment(GLint internalFormat, GLint format, GLsizei width, GLsizei height, GLenum type)
    : m_format(format), m_internalFormat(internalFormat), m_width(width), m_height(height), m_type(type)
{
	// Empty
}

ColorAttachment::ColorAttachment(const ColorAttachment& attchmt)
    : m_index(attchmt.m_index), m_multisampled(attchmt.m_multisampled), m_samples(attchmt.m_samples),
      m_format(attchmt.m_format), m_internalFormat(attchmt.m_internalFormat), m_width(attchmt.m_width),
      m_height(attchmt.m_height), m_type(attchmt.m_type), syncSizeWithFramebuffer(attchmt.syncSizeWithFramebuffer),
      m_texture(attchmt.m_texture)
{
}

void ColorAttachment::create()
{
	glGenTextures(1, &m_texture);
	resize(m_width, m_height);
}

void ColorAttachment::resize(int width, int height)
{
	this->m_width = width;
	this->m_height = height;

	// TODO: (DR) glTexImage2DMultisample is only available from OpenGL 3.2+, might be an issue
	if (m_multisampled)
	{
		glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, m_texture);
		glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, m_samples, m_internalFormat, width, height, GL_TRUE);
		glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, 0);
	}
	else
	{
		glBindTexture(GL_TEXTURE_2D, m_texture);
		glTexImage2D(GL_TEXTURE_2D, 0, m_internalFormat, width, height, 0, m_format, m_type, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glBindTexture(GL_TEXTURE_2D, 0);
	}
}

void ColorAttachment::bind() const
{
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + m_index,
	                       (m_multisampled ? GL_TEXTURE_2D_MULTISAMPLE : GL_TEXTURE_2D), m_texture, 0);
}

void ColorAttachment::dispose()
{
	if (m_texture)
	{
		glDeleteTextures(1, &m_texture);
		m_texture = 0;
	}
}
} // namespace Vp
