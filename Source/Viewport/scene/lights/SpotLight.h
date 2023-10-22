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

#include "Light.h"

namespace Vp
{
class SpotLight : public Light
{
public:
	float radius = 70.0f;
	float cutoffAngle = 10;
	float cutoffSoftAngle = 6;
	glm::vec3 direction = glm::vec3(0, 0, -1);

	void setUniforms(const PhongShader& shader, int index) const override;
};
} // namespace Vp