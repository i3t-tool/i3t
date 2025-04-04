/**
 * \file
 * \brief Declares some math functions
 * \authors Michal Folta, Petr Felkel, Martin Herich
 * \copyright Copyright (C) 2016-2023 I3T team, Department of Computer Graphics
 * and Interaction, FEE, Czech Technical University in Prague, Czech Republic
 *
 * This file is part of I3T - An Interactive Tool for Teaching Transformations
 * http://www.i3t-tool.org
 *
 * GNU General Public License v3.0 (see LICENSE.txt or https://www.gnu.org/licenses/gpl-3.0.txt)
 *
 *
 * Test functions modified
 * - eq(A,B) use the interval test based on interval around values of size factor * nearest bigger / smaller float value
 * - eqToOne use classic epsilon test with eps = 0.00001 - see the constants below
 *
 * \todo Test the appropriate factors
 */
#pragma once

#include "Core/Defs.h"

#include "glm/glm.hpp"
#include "glm/gtx/norm.hpp"
#include <iostream>
#include <random>
#include <vector>

#define I3_PI 3.14159265358979f

// ----------------------- A better way for float comparison -------------
// based on https://stackoverflow.com/questions/4548004/how-to-correctly-and-standardly-compare-floats
//
FORCE_INLINE bool nearlyEqual(const float a, const float b)
{
	return std::nextafter(a, std::numeric_limits<float>::lowest()) <= b &&
	       std::nextafter(a, std::numeric_limits<float>::max()) >= b;
}

// FORCE_INLINE bool nearlyEqual(const double a, const double b)
// {
// 	return std::nextafter(a, std::numeric_limits<double>::lowest()) <= b &&
// 	       std::nextafter(a, std::numeric_limits<double>::max()) >= b;
// }

/**
 * \brief Approximate float comparison
 * \param a first float
 * \param b second float
 * \param factor a factor of epsilon
 * \return true if the float values represent nearly the same value (precision interval is widened by \a factor)
 */
FORCE_INLINE bool nearlyEqual(const float a, const float b, const int factor)
{
	const float min_a = a - (a - std::nextafter(a, std::numeric_limits<float>::lowest())) * static_cast<float>(factor);
	const float max_a = a + (std::nextafter(a, std::numeric_limits<float>::max()) - a) * static_cast<float>(factor);
#ifdef PF_DEBUG
	if (!(min_a <= b && max_a >= b))
	{
		std::cerr << std::setprecision(15);
		std::cerr << "MinDelta = "
		          << (a - std::nextafter(a, std::numeric_limits<float>::lowest())) * static_cast<float>(factor)
		          << std::endl;
		std::cerr << "MaxDelta = "
		          << (std::nextafter(a, std::numeric_limits<float>::max()) - a) * static_cast<float>(factor)
		          << std::endl;
		std::cerr << a << " != " << b << " from ( " << min_a << ", " << max_a << "), factor = " << factor << std::endl;
	}
#endif

	return min_a <= b && max_a >= b;
}

/**
 * \brief Approximate double comparison
 * \param a first double
 * \param b second double
 * \param factor a factor of epsilon
 * \return true if the double values represent nearly the same value (precision interval is widened by \a factor)
 */
// FORCE_INLINE bool nearlyEqual(const double a, const double b, const int factor)
// {
//	const double min_a = a - (a - std::nextafter(a, std::numeric_limits<double>::lowest()))
//	                     * static_cast<double>(factor);
//	const double max_a = a + (std::nextafter(a, std::numeric_limits<double>::max()) - a)
//	                     * static_cast<double>(factor);
// 	return min_a <= b && max_a >= b;
// }

namespace Math
{
static constexpr int FACTOR_NEARLY_EQUAL = 0x01 << 8; ///< interval weight for comparison of nearly equal values
static constexpr int FACTOR_SIMILAR = 0x01 << 12;     ///< interval weight for comparison of similar values
static constexpr int FACTOR_ROUGHLY_SIMILAR = 0x01
                                              << 14; ///< interval weight for comparison of very roughly similar values

// deprecated constants
// static constexpr int FACTOR_FOR_FLOATS = 0x01 << 12;      ///< interval weight for float comparison
// static constexpr int FACTOR_FOR_MATRICES = 0x01 << 12;    ///< interval weight for matrix comparison
// static constexpr int FACTOR_FOR_QUATERNIONS = 0x01 << 12; ///< interval weight for quaternion comparison


// \todo Epsilon arithmetic is deprecated
static constexpr float FLT_EPSILON_10 = 10.0f * FLT_EPSILON;     ///< 1.192093 E-6
static constexpr float FLT_EPSILON_100 = 100.0f * FLT_EPSILON;   ///< 1.192093 E-5
static constexpr float FLT_EPSILON_1000 = 1000.0f * FLT_EPSILON; ///< 1.192093 E-4
static constexpr float FLT_EPSILON_FIVE_DECIMALS = 0.00001f;     ///< simple interval (1.0 +- epsilon)

// test in simple interval (1.0 +- epsilon)
FORCE_INLINE bool eqToOne(const float value, const float epsilon = FLT_EPSILON_FIVE_DECIMALS)
{
	return (1.0f - epsilon) <= abs(value) && abs(value) <= (1.0f + epsilon);
}
FORCE_INLINE bool eqToZero(const float value, const float epsilon = FLT_EPSILON_FIVE_DECIMALS)
{
	return abs(value) <= epsilon;
}


// Two variants of eq - they differ by the last parameter int or float.
// - int uses the factor and is considered default
// - float uses epsilon

/**
 * \brief Comparison of two floats using epsilon, useful for comparison to zero and to one
 * \param lhs First value
 * \param rhs Second value
 * \param epsilon interval size - roughly defining the number of decimal places
 * \return epsilon comparison
 */
FORCE_INLINE bool eq(const float lhs, const float rhs, const float epsilon)
{
	return abs(lhs - rhs) < epsilon;
	// return nearlyEqual(lhs, rhs);
}

// FORCE_INLINE bool eq(const float lhs, const float rhs)
// {
//	// return abs(lhs - rhs) < epsilon;
// 	return nearlyEqual(lhs, rhs);
// }

/**
 * \brief Comparison of two floats using factor * difference to nextafter() value,
 * useful for comparison of two floats of unknown value
 * \param lhs First value
 * \param rhs Second value
 * \param factor multiplies the distance to the nextafter() value used as a size of comparison interval
 * \return factor based comparison
 */
FORCE_INLINE bool eq(const float lhs, const float rhs, const int factor = FACTOR_SIMILAR)
{
	// return abs(lhs - rhs) < epsilon;
	return nearlyEqual(lhs, rhs, factor);
}

// FORCE_INLINE bool eq(const glm::vec3& lhs, const glm::vec3& rhs, const float epsilon = FLT_EPSILON_100)
FORCE_INLINE bool eq(const glm::vec3& lhs, const glm::vec3& rhs, const int factor = FACTOR_SIMILAR)
{
	for (int i = 0; i < 3; ++i)
	{
		// if (!Math::eq(lhs[i], rhs[i], epsilon))
		if (!Math::eq(lhs[i], rhs[i], factor))
		{
			return false;
		}
	}

	return true;
}

// FORCE_INLINE bool eq(const glm::vec4& lhs, const glm::vec4& rhs, const float epsilon = FLT_EPSILON_100)
FORCE_INLINE bool eq(const glm::vec4& lhs, const glm::vec4& rhs, const int factor = FACTOR_SIMILAR)
{
	for (int i = 0; i < 4; ++i)
	{
		// if (!Math::eq(lhs[i], rhs[i], epsilon))
		if (!Math::eq(lhs[i], rhs[i], factor))
		{
			return false;
		}
	}

	return true;
}

// FORCE_INLINE bool eq(const glm::quat& lhs, const glm::quat& rhs, const float epsilon = FLT_EPSILON_100)
FORCE_INLINE bool eq(const glm::quat& lhs, const glm::quat& rhs, const int factor = FACTOR_SIMILAR)
{
	for (int i = 0; i < 4; ++i)
	{
		if (!Math::eq(lhs[i], rhs[i], factor))
		{
			return false;
		}
	}

	return true;
}

// FORCE_INLINE bool eq(const glm::mat4& lhs, const glm::mat4& rhs, const float epsilon = FLT_EPSILON_100)

// Comparison of matrices using factor
FORCE_INLINE bool eq(const glm::mat4& lhs, const glm::mat4& rhs, const int factor = FACTOR_SIMILAR)
{
	for (int i = 0; i < 4; ++i)
	{
		for (int j = 0; j < 4; ++j)
		{
			// if (i == 1 && j == 2)
			//	std::cerr << "lhs[" << i << "], [" << j << "] = " << lhs[i][j]
			//	          << " != rhs[][] = " << rhs[i][j] << std::endl;
			if (!Math::eq(lhs[i][j], rhs[i][j], factor))
			{
				// std::cerr << "lhs[" << i << "], [" << j << "] = " << lhs[i][j]
				//          << " != rhs[][] = " << rhs[i][j] << " return false"
				//          << std::endl;
				return false;
			}
		}
	}

	return true;
}

// Comparison of matrices using epsilon
FORCE_INLINE bool eq(const glm::mat4& lhs, const glm::mat4& rhs, const float epsilon)
{
	for (int i = 0; i < 4; ++i)
	{
		for (int j = 0; j < 4; ++j)
		{
			// if (i == 1 && j == 2)
			//	std::cerr << "lhs[" << i << "], [" << j << "] = " << lhs[i][j]
			//	          << " != rhs[][] = " << rhs[i][j] << std::endl;
			if (!Math::eq(lhs[i][j], rhs[i][j], epsilon))
			{
				// std::cerr << "lhs[" << i << "], [" << j << "] = " << lhs[i][j]
				//          << " != rhs[][] = " << rhs[i][j] << " return false"
				//          << std::endl;
				return false;
			}
		}
	}

	return true;
}

template <typename T>
bool isNormalized(const T& val)
{
	// return eq(glm::length2(val), 1.0f, FACTOR_FOR_MATRICES);
	return eqToOne(glm::length2(val));
}

FORCE_INLINE bool areElementsSame(const glm::vec3 vec)
{
	return Math::eq(vec[0], vec[1]) && Math::eq(vec[1], vec[2]);
}

/// Inclusive interval
template <typename T>
FORCE_INLINE bool withinInterval(T val, T from, T to)
{
	return from <= val && val <= to;
}

/**
 * \brief Linear interpolation between two vectors vec3 \a a and \a b
 * \param a
 * \param b
 * \param alpha interpolation parameter - must be from interval (0..1)
 * \return linear interpolation of two vec3 vectors
 */
static glm::vec3 lerp(glm::vec3 a, glm::vec3 b, float alpha)
{
	glm::vec3 inter = a * (1.0f - alpha);
	inter = inter + b * alpha;
	return inter;
}

/**
 * \brief Linear interpolation between two vectors vec4 \a a and \a b
 * \param a the first vector
 * \param b the second vector
 * \param alpha interpolation parameter - must be from interval (0..1)
 * \return linear interpolation of two vec4 vectors
 */
static glm::vec4 lerp(glm::vec4 a, glm::vec4 b, float alpha)
{
	glm::vec4 inter = a * (1.0f - alpha);
	inter.a = a.w;
	inter = inter + b * alpha;
	return inter;
}

/**
 * \brief Linear interpolation between two 4x4 matrices \p lhs and \p rhs including translation columns.
 * Function depends on the parameter \a useQuat:
 * - true - A correct interpolation of rotational matrices using quaternions
 * - false - Incorrect interpolation, just for teaching purposes (how not to do it).
 * \param lhs the first matrix
 * \param rhs the second matrix
 * \param alpha interpolation parameter - must be from interval (0..1)
 * interpolation parameter - must be from interval (0..1)
 * \param useQuat use quaternion interpolation, if true
 * \return linear interpolation of two matrices vectors [wrong for useQuat == false!]
 */
glm::mat4 lerp(const glm::mat4& lhs, const glm::mat4& rhs, float alpha, bool useQuat = false);

/**
 * \brief Fifth order interpolation
 * \param a alpha
 * \return smoothed alpha
 */
static float smoothAlpha(const float a)
{
	return ((6 * a - 15) * a + 10) * a * a * a;
}

/**
 * \brief Random float from interval 0 .. 0.999
 * \return random float
 */
static float randomf()
{
	return (float) ((rand() % 1000) / 1000.0f);
}

/**
 * \brief Random float from interval -0.5 .. +0.499
 * \return random float
 */
static float randomfHalf()
{
	return -0.5f + randomf();
}

/**
 * \brief Random integer from interval 0..\a topNotInclude
 * \param topNotInclude maximum value of the interval
 * \return Random integer
 */
static int randomi(const int topNotInclude)
{
	return rand() % topNotInclude;
}

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

static float range(const float value, const float low1, const float high1, const float low2, const float high2)
{
	return low2 + (value - low1) * (high2 - low2) / (high1 - low1);
}
}; // namespace Math
