#include "gtest/gtest.h"

#include "Core/Nodes/GraphManager.h"

#include "Generator.h"
#include "../Utils.h"

using namespace Core;

TEST(TranslationTest, CoordsValidation)
{
	auto translationNode = Builder::createTransform<Translation>();

	{
		// Invalid coordinates.
		auto result = translationNode->setValue(-2.0f, {0, 3});
		EXPECT_EQ(ValueSetResult::Status::Err_ConstraintViolation, result.status);
	}
	{
		// Valid coordinates.
    setValue_expectOk(translationNode, -2.0f, {3, 0});
    setValue_expectOk(translationNode, -2.0f, {3, 1});
    setValue_expectOk(translationNode, -2.0f, {3, 2});

		auto translMat = glm::translate(glm::vec3(-2.0f, -2.0f, -2.0f));
		auto data = translationNode->getData().getMat4();

		EXPECT_EQ(translMat, data);
	}
}

TEST(TranslationTest, GettersAndSetterShouldBeOk)
{
  auto translation = Builder::createTransform<Translation>()->as<Translation>();

	auto vec = generateVec3();

	translation->setX(vec.x);
	translation->setY(vec.y);
	translation->setZ(vec.z);

	EXPECT_EQ(glm::translate(vec), translation->getData().getMat4());
}
