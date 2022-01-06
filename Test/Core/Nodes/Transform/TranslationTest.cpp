#include "gtest/gtest.h"

#include "Core/Nodes/GraphManager.h"

#include "Generator.h"
#include "../Utils.h"

using namespace Core;

TEST(TranslationTest, InvalidValues_ShouldNotBePermitted)
{
	auto translationNode = Builder::createTransform<ETransformType::Translation>()
	    ->as<TransformImpl<ETransformType::Translation>>();

	// Invalid coordinates.
	auto result = translationNode->setValue(generateFloat(), {0, 3});

	EXPECT_EQ(ValueSetResult::Status::Err_ConstraintViolation, result.status);
	EXPECT_EQ(ETransformState::Valid, translationNode->isValid());
}

TEST(TranslationTest, ValidValues_Ok)
{
	auto translationNode = Builder::createTransform<ETransformType::Translation>();

	// Valid coordinates.
	setValue_expectOk(translationNode, -2.0f, {3, 0});
	setValue_expectOk(translationNode, -2.0f, {3, 1});
	setValue_expectOk(translationNode, -2.0f, {3, 2});

	auto translMat = glm::translate(glm::vec3(-2.0f, -2.0f, -2.0f));
	auto data = translationNode->getData().getMat4();

	EXPECT_EQ(translMat, data);
	EXPECT_EQ(ETransformState::Valid, translationNode->isValid());
}

TEST(TranslationTest, Unlocked_InvalidValues_InvalidState)
{
	auto translationNode = Builder::createTransform<ETransformType::Translation>();
	translationNode->unlock();

	// Invalid coordinates.
	setValue_expectOk(translationNode, generateFloat(10.0f, 20.0f), {1, 1});

	EXPECT_EQ(ETransformState::Invalid, translationNode->isValid());
}

TEST(TranslationTest, GettersAndSetterShouldBeOk)
{
  auto translation = Builder::createTransform<ETransformType::Translation>()
			->as<TransformImpl<ETransformType::Translation>>();

	auto vec = generateVec3();

	translation->setX(vec.x);
	translation->setY(vec.y);
	translation->setZ(vec.z);

	EXPECT_EQ(glm::translate(vec), translation->getData().getMat4());
}
