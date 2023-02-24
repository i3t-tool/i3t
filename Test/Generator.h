#pragma once

#include <random>

#include "glm/glm.hpp"

#include "Core/Nodes/GraphManager.h"

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

inline glm::vec3 generateVec3() { return glm::vec3(generateFloat(), generateFloat(), generateFloat()); }

inline glm::vec4 generateVec4()
{
	return glm::vec4(generateFloat(), generateFloat(), generateFloat(), generateFloat());
}

inline glm::mat4 generateMat4() { return glm::mat4(generateVec4(), generateVec4(), generateVec4(), generateVec4()); }

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
