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
#pragma once

#include "pgr.h"

namespace Vp
{
/**
 * Depth attachment using a Renderbuffer
 */
class DepthAttachment
{
public:
	bool m_multisampled{false}; ///< Use MSAA texture
	unsigned int m_samples{4};  ///< Number of MSAA samples if multisampled

	bool m_useRenderbuffer{true};

	bool m_stencil; ///< Whether the specified depth format includes a stencil buffer

	int m_width;  ///< Width in pixels
	int m_height; ///< Height in pixels

	GLint m_minFilter{GL_NEAREST};
	GLint m_magFilter{GL_NEAREST};

	GLint m_textureWrapS{GL_REPEAT};
	GLint m_textureWrapT{GL_REPEAT};

	// TODO: (DR) Not implemented, generally its easier to use a second FBO
	bool m_syncSize{true};

	GLuint m_id{0}; ///< ID of the created object (renderbuffer or texture)

	/**
	 * Creates a depth buffer using a 2D texture or read-only renderbuffer.
	 * \param useRenderbuffer Whether to use a renderbuffer or a 2D texture for storage.
	 */
	DepthAttachment(bool stencil, GLsizei width, GLsizei height, bool useRenderbuffer);

	/**
	 * Copy constructor // TODO: (DR)(CRIT) Update this
	 */
	DepthAttachment(const DepthAttachment& attchmt);

	void create();
	void resize(int width, int height);
	void bind() const;
	void dispose();
};

} // namespace Vp
