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

#include <glm/glm.hpp>

namespace Vp
{
/**
 * Static utility class for trackball interaction
 */
class Trackball
{
public:
	/**
	 * Resolves trackball movement from one point to another and returns a corresponding rotation matrix
	 * @param screenStart Start point in range (-1, 1) or (0, screenSize)
	 * @param screenEnd End point in range (-1, 1) or (0, screenSize)
	 * @param screenSize Optionally screen size
	 * @return Immediate rotation matrix
	 */
	static glm::mat4 rotate(glm::vec2 screenStart, glm::vec2 screenEnd, glm::ivec2 screenSize = glm::ivec2(1, 1));

	// For debug purposes (trackball size visualization)
	static glm::vec2 debug_trackballScreenSize;

private:
	static void computeRotationAxisAndAngle(glm::vec3& axis, float& angle, glm::vec2 startPoint, glm::vec2 endPoint,
	                                        glm::ivec2 screenSize);

	static glm::mat4 computeRotation(glm::vec2 startPoint, glm::vec2 endPoint, glm::ivec2 screenSize);

	static glm::vec2 mapScreenCoords(glm::vec2 screenPos, glm::ivec2 screenSize);
	static glm::vec2 mapNormalizedCoords(glm::vec2 pos, glm::ivec2 screenSize);

	/**
	 * Project an x,y pair onto a sphere of radius or a hyperbolic sheet if we are away from the center of the sphere.
	 */
	static float projectToSphere(float radius, float x, float y);
};

} // namespace Vp
