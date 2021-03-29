#include "gtest/gtest.h"

#include "Core/Nodes/GraphManager.h"

#include "Generator.h"

using namespace Core;

TEST(SetWrongMatToScaleNode, ActionShouldNotBePermitted)
{
	// Create uniform scale.
	auto scale = Builder::createTransform<Scale>();

	{
		// Set new uniform scale.
		auto result = scale->setValue(glm::scale(glm::vec3(3.0f, 3.0f, 3.0f)));

		EXPECT_EQ(ValueSetResult::Status::Ok, result.status);
	}
	{
		// Set new non-uniform scale.
		auto result = scale->setValue(glm::scale(glm::vec3(1.0f, 5.0f, 3.0f)));

		EXPECT_EQ(ValueSetResult::Status::Err_ConstraintViolation, result.status);
	}
	{
		// Create free transform.
		glm::mat4 mat(1.0f);
		mat[1][3] = 165.0f;

		// Set non uniform scale constraint.
		scale->setDataMap(Transform::g_Scale);
		auto result = scale->setValue(mat);

		EXPECT_EQ(ValueSetResult::Status::Err_ConstraintViolation, result.status);
	}
}

TEST(SetVec3Scale, ScaleShouldBeValid)
{
	auto scale = glm::vec3(1.5f, 1.5f, 1.5f);
	auto scaleNode = Builder::createTransform<Scale>();

	scaleNode->setDataMap(Transform::g_Scale);
	EXPECT_EQ(Transform::g_Scale, scaleNode->getDataMap());

	auto result = scaleNode->setValue(scale);
	EXPECT_EQ(ValueSetResult::Status::Ok, result.status);

	auto expectedScale = glm::scale(scale);
	EXPECT_EQ(expectedScale, scaleNode->getData().getMat4());
}

TEST(ResetScaleNode, ResetsNodeToInitialValues)
{
	auto scale = glm::vec3(7.f, -5.f, 3.f);

	// Create non-uniform scale
	auto scaleNode = Builder::createTransform<Core::Scale>(scale, Transform::g_Scale);
	EXPECT_EQ(scaleNode->getDataMap(), Transform::g_Scale);

	// Set free transformation node.
	scaleNode->setDataMap(Transform::g_Free);
	EXPECT_EQ(scaleNode->getDataMap(), Transform::g_Free);

	glm::mat4 mat(1.0f);
	mat[1][3] = 165.0f;

	auto result = scaleNode->setValue(mat);
	EXPECT_EQ(ValueSetResult::Status::Ok, result.status);

	{
		auto data = scaleNode->getData().getMat4();

		EXPECT_TRUE(Math::eq(data, mat));
	}
	{
		// Reset to initial values and state.
		scaleNode->reset();
		EXPECT_EQ(scaleNode->getDataMap(), Transform::g_Scale);

		auto data = scaleNode->getData().getMat4();

		auto tmp = glm::scale(scale);

		EXPECT_TRUE(Math::eq(data, tmp));
	}
}

TEST(UniformScaleSynergies, OneValueSetShouldFollowUniformScaleSynergies)
{
	auto scale = glm::vec3(-2.0f);
	auto scaleMat = glm::scale(scale);

	auto scaleNode = Builder::createTransform<Core::Scale>(scale, Transform::g_UniformScale);

	{
		// Invalid coordinates.
		auto result = scaleNode->setValue(-2.0f, {3, 1});
		EXPECT_EQ(ValueSetResult::Status::Err_ConstraintViolation, result.status);
	}
	{
		// Valid coordinates.
		auto result = scaleNode->setValue(-2.0f, {1, 1});
		EXPECT_EQ(ValueSetResult::Status::Ok, result.status);

		auto data = scaleNode->getData().getMat4();

		EXPECT_EQ(data, scaleMat);
	}
}

TEST(Scale, GettersAndSetterShouldBeOk)
{
  auto scale = Builder::createTransform<Scale>()->as<Scale>();

  auto vec = generateVec3();

  {
		// Uniform scale.
    scale->setX(vec.x);
    scale->setY(vec.y);
    scale->setZ(vec.z);

    EXPECT_EQ(glm::scale(glm::vec3{vec.z, vec.z, vec.z}), scale->getData().getMat4());
	}
  {
		scale->setDataMap(Transform::g_Scale);

    scale->setX(vec.x);
    scale->setY(vec.y);
    scale->setZ(vec.z);

    EXPECT_EQ(glm::scale(vec), scale->getData().getMat4());
	}
}
