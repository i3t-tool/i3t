#include "gtest/gtest.h"

#include "Generator.h"
#include "Utils/Math.h"

TEST(GeneratorTest, GeneratorShouldGenerateRandomValues)
{
	float prev = 0.0f;
	float next = 0.0f;

	for (int i = 0; i < 10; ++i)
	{
		next = generateFloat();
		EXPECT_FALSE(Math::eq(prev, next));
		prev = next;
	}
}
