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

#include <algorithm>
#include <vector>

#include <glm/glm.hpp>

namespace Vp
{
namespace GfxUtils
{

inline std::pair<glm::vec3, glm::vec3> createBoundingBox(std::vector<glm::vec3> points)
{
	auto xExtremes = std::minmax_element(points.begin(), points.end(), [](const glm::vec3& lhs, const glm::vec3& rhs) {
		return lhs.x < rhs.x;
	});
	auto yExtremes = std::minmax_element(points.begin(), points.end(), [](const glm::vec3& lhs, const glm::vec3& rhs) {
		return lhs.y < rhs.y;
	});
	auto zExtremes = std::minmax_element(points.begin(), points.end(), [](const glm::vec3& lhs, const glm::vec3& rhs) {
		return lhs.z < rhs.z;
	});

	glm::vec3 min(xExtremes.first->x, yExtremes.first->y, zExtremes.first->z);
	glm::vec3 max(xExtremes.second->x, yExtremes.second->y, zExtremes.second->z);
	return std::make_pair(min, max);
}

inline glm::vec4 computePlane(glm::vec3 normal, glm::vec3 point)
{
	normal = glm::normalize(normal);
	return glm::vec4(normal.x, normal.y, normal.z, -glm::dot(normal, point));
}

inline glm::vec3 computeNormal(glm::vec3 a, glm::vec3 b, glm::vec3 c)
{
	glm::vec3 dir = glm::cross((b - a), (c - a));
	return glm::normalize(dir);
}

} // namespace GfxUtils
} // namespace Vp
