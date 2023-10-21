/**
 * \file
 * \brief
 * \author Dan Rakušan <rakusan.dan@gmail.com>
 * \copyright Copyright (C) 2016-2023 I3T team, Department of Computer Graphics
 * and Interaction, FEE, Czech Technical University in Prague, Czech Republic
 *
 * This file is part of I3T - An Interactive Tool for Teaching Transformations
 * http://www.i3t-tool.org
 *
 * GNU General Public License v3.0 (see LICENSE.txt or https://www.gnu.org/licenses/gpl-3.0.txt)
 */
#include "ColorAttachment.h"

#include "Logger/Logger.h"

using namespace Vp;

ColorAttachment::ColorAttachment(GLint internalFormat, GLint format, GLsizei width, GLsizei height, GLenum type)
    : m_format(format), m_internalFormat(internalFormat), m_width(width), m_height(height), m_type(type)
{
	// Empty
}

// Copy constructor
ColorAttachment::ColorAttachment(const ColorAttachment& attchmt)
    : m_index(attchmt.m_index), m_multisampled(attchmt.m_multisampled), m_samples(attchmt.m_samples),
      m_format(attchmt.m_format), m_internalFormat(attchmt.m_internalFormat), m_width(attchmt.m_width),
      m_height(attchmt.m_height), m_type(attchmt.m_type), m_minFilter(attchmt.m_minFilter),
      m_magFilter(attchmt.m_magFilter), m_textureWrapS(attchmt.m_textureWrapS), m_textureWrapT(attchmt.m_textureWrapT),
      m_syncSize(attchmt.m_syncSize)
{
	// Empty
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

	// TODO: (DR)(NOTE) glTexImage2DMultisample is only available from OpenGL 3.2+, might be an issue
	if (m_multisampled)
	{
		glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, m_texture);
		glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, m_samples, m_internalFormat, width, height, GL_TRUE);
		glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, 0);
		// Note: Multisampled texture filtering is always NEAREST
	}
	else
	{
		glBindTexture(GL_TEXTURE_2D, m_texture);
		glTexImage2D(GL_TEXTURE_2D, 0, m_internalFormat, width, height, 0, m_format, m_type, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, m_minFilter);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, m_magFilter);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, m_textureWrapS);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, m_textureWrapT);
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
