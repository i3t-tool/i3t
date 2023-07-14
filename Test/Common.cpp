#include "Common.h"

#include "gtest/gtest.h"

#include "Config.h"
#include "Core/Application.h"
#include "Logger/Logger.h"
#include "Utils/Format.h"
#include "Utils/Math.h"

// using namespace Core;

bool compare(const glm::mat4& lhs, const glm::mat4& rhs, float epsilon)
{
	auto result = Math::eq(lhs, rhs, epsilon);
	if (!result)
	{
		std::cerr << Utils::toString(lhs) << std::endl;
		std::cerr << "!=\n";
		std::cerr << Utils::toString(rhs) << std::endl;
	}

	return result;
}
