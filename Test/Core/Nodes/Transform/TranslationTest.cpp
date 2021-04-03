#include "gtest/gtest.h"

#include "Core/Nodes/GraphManager.h"

#include "Generator.h"

using namespace Core;

TEST(Translation, CoordsValidation)
{
	auto translationNode = Core::Builder::createTransform<Translation>();

	{
		// Invalid coordinates.
		auto result = translationNode->setValue(-2.0f, {0, 3});
		EXPECT_EQ(ValueSetResult::Status::Err_ConstraintViolation, result.status);
	}
	{
		// Valid coordinates.
		{
			auto result = translationNode->setValue(-2.0f, {3, 0});
			EXPECT_EQ(ValueSetResult::Status::Ok, result.status);
		}
		{
			auto result = translationNode->setValue(-2.0f, {3, 1});
			EXPECT_EQ(ValueSetResult::Status::Ok, result.status);
		}
		{
			auto result = translationNode->setValue(-2.0f, {3, 2});
			EXPECT_EQ(ValueSetResult::Status::Ok, result.status);
		}

		auto translMat = glm::translate(glm::vec3(-2.0f, -2.0f, -2.0f));
		auto data = translationNode->getData().getMat4();

		EXPECT_EQ(translMat, data);
	}
}

TEST(Translation, GettersAndSetterShouldBeOk)
{
  auto translation = Core::Builder::createTransform<Translation>()->as<Translation>();

	auto vec = generateVec3();

	translation->setX(vec.x);
	translation->setY(vec.y);
	translation->setZ(vec.z);

	EXPECT_EQ(glm::translate(vec), translation->getData().getMat4());
}
