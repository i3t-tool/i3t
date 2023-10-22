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

#include "WorldShader.h"

namespace Vp
{
class ObjectShader : public WorldShader
{
protected:
	GLint opacityId;

public:
	float m_opacity{1.0f};

	explicit ObjectShader(GLuint id);

	void init(bool initSuperclass) override;
	void setUniforms() override;
};

} // namespace Vp
