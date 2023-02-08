#include "DepthAttachment.h"

#include "Logger/Logger.h"

using namespace Vp;
DepthAttachment::DepthAttachment(GLint internalFormat, bool stencil, GLsizei width, GLsizei height)
    : m_internalFormat(internalFormat), m_stencil(stencil), m_width(width), m_height(height)
{
	// Empty
}

void DepthAttachment::create()
{
	glGenRenderbuffers(1, &m_id);
	resize(m_width, m_height);
}

void DepthAttachment::resize(int width, int height)
{
	this->m_width = width;
	this->m_height = height;

	glBindRenderbuffer(GL_RENDERBUFFER, m_id);
	if (m_multisampled)
	{
		glRenderbufferStorageMultisample(GL_RENDERBUFFER, m_samples, m_internalFormat, m_width, m_height);
	}
	else
	{
		glRenderbufferStorage(GL_RENDERBUFFER, m_internalFormat, m_width, m_height);
	}
	glBindRenderbuffer(GL_RENDERBUFFER, 0);
}

void DepthAttachment::bind() const
{
	glBindRenderbuffer(GL_RENDERBUFFER, m_id);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, m_stencil ? GL_DEPTH_STENCIL_ATTACHMENT : GL_DEPTH_ATTACHMENT,
	                          GL_RENDERBUFFER, m_id);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);
}

void DepthAttachment::dispose()
{
	if (m_id)
	{
		glDeleteRenderbuffers(1, &m_id);
		m_id = 0;
	}
}
