#include "Common.h"

#include "gtest/gtest.h"

#include "../Test/Generator.h"
#include "Config.h"
#include "Logger/Logger.h"
#include "Utils/Format.h"
#include "Utils/Math.h"

// using namespace Core;

// bool compare(const glm::mat4& lhs, const glm::mat4& rhs, float epsilon)
bool compare(const glm::mat4& lhs, const glm::mat4& rhs, int factor)
{
	// auto result = Math::eq(lhs, rhs, epsilon);
	auto result = Math::eq(lhs, rhs, factor);
	if (!result)
	{
		std::cerr << Utils::toString(lhs) << std::endl;
		std::cerr << "!=\n";
		std::cerr << Utils::toString(rhs) << std::endl;
	}

	return result;
}

// usage EXPECT_TRUE(assertionCompare(lhs, rhs, ))
testing::AssertionResult assertionCompare(const glm::mat4& lhs, const glm::mat4& rhs, int factor)
{
	if (Math::eq(lhs, rhs, factor))
	{
		return testing::AssertionSuccess();
	}
	else
	{
		return testing::AssertionFailure() << Utils::toString(lhs) << std::endl
		                                   << "!=\n"
		                                   << Utils::toString(rhs) << std::endl;
	}
}

// https://google.github.io/googletest/advanced.html
// usage EXPECT_TRUE(assertionCompare(lhs, rhs, ))
testing::AssertionResult assertionCompare(const glm::quat& lhs, const glm::quat& rhs, int factor)
{

	if (Math::eq(lhs, rhs, factor))
	{
		return testing::AssertionSuccess() << Utils::toString(lhs) << std::endl;
	}
	else
	{
		return testing::AssertionFailure() << Utils::toString(lhs) << std::endl
		                                   << "!=\n"
		                                   << Utils::toString(rhs) << std::endl;
	}
}

// A predicate-formatter for asserting that two matrices are equal.
// Use: EXPECT_PRED_FORMAT2(AssertEqualMatrices, a, b);
// or   EXPECT_PRED_FORMAT2(static_cast<bool (*)(glm::mat4, glm::mat4)AssertEqualMatrices, a, b); for templates
testing::AssertionResult AssertEqualMatrices(const char* m_expr, const char* n_expr, glm::mat4 m, glm::mat4 n)
{
	if (Math::eq(m, n, Math::FACTOR_SIMILAR))
		return testing::AssertionSuccess();

	return testing::AssertionFailure() << " Matrix " << m_expr << " != " << n_expr << "." << std::endl
	                                   << Utils::toString(m, true) << std::endl
	                                   << "\t!=" << std::endl
	                                   << Utils::toString(n, true) << std::endl
	                                   << std::endl;
}

testing::AssertionResult AssertRoughlyEqualMatrices(const char* m_expr, const char* n_expr, glm::mat4 m, glm::mat4 n)
{
	if (Math::eq(m, n, Math::FACTOR_ROUGHLY_SIMILAR))
		return testing::AssertionSuccess();

	return testing::AssertionFailure() << " Matrix " << m_expr << " != " << n_expr << "." << std::endl
	                                   << Utils::toString(m, true) << std::endl
	                                   << "\t!=" << std::endl
	                                   << Utils::toString(n, true) << std::endl
	                                   << std::endl;
}
