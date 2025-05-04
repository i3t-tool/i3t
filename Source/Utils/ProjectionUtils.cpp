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
	glm::mat4 negZ(1.f);
	negZ[2][2] = -1; // Flip Z (NDC is +z forward)
	glm::mat4 negX(1.f);
	negX[0][0] = -1; // Flip X (NDC ls LHS)
	return {negX * negZ, negZ * m};
}

std::array<glm::mat4, 3> decomposePerspectiveShirley(const glm::mat4& m)
{
	auto [l, r, b, t, n, f] = decomposePerspective(m);

#ifndef NDEBUG
	if (!Math::compare(glm::frustum(l, r, b, t, n, f), m, Math::FACTOR_ROUGHLY_SIMILAR))
	{
		LOG_WARN("[TRACKING] Parameter decomposition of the perspective matrix yielded a matrix not equal to the "
		         "original!\nThis can be ignored if the original matrix is invalid.");
	}
#endif

	glm::mat4 ortho = {{2 / (r - l), 0, 0, 0},
	                   {0, 2 / (t - b), 0, 0},
	                   {0, 0, -2.f / (f - n), 0},
	                   {-(r + l) / (r - l), -(t + b) / (t - b), -(f + n) / (f - n), 1}};
#ifndef NDEBUG
	glm::mat4 orthoGlm = glm::ortho(l, r, b, t, n, f);
	if (!Math::compare(ortho, orthoGlm))
	{
		LOG_WARN("[TRACKING] Shirley ortho matrix does not equal to the standard glm::ortho matrix\nThis can be "
		         "ignored if the provided perspective matrix is invalid.");
	}
#endif

	glm::mat4 persp = {{n, 0, 0, 0}, {0, n, 0, 0}, {0, 0, n + f, -1}, {0, 0, f * n, 0}};

#ifndef NDEBUG
	if (!Math::compare(ortho * persp, m, Math::FACTOR_ROUGHLY_SIMILAR))
	{
		LOG_WARN("[TRACKING] Shirley decomposition of the perspective matrix yielded a matrix not equal to the "
		         "original!\nThis can be ignored if the original matrix is invalid. (1)");
	}
#endif

	glm::mat4 neg(1.f);
	neg[2][2] = -1; // Flip Z (NDC is +z forward)

	ortho = neg * ortho;

#ifndef NDEBUG
	if (!Math::compare(neg * ortho * persp, m, Math::FACTOR_ROUGHLY_SIMILAR))
	{
		LOG_WARN("[TRACKING] Shirley decomposition of the perspective matrix yielded a matrix not equal to the "
		         "original!\nThis can be ignored if the original matrix is invalid. (2)");
	}
#endif

	// TODO: Figure this out, is this worth rewriting stuff to support dynamic RH/LH switch?
	// We have to flip X because we are using a RH coordinate system, but Ogl NDC is LH ... so it won't work nicely,
	// flipping the X and then "pretending" the X axis is flipped is a close "emulation"
	neg[0][0] = -1; // Flip X (NDC ls LHS)

	return {persp, ortho, neg};
}

std::array<glm::mat4, 5> decomposePerspectiveBrown(const glm::mat4& m)
{
	auto [l, r, b, t, n, f] = decomposePerspective(m);

#ifndef NDEBUG
	if (!Math::compare(glm::frustum(l, r, b, t, n, f), m, Math::FACTOR_ROUGHLY_SIMILAR))
	{
		LOG_WARN("[TRACKING] Parameter decomposition of the perspective matrix yielded a matrix not equal to the "
		         "original!\nThis can be ignored if the original matrix is invalid.");
	}
#endif

	// glm::mat4 ortho = {{2 / (r - l), 0, 0, 0},
	//                    {0, 2 / (t - b), 0, 0},
	//                    {0, 0, -2.f / (f - n), 0},
	//                    {-(r + l) / (r - l), -(t + b) / (t - b), -(f + n) / (f - n), 1}};

	glm::mat4 ortho1 = {{1, 0, 0, 0}, {0, 1, 0, 0}, {0, 0, 1, 0}, {-(l + r) / 2, -(b + t) / 2, 0, 1}};

	glm::mat4 persp1 = {{1, 0, 0, 0}, {0, 1, 0, 0}, {0, 0, ((f + n) / (f - n)), -1}, {0, 0, -2 * f * n / (n - f), 0}};

	glm::mat4 persp2 = {{n, 0, 0, 0}, {0, n, 0, 0}, {0, 0, 1, 0}, {0, 0, 0, 1}};

	glm::mat4 ortho2 = {{2 / (r - l), 0, 0, 0}, {0, 2 / (t - b), 0, 0}, {0, 0, 1, 0}, {0, 0, 0, 1}};

	glm::mat4 neg(1.f);
	neg[2][2] = -1; // Flip Z (NDC is +z forward)
	neg[0][0] = -1; // Flip X (NDC ls LHS)

	return {ortho1, persp1, persp2, ortho2, neg};
}

glm::vec4 divide(const glm::vec4& v)
{
	return v / v.w;
}

} // namespace ProjectionUtils
