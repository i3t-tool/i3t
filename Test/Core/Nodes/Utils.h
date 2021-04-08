#include "gtest/gtest.h"

#include "glm/glm.hpp"

#include "Core/Nodes/GraphManager.h"

using namespace Core;

inline glm::mat4 getMatProduct(const std::vector<Ptr<Transformation>>& matrices)
{
	glm::mat4 result(1.0f);
	for (const auto& mat : matrices)
		result *= mat->getData().getMat4();
	return result;
}
