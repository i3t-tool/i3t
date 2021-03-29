#pragma once

#include <random>

#include "glm/glm.hpp"

std::default_random_engine& randomEngine();

inline float generateFloat(float from = -10.0f, float to = 10.0f)
{
	std::uniform_real_distribution<> distribution(from, to);

	return static_cast<float>(distribution(randomEngine()));
}

inline glm::vec3 generateVec3()
{
	return glm::vec3(generateFloat(), generateFloat(), generateFloat());
}

inline glm::vec4 generateVec4()
{
	return glm::vec4(generateFloat(), generateFloat(), generateFloat(), generateFloat());
}

inline glm::mat4 generateMat4()
{
	return glm::mat4(generateVec4(), generateVec4(), generateVec4(), generateVec4());
}
