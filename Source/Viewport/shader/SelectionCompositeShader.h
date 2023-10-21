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
class SelectionCompositeShader : public Shader
{
protected:
	GLint m_sourceSampler;

	GLint m_resolutionId;
	GLint m_cutoffId;

public:
	GLuint m_sourceTextureId{0};

	glm::vec2 m_resolution{glm::vec2(1.0f)};
	float m_cutoff{0.2f};

	explicit SelectionCompositeShader(GLuint id);

	void init(bool initSuperclass) override;
	void setUniforms() override;
};
} // namespace Vp
