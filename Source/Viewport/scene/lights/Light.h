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

#include "glm/glm.hpp"

namespace Vp
{
class PhongShader;

class Light
{
public:
	virtual ~Light() = default;

	glm::vec3 pos{glm::vec3(0.0f)};
	glm::vec3 color{glm::vec3(1)};
	glm::vec3 specular{glm::vec3(1)};
	float intensity{0.6f};
	virtual void setUniforms(const PhongShader& shader, int index) const = 0;
};
} // namespace Vp