/**
 * \file
 * \brief
 * \author Martin Herich
 * \copyright Copyright (C) 2016-2023 I3T team, Department of Computer Graphics
 * and Interaction, FEE, Czech Technical University in Prague, Czech Republic
 *
 * This file is part of I3T - An Interactive Tool for Teaching Transformations
 * http://www.i3t-tool.org
 *
 * GNU General Public License v3.0 (see LICENSE.txt or https://www.gnu.org/licenses/gpl-3.0.txt)
 */
#pragma once

#include <random>

#include "glm/glm.hpp"
// #include "glm/gtx/quaternion.hpp"
#include "Core/Nodes/GraphManager.h"
#include "glm/ext/quaternion_geometric.hpp" // normalize

using namespace Core;

std::default_random_engine& randomEngine();

inline int generateInt(int from = 0, int to = 3)
{
	std::mt19937 mt{std::random_device{}()};
	std::uniform_int_distribution<> dist(from, to);

	return static_cast<int>(dist(mt));
}

inline float generateFloat(float from = -10.0f, float to = 10.0f)
{
	std::mt19937 mt{std::random_device{}()};
	std::uniform_real_distribution<> dist(from, to);

	return static_cast<float>(dist(mt));
}

inline float generateAngleFromDegs(float from = 0.0f, float to = 90.0f)
{
	return glm::radians(generateFloat(from, to));
}

inline glm::vec3 generateVec3()
{
	return glm::vec3(generateFloat(), generateFloat(), generateFloat());
}

inline glm::vec4 generateVec4()
{
	return glm::vec4(generateFloat(), generateFloat(), generateFloat(), generateFloat());
}

inline glm::vec4 generateUnitVec4()
{
	return (glm::normalize(generateVec4()));
}

inline glm::vec4 generateNonUnitVec4()
{
	glm::vec4 vec;

	do
	{
		vec = generateVec4();
#ifdef PF_DEBUG
		std::cerr << "regenerate vector" << std::endl;
#endif

	} while (glm::length2(vec) <= 1.0f);

	return (vec);
}


inline glm::mat4 generateMat4()
{
	return glm::mat4(generateVec4(), generateVec4(), generateVec4(), generateVec4());
}

inline Ptr<Sequence> arrangeSequence(int matrices = 3)
{
	// Create seq. and matrices.
	auto seq = Core::GraphManager::createSequence();

	auto mat1 = Core::Builder::createTransform<ETransformType::Free>();
	auto mat2 = Core::Builder::createTransform<ETransformType::Free>();
	auto mat3 = Core::Builder::createTransform<ETransformType::Free>();

	{
		auto result = mat1->setValue(generateMat4());
		EXPECT_EQ(ValueSetResult::Status::Ok, result.status);
	}
	{
		auto result = mat2->setValue(generateMat4());
		EXPECT_EQ(ValueSetResult::Status::Ok, result.status);
	}
	{
		auto result = mat3->setValue(generateMat4());
		EXPECT_EQ(ValueSetResult::Status::Ok, result.status);
	}

	// Insert matrices into the seq.
	seq->addMatrix(std::move(mat1), 0);
	seq->addMatrix(std::move(mat2), 1);
	seq->addMatrix(std::move(mat3), 2);

	return seq;
}

inline glm::quat generateQuat()
{
	return {generateFloat(), generateFloat(), generateFloat(), generateFloat()};
}

inline glm::quat generateUnitQuat()
{
	return (glm::normalize(generateQuat()));
}

/**
 * \brief generate quaternion of length > 1
 * \return quaternion of length > 1
 */
inline glm::quat generateNonUnitQuat()
{
	glm::quat quat;

	do
	{
		quat = generateQuat();
#ifdef PF_DEBUG
		std::cerr << "regenerate quat" << std::endl;
#endif

	} while (glm::length2(quat) <= 1.0f);

	return (quat);
}
