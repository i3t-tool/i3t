/**
 * \file
 * \brief Declares some math functions
 * \authors Dan Rakušan <rakusan.dan@gmail.com>
 * \copyright Copyright (C) 2016-2025 I3T team, Department of Computer Graphics
 * and Interaction, FEE, Czech Technical University in Prague, Czech Republic
 *
 * This file is part of I3T - An Interactive Tool for Teaching Transformations
 * http://www.i3t-tool.org
 *
 * GNU General Public License v3.0 (see LICENSE.txt or https://www.gnu.org/licenses/gpl-3.0.txt)
 */
#pragma once

#include <array>

#include <glm/matrix.hpp>

namespace ProjectionUtils
{
/**
 * Decomposes the standard OpenGL perspective projection matrix into 6 frustum parameters.
 * m[0,0] = 2n/(R-L)
 * m[1,1] = 2n/(T-B)
 * m[2,0] = (R+L)/(R-L)
 * m[2,1] = (T+B)/(T-B)
 * m[2,2] = -(f+n)/(f-n) = A    (-1 for infinity f)
 * m[3,2] = -(2fn)/(f-n) = B    (-2n for infinity f)
 * m[2,3] = -1
 * @param m The perspective matrix
 * @return Returns array of L,R,B,T,N,F
 */
std::array<float, 6> decomposePerspective(const glm::mat4& m);
void extractZNearZFar(const glm::mat4& projectionMatrix, float& zNear, float& zFar);

std::array<glm::mat4, 2> constructZFlippedProjection(const glm::mat4& m);

/**
 * Decomposition of an OpenGL perspective matrix into 3 submatrices described in the book Fundamentals of computer
 * graphics, chapter 7 pg. 150. (SHIRLEY, Peter; ASHIKHMIN, Michael; MARSCHNER, Steve.)
 */
std::array<glm::mat4, 3> decomposePerspectiveShirley(const glm::mat4& m);

/**
 * Decomposition of an OpenGL perspective matrix into 4 submatrices described by Wayne Brown at
 * https://learnwebgl.brown37.net/08_projections/projections_perspective.html#the-perspective-calculation
 */
std::array<glm::mat4, 5> decomposePerspectiveBrown(const glm::mat4& m);

glm::vec4 divide(const glm::vec4& v);

inline glm::mat4 viewport(float x, float y, float width, float height, float n, float f)
{
	float widthDiv2 = width / 2.f;
	float heightDiv2 = height / 2.f;
	return glm::mat4({{widthDiv2, 0, 0, 0},
	                  {0, heightDiv2, 0, 0},
	                  {0, 0, (f - n) / 2.f, 0},
	                  {x + widthDiv2, y + heightDiv2, (f + n) / 2.f, 1}});
}

}; // namespace ProjectionUtils
