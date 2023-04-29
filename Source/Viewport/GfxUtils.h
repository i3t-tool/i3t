#pragma once

#include <algorithm>
#include <vector>

#include <glm/glm.hpp>

namespace Vp
{
namespace GfxUtils
{
void extractZNearZFar(glm::mat4 projectionMatrix, float& zNear, float& zFar);

inline std::pair<glm::vec3, glm::vec3> createBoundingBox(std::vector<glm::vec3> points)
{
	auto xExtremes = std::minmax_element(points.begin(), points.end(),
	                                     [](const glm::vec3& lhs, const glm::vec3& rhs) { return lhs.x < rhs.x; });
	auto yExtremes = std::minmax_element(points.begin(), points.end(),
	                                     [](const glm::vec3& lhs, const glm::vec3& rhs) { return lhs.y < rhs.y; });
	auto zExtremes = std::minmax_element(points.begin(), points.end(),
	                                     [](const glm::vec3& lhs, const glm::vec3& rhs) { return lhs.z < rhs.z; });

	glm::vec3 min(xExtremes.first->x, yExtremes.first->y, zExtremes.first->z);
	glm::vec3 max(xExtremes.second->x, yExtremes.second->y, zExtremes.second->z);
	return std::make_pair(min, max);
}
} // namespace GfxUtils
} // namespace Vp
