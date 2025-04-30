/**
 * \file
 * \brief
 * \authors Dan Rakušan <rakusan.dan@gmail.com>
 * \copyright Copyright (C) 2016-2025 I3T team, Department of Computer Graphics
 * and Interaction, FEE, Czech Technical University in Prague, Czech Republic
 *
 * This file is part of I3T - An Interactive Tool for Teaching Transformations
 * http://www.i3t-tool.org
 *
 * GNU General Public License v3.0 (see LICENSE.txt or https://www.gnu.org/licenses/gpl-3.0.txt)
 */
#include "ProjectionUtils.h"

#include "Math.h"

namespace ProjectionUtils
{

void extractZNearZFar(const glm::mat4& projection, float& zNear, float& zFar)
{
	// Extract near and far values from projection matrix
	// Note that using this method there can be a relatively "HUGE" precision error
	// It might not suitable to use this method for something like comparing depth values (as near/far values will
	// shift)
	if (projection[2][3] == 0)
	{
		// Orthographic matrix
		float m32 = projection[3][2];
		float m22 = projection[2][2];
		zNear = (m32 + 1) / m22;
		zFar = (m32 - 1) / m22;
	}
	else
	{
		// Perspective matrix
		float m32 = projection[3][2];
		float m22 = projection[2][2];
		zNear = m32 / (m22 - 1);
		zFar = m32 / (m22 + 1);
	}
}

std::array<float, 6> decomposePerspective(const glm::mat4& m)
{
	float n;
	float f;
	extractZNearZFar(m, n, f);

	float bottom = n * (m[2][1] - 1) / m[1][1];
	float top = n * (m[2][1] + 1) / m[1][1];

	float left = n * (m[2][0] - 1) / m[0][0];
	float right = n * (m[2][0] + 1) / m[0][0];

	return {left, right, bottom, top, n, f};
}

std::array<glm::mat4, 2> constructZFlippedProjection(const glm::mat4& m)
{
	glm::mat4 negator(1.f);
	negator[2][2] = -1;
	return {negator, negator * m};
}

std::array<glm::mat4, 3> constructPiecewisePerspective(const glm::mat4& m)
{
	auto [l, r, b, t, n, f] = decomposePerspective(m);

	assert(Math::compare(glm::frustum(l, r, b, t, n, f), m, Math::FACTOR_ROUGHLY_SIMILAR));

	// -- perspective matrix from Shirley FCG p. 152
	// --'(has negative n and f)
	// -- |  n   0   0    0  |
	// -- |  0   n   0.   0  |
	// -- |  0   0  n+f  -fn |
	// -- |  0   0   1    0  |
	//
	// -- Modified for OpenGL with stored negative n and f as positive distances
	// -- | -n   0   0    0  |
	// -- |  0  -n   0.   0  |
	// -- |  0   0 -n-f  -fn |
	// -- |  0   0   1    0  |

	// self.operation = Operation.new(
	// 	-- inputs { name = type }
	// 	{ n = ValueType.Float ,  f = ValueType.Float },
	// 	-- outputs { name = type }
	// 	{ result = ValueType.Matrix }
	// )
	// self.on_init = function()
	// 	-- custom initialization code
	// end
	// self.on_update_values = function()
	// 	-- custom on update values code, access the node using
	// 	-- self.node reference.
	//   local operator = self.node:as_operator()
	//   if (operator:is_input_plugged(1) and operator:is_input_plugged(2)) then
	// 	local f = operator:get_input_float(1)
	// 	local n = operator:get_input_float(2)
	//
	// -- perspective matrix from Shirley FCG p. 152
	// -- operator:set_value(Mat4.new(Vec4.new(n, 0, 0, 0), Vec4.new(0, n, 0, 0), Vec4.new(0, 0, n + f, 1), Vec4.new(0,
	// 0, -f * n, 0)))
	//
	// -- Modified for OpenGL with stored negative n and f as positive distances
	// --  operator:set_value(Mat4.new(Vec4.new(-n, 0, 0, 0), Vec4.new(0, -n, 0, 0), Vec4.new(0, 0, -(n + f), 1),
	// Vec4.new(0, 0, -f * n, 0)))
	//
	// -- Modified for OpenGL times (-1)
	//    operator:set_value(Mat4.new(Vec4.new(n, 0, 0, 0), Vec4.new(0, n, 0, 0), Vec4.new(0, 0, (n + f), -1),
	//    Vec4.new(0, 0, f * n, 0)))
	//
	//   end
	// end

	// Mat4.new(Vec4.new(n, 0, 0, 0), Vec4.new(0, n, 0, 0), Vec4.new(0, 0, (n + f), -1), Vec4.new(0, 0, f * n, 0))
	//
	// Mat4.new(Vec4.new(n, 0, 0, 0), Vec4.new(0, n, 0, 0), Vec4.new(0, 0, (n + f), -1), Vec4.new(0, 0, f * n, 0))
	//

	// glm::mat4 ortho = {{2 / (r - l), 0, 0, 0},
	//                    {0, 2 / (t - b), 0, 0},
	//                    {0, 0, 2.f / (f - n), 0},
	//                    {(r + l) / (l - r), (t + b) / (b - t), (f + n) / (f - n), 1}};
	//
	// // glm::mat4 persp = {{n, 0, 0, 0}, {0, n, 0, 0}, {0, 0, n + f, -1}, {0, 0, f * n, 0}};
	// glm::mat4 persp = {{n, 0, 0, 0}, {0, n, 0, 0}, {0, 0, n + f, 1}, {0, 0, -f * n, 0}};

	// Shirley
	// glm::mat4 ortho = {{2 / (r - l), 0, 0, 0},
	//                    {0, 2 / (t - b), 0, 0},
	//                    {0, 0, 2.f / (n - f), 0},
	//                    {-(r + l) / (r - l), -(t + b) / (t - b), (f + n) / (f - n), 1}};
	//
	// // glm::mat4 persp = {{n, 0, 0, 0}, {0, n, 0, 0}, {0, 0, n + f, -1}, {0, 0, f * n, 0}};
	// glm::mat4 persp = {{n, 0, 0, 0}, {0, n, 0, 0}, {0, 0, n + f, 1}, {0, 0, -f * n, 0}};
	// glm::mat4 persp = {{n, 0, 0, 0}, {0, n, 0, 0}, {0, 0, n + f, -1}, {0, 0, f * n, 0}};
	// glm::mat4 persp = {{n, 0, 0, 0}, {0, n, 0, 0}, {0, 0, n + f, 1}, {0, 0, -f * n, 0}};

	glm::mat4 ortho = {{2 / (r - l), 0, 0, 0},
	                   {0, 2 / (t - b), 0, 0},
	                   {0, 0, -2.f / (f - n), 0},
	                   {-(r + l) / (r - l), -(t + b) / (t - b), -(f + n) / (f - n), 1}};
#ifndef NDEBUG
	glm::mat4 orthoGlm = glm::ortho(l, r, b, t, n, f);
	assert(Math::compare(ortho, orthoGlm));
#endif

	glm::mat4 persp = {{n, 0, 0, 0}, {0, n, 0, 0}, {0, 0, n + f, -1}, {0, 0, f * n, 0}};

	assert(Math::compare(ortho * persp, m, Math::FACTOR_ROUGHLY_SIMILAR));

	glm::mat4 neg(1.f);
	neg[2][2] = -1;

	ortho = neg * ortho;

	assert(Math::compare(neg * ortho * persp, m, Math::FACTOR_ROUGHLY_SIMILAR));

	return {neg, ortho, persp};
}

} // namespace ProjectionUtils
