//---------------------------------------------------------------------------
/**
 * \file	mathUtils.h
 *
 * Declares some math functions
 * \date  2014/11/16
 * \author Michal Folta, CTU Prague
 */
//---------------------------------------------------------------------------
#pragma once

#include <iostream>
#include <random>
#include <vector>

#include "glm/glm.hpp"
#include "glm/gtx/matrix_interpolation.hpp"
#include "glm/gtx/norm.hpp"

#include "Core/Defs.h"

namespace Math
{
static constexpr float FLT_EPSILON_10 = 10.0f * FLT_EPSILON; // 1.192093 E-6

FORCE_INLINE bool eq(const float lhs, const float rhs,
                     const float epsilon = FLT_EPSILON_10)
{
	return abs(lhs - rhs) < epsilon;
}

FORCE_INLINE bool eq(const glm::vec3& lhs, const glm::vec3& rhs,
                     const float epsilon = FLT_EPSILON_10)
{
	for (int i = 0; i < 3; ++i)
	{
		if (!Math::eq(lhs[i], rhs[i], epsilon))
		{
			return false;
		}
	}

	return true;
}

FORCE_INLINE bool eq(const glm::vec4& lhs, const glm::vec4& rhs,
                     const float epsilon = FLT_EPSILON_10)
{
	for (int i = 0; i < 4; ++i)
	{
		if (!Math::eq(lhs[i], rhs[i], epsilon))
		{
			return false;
		}
	}

	return true;
}

FORCE_INLINE bool eq(const glm::quat& lhs, const glm::quat& rhs,
                     const float epsilon = FLT_EPSILON_10)
{
	for (int i = 0; i < 4; ++i)
	{
		if (!Math::eq(lhs[i], rhs[i], epsilon))
		{
			return false;
		}
	}

	return true;
}

FORCE_INLINE bool eq(const glm::mat4& lhs, const glm::mat4& rhs,
                     const float epsilon = FLT_EPSILON_10)
{
	for (int i = 0; i < 4; ++i)
	{
		for (int j = 0; j < 4; ++j)
		{
			if (i == 1 && j == 2)
				std::cerr << "lhs[" << i << "], [" << j << "] = " << lhs[i][j]
				          << " != rhs[][] = " << rhs[i][j] << std::endl;
			if (!Math::eq(lhs[i][j], rhs[i][j], epsilon))
			{
				std::cerr << "lhs[" << i << "], [" << j << "] = " << lhs[i][j]
				          << " != rhs[][] = " << rhs[i][j] << " return false"
				          << std::endl;
				return false;
			}
		}
	}

	return true;
}

template <typename T> bool isNormalized(const T& val)
{
	return eq(glm::length2(val), 1.0f);
}

FORCE_INLINE bool areElementsSame(const glm::vec3 vec)
{
	return Math::eq(vec[0], vec[1]) && Math::eq(vec[1], vec[2]);
}

FORCE_INLINE bool withinInterval(float val, float from, float to)
{
	return from <= val && val <= to;
}

/**
 * \brief Linear interpolation between two vectors vec3 \a a and \a b
 * \param a
 * \param b
 * \param alpha interpolation parameter - must be from interval (0..1)
 * \return
 */
static glm::vec3 lerp(glm::vec3 a, glm::vec3 b, float alpha)
{
	glm::vec3 inter = a * (1.0f - alpha);
	inter = inter + b * alpha;
	return inter;
}

FORCE_INLINE glm::mat4 lerp(const glm::mat4& lhs, const glm::mat4& rhs,
                            float alpha, bool useQuat = false)
{
	if (!useQuat)
	{
		return glm::interpolate(lhs, rhs, alpha);
	}
	auto q1 = glm::quat_cast(lhs);
	auto q2 = glm::quat_cast(rhs);
	auto result = glm::mix(q1, q2, alpha);

	return glm::mat4_cast(result);
}

/**
 * \brief Linear interpolation between two vectors vec4 \a a and \a b
 * \param a
 * \param b
 * \param alpha interpolation parameter - must be from interval (0..1)
 * \return
 */
static glm::vec4 lerp(glm::vec4 a, glm::vec4 b, float alpha)
{
	glm::vec4 inter = a * (1.0f - alpha);
	inter.a = a.w;
	inter = inter + b * alpha;
	return inter;
}

/**
 * \brief Value base linear interpolation between two matrices \a a and \a b.
 * Incorrect, just for teaching purposes. \param a \param b \param alpha
 * interpolation parameter - must be from interval (0..1) \return
 */
static glm::mat4 lerp(glm::mat4 a, glm::mat4 b, float alpha)
{
	glm::mat4 res;

	res[0] = lerp(a[0], b[0], alpha);
	res[1] = lerp(a[1], b[1], alpha);
	res[2] = lerp(a[2], b[2], alpha);
	res[3] = lerp(a[3], b[3], alpha);

	return res;
}

static float smoothAlpha(const float a)
{
	return ((6 * a - 15) * a + 10) * a * a * a;
}

/**
 * \brief Random float from interval 0 .. 0.999
 * \return random float
 */
static float randomf() { return (float)((rand() % 1000) / 1000.0f); }

/**
 * \brief Random float from interval -0.5 .. +0.499
 * \return random float
 */
static float randomfHalf() { return -0.5f + randomf(); }

/**
 * \brief Random integer from interval 0..\a topNotInclude
 * \param topNotInclude
 * \return Random integer
 */
static int randomi(const int topNotInclude) { return rand() % topNotInclude; }

/**
 * \brief Random integer from interval \a downIncluded .. \a topNotInclude
 * \param downIncluded [min generated value
 * \param topNotInclude max) generated value
 * \return Random integer
 */
static int randomi(const int downIncluded, const int topNotInclude)
{
	return rand() % topNotInclude + downIncluded;
}

static glm::vec3 randomVec()
{
	return glm::normalize(glm::vec3(randomfHalf(), randomfHalf(), randomfHalf()));
}

static void prefixSum(std::vector<int>& arr)
{
	if (arr.empty())
		return;

	int tmp = arr[0];
	arr[0] = 0;
	for (unsigned int i = 1; i < arr.size(); i++)
	{
		int t = arr[i];
		arr[i] = arr[i - 1] + tmp;
		tmp = t;
	}
	arr.push_back(tmp);
}
}; // namespace Math
