/**
 * \file
 * \brief
 * \authors Michal Folta, Petr Felkel, Martin Herich
 * \copyright Copyright (C) 2016-2023 I3T team, Department of Computer Graphics
 * and Interaction, FEE, Czech Technical University in Prague, Czech Republic
 *
 * This file is part of I3T - An Interactive Tool for Teaching Transformations
 * http://www.i3t-tool.org
 *
 * GNU General Public License v3.0 (see LICENSE.txt or https://www.gnu.org/licenses/gpl-3.0.txt)
 */
// crazy format error on #
#include "Utils/Math.h"

namespace Math
{
glm::mat4 lerp(const glm::mat4& lhs, const glm::mat4& rhs, float alpha, bool useQuat)
{
	glm::mat4 result;

	if (!useQuat)
	{
		result[0] = glm::mix(lhs[0], rhs[0], alpha);
		result[1] = glm::mix(lhs[1], rhs[1], alpha);
		result[2] = glm::mix(lhs[2], rhs[2], alpha);
		result[3] = glm::mix(lhs[3], rhs[3], alpha);
	}
	else
	{
		// rotation part mat3x3
		auto q1 = glm::quat_cast(lhs);
		auto q2 = glm::quat_cast(rhs);
		result = glm::mat4_cast(glm::mix(q1, q2, alpha));

		// translation part
		glm::vec4 r = glm::mix(lhs[3], rhs[3], alpha);
		result[3] = r;
	}

	return result;
}
} // namespace Math
