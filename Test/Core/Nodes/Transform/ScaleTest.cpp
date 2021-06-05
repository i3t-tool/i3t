#include "gtest/gtest.h"

#include "Core/Nodes/GraphManager.h"

#include "Generator.h"
#include "../Utils.h"

using namespace Core;

TEST(ScaleTest, SetWrongMatToScaleNode_ActionShouldNotBePermitted)
{
	// Create uniform scale.
	auto scale = Core::Builder::createTransform<Scale>();

	{
		// Set new uniform scale.
		setValue_expectOk(scale, glm::scale(glm::vec3(3.0f, 3.0f, 3.0f)));
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
		scale->setDataMap(&Transform::g_Scale);
		auto result = scale->setValue(mat);

		EXPECT_EQ(ValueSetResult::Status::Err_ConstraintViolation, result.status);
	}
}

TEST(ScaleTest, SetVec3Scale)
{
	auto scale = glm::vec3(1.5f, 1.5f, 1.5f);
	auto scaleNode = Core::Builder::createTransform<Scale>();

	scaleNode->setDataMap(&Transform::g_Scale);
	EXPECT_EQ(&Transform::g_Scale, scaleNode->getDataMap());

	setValue_expectOk(scaleNode, scale);

	auto expectedScale = glm::scale(scale);
	EXPECT_EQ(expectedScale, scaleNode->getData().getMat4());
}

TEST(ScaleTest, ResetsNodeToInitialValues)
{
	auto scale = generateVec3();

	// Create non-uniform scale
	auto scaleNode = Core::Builder::createTransform<Core::Scale>(scale);
	EXPECT_EQ(scaleNode->getDataMap(), &Transform::g_Scale);

	// Set free transformation node.
	scaleNode->unlock();
	EXPECT_EQ(scaleNode->getDataMap(), &Transform::g_Free);

	glm::mat4 mat(1.0f);
	mat[1][3] = 165.0f;

	setValue_expectOk(scaleNode, mat);
	{
		auto data = scaleNode->getData().getMat4();

		EXPECT_TRUE(Math::eq(data, mat));
	}
	{
		// Reset to initial values and state.
		scaleNode->reset();
		EXPECT_EQ(scaleNode->getDataMap(), &Transform::g_Scale);

		auto expected = glm::scale(scale);
		auto actual = scaleNode->getData().getMat4();

		EXPECT_TRUE(Math::eq(expected, actual));
	}
}

TEST(ScaleTest, UniformScaleSynergies)
{
	auto scaleValue = generateFloat();
	auto scale = glm::vec3(scaleValue);
	auto scaleMat = glm::scale(scale);

	auto scaleNode = Core::Builder::createTransform<Core::Scale>(scale);
	scaleNode->enableSynergies();

	{
		// Invalid coordinates.
		auto result = scaleNode->setValue(scaleValue, {3, 1});
		EXPECT_EQ(ValueSetResult::Status::Err_ConstraintViolation, result.status);
	}
	{
		// Valid coordinates.
		setValue_expectOk(scaleNode, scaleValue, {1, 1});

		auto data = scaleNode->getData().getMat4();

		EXPECT_EQ(data, scaleMat);
	}
}

TEST(ScaleTest, GettersAndSetterShouldBeOk)
{
  auto scale = Core::Builder::createTransform<Scale>()->as<Scale>();
	scale->enableSynergies();

  auto vec = generateVec3();

  {
		// Uniform scale.
    scale->setX(vec.x);
    scale->setY(vec.y);
    scale->setZ(vec.z);

    EXPECT_EQ(glm::scale(glm::vec3{vec.z, vec.z, vec.z}), scale->getData().getMat4());
	}
  {
		scale->disableSynergies();

    scale->setX(vec.x);
    scale->setY(vec.y);
    scale->setZ(vec.z);

    EXPECT_EQ(glm::scale(vec), scale->getData().getMat4());
	}
}
