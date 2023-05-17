#include "DepthAttachment.h"

#include "Logger/Logger.h"

using namespace Vp;

DepthAttachment::DepthAttachment(bool stencil, GLsizei width, GLsizei height, bool useRenderbuffer)
    : m_stencil(stencil), m_width(width), m_height(height)
{
	m_useRenderbuffer = useRenderbuffer;
}

DepthAttachment::DepthAttachment(const DepthAttachment& attchmt)
    : m_multisampled(attchmt.m_multisampled), m_samples(attchmt.m_samples),
      m_useRenderbuffer(attchmt.m_useRenderbuffer), m_stencil(attchmt.m_stencil), m_width(attchmt.m_width),
      m_height(attchmt.m_height), m_minFilter(attchmt.m_minFilter), m_magFilter(attchmt.m_magFilter),
      m_textureWrapS(attchmt.m_textureWrapS), m_textureWrapT(attchmt.m_textureWrapT), m_syncSize(attchmt.m_syncSize)
{
	// Empty
}

void DepthAttachment::create()
{
	if (m_useRenderbuffer)
	{
		glGenRenderbuffers(1, &m_id);
	}
	else
	{
		glGenTextures(1, &m_id);
	}
	resize(m_width, m_height);
}

void DepthAttachment::resize(int width, int height)
{
	this->m_width = width;
	this->m_height = height;

	// TODO: (DR) It might be a good idea to add various error handling from the opengl spec, checking max sample counts
	//  for example

	if (m_useRenderbuffer)
	{
		glBindRenderbuffer(GL_RENDERBUFFER, m_id);
		if (m_multisampled)
		{
			glRenderbufferStorageMultisample(GL_RENDERBUFFER, m_samples, m_stencil ? GL_DEPTH24_STENCIL8 : GL_DEPTH_COMPONENT,
			                                 m_width, m_height);
		}
		else
		{
			glRenderbufferStorage(GL_RENDERBUFFER, m_stencil ? GL_DEPTH24_STENCIL8 : GL_DEPTH_COMPONENT, m_width, m_height);
		}
		glBindRenderbuffer(GL_RENDERBUFFER, 0);
	}
	else
	{
		// TODO: (DR)(NOTE) glTexImage2DMultisample is only available from OpenGL 3.2+, might be an issue
		if (m_multisampled)
		{
			glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, m_id);
			glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, m_samples,
			                        m_stencil ? GL_DEPTH24_STENCIL8 : GL_DEPTH_COMPONENT, width, height, GL_TRUE);
			glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, 0);
			// Note: Multisampled texture filtering is always NEAREST
		}
		else
		{
			glBindTexture(GL_TEXTURE_2D, m_id);
			glTexImage2D(GL_TEXTURE_2D, 0, m_stencil ? GL_DEPTH24_STENCIL8 : GL_DEPTH_COMPONENT, width, height, 0,
			             m_stencil ? GL_DEPTH_STENCIL : GL_DEPTH_COMPONENT24,
			             m_stencil ? GL_UNSIGNED_INT_24_8 : GL_UNSIGNED_INT, NULL);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, m_minFilter);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, m_magFilter);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, m_textureWrapS);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, m_textureWrapT);
			glBindTexture(GL_TEXTURE_2D, 0);
		}
	}
}

void DepthAttachment::bind() const
{
	if (m_useRenderbuffer)
	{
		glBindRenderbuffer(GL_RENDERBUFFER, m_id);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, m_stencil ? GL_DEPTH_STENCIL_ATTACHMENT : GL_DEPTH_ATTACHMENT,
		                          GL_RENDERBUFFER, m_id);
		glBindRenderbuffer(GL_RENDERBUFFER, 0);
	}
	else
	{
		glFramebufferTexture2D(GL_FRAMEBUFFER, m_stencil ? GL_DEPTH_STENCIL_ATTACHMENT : GL_DEPTH_ATTACHMENT,
		                       (m_multisampled ? GL_TEXTURE_2D_MULTISAMPLE : GL_TEXTURE_2D), m_id, 0);
	}
}

void DepthAttachment::dispose()
{
	if (m_id)
	{
		if (m_useRenderbuffer)
		{
			glDeleteRenderbuffers(1, &m_id);
		}
		else
		{
			glDeleteTextures(1, &m_id);
		}
		m_id = 0;
	}
}
