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

#include "Shader.h"

namespace Vp
{
/**
 * Two pass box blur with alpha support
 */
class BoxBlurShader : public Shader
{
protected:
	GLint m_sourceSampler;

	GLint m_kernelSizeId;
	GLint m_verticalId;
	GLint m_resolutionId;

public:
	GLuint m_sourceTextureId{0};

	int m_kernelSize{10};   ///< Size of the blur kernel in pixels
	bool m_vertical{true};  ///< Vertical or horizontal blur pass
	glm::vec2 m_resolution; ///< Resolution of the current buffer in pixels

	explicit BoxBlurShader(GLuint id);

	void init(bool initSuperclass) override;
	void setUniforms() override;
};
} // namespace Vp
