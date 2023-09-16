#include "gtest/gtest.h"

// #include <cmath>

#include "Core/Nodes/GraphManager.h"

// #include "../Utils.h"

using namespace Core;

TEST(QuaternionTest, DefinitionVariants)
{
	// initialization
	auto quatNode = Builder::createTransform<ETransformType::Quat>();
	auto& q = quatNode->getDefaultValue("quat").getQuat();
	EXPECT_EQ(q, glm::quat(1.0f, 0.0f, 0.0f, 0.0f));

	// different angles around X
	float angles[] = {0, M_PI / 4.0, M_PI / 3.0, M_PI / 2.0, M_PI, 2.0 * M_PI};
	glm::vec3 axes[] = {
	    glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f),
	    glm::vec3(1.0f, 1.0f, 0.0f), glm::vec3(1.0f, 0.0f, 1.0f), glm::vec3(1.0f, 1.0f, 1.0f),
	};
	// glm::vec3 axis = glm::vec3(1.0f, 0.0f, 0.0f);

	for (const auto axis : axes)
	{
		glm::vec3 normalized_axis = glm::normalize(axis);

		for (const auto angle : angles)
		{
			auto c = cosf(angle);
			auto s = sinf(angle);
			glm::quat q1 = glm::quat();
			q1.w = c;
			q1.x = s * normalized_axis.x;
			q1.y = s * normalized_axis.y;
			q1.z = s * normalized_axis.z;

			quatNode->setDefaultValue("quat", q1);

			glm::quat q3 = quatNode->getDefaultValue("quat").getQuat();
			/*
			std::cout << "Angle = " << angle << "  ";
			std::cout << "(" << q1.w << ",  " << q1.x << ",  " << q1.y << ",  " << q1.z << ") = ";
			std::cout << "(" << q3.w << ",  " << q3.x << ",  " << q3.y << ",  " << q3.z << ")";
			std::cout << std::endl;
			*/

			// EXPECT_EQ(q1, q3);  // does not use nearlyEqual() for comparison, use Math::eq()
			EXPECT_TRUE(Math::eq(q1, q3));
			EXPECT_TRUE(Math::eq(q1.w, q3.w));
			EXPECT_TRUE(Math::eq(q1.x, q3.x));
			EXPECT_TRUE(Math::eq(q1.y, q3.y));
			EXPECT_TRUE(Math::eq(q1.z, q3.z));
			EXPECT_TRUE(Math::eq(q1, q3));

			// std::cout << "-----------------" << std::endl;
		}
		// std::cout << "=================" << std::endl;
	}
}


/*
EXPECT_TRUE(
      Math::eq(

*/