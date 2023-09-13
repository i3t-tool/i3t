#include "Common.h"

#include "gtest/gtest.h"

#include "../Test/Generator.h"
#include "Config.h"
#include "Core/Application.h"
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

TEST(Math, MatrixComparison)
{
	const glm::mat4 a = generateMat4();
	const glm::mat4 b = generateMat4();
	EXPECT_PRED_FORMAT2(AssertEqualMatrices, a, a);  // Succeeds
	EXPECT_PRED_FORMAT2(!AssertEqualMatrices, a, b); // !Fails
}
