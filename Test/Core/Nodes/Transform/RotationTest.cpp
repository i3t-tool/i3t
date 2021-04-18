#include "gtest/gtest.h"
#include <Core/Nodes/Utils.h>

#include "Core/Nodes/GraphManager.h"

#include "Generator.h"
#include "../Utils.h"

using namespace Core;

//===-- Euler rotation around X axis --------------------------------------===//

TEST(EulerXTest, OneValueSet)
{
	auto rotXNode = Core::Builder::createTransform<EulerRotX>();

	{
		// mat[1][1], cos(T)
		auto rads = 0.5f;

		setValue_expectOk(rotXNode, glm::cos(rads), {1, 1});

		auto mat = rotXNode->getData().getMat4();
		auto expectedMat = glm::rotate(rads, glm::vec3(1.0f, 0.0f, 0.0f));
		EXPECT_TRUE(Math::eq(expectedMat, mat));
	}
	{
		// mat[1][2], sin(T)
		auto rads = 0.4f;

		setValue_expectOk(rotXNode, glm::sin(rads), {1, 2});

		auto mat = rotXNode->getData().getMat4();
		auto expectedMat = glm::rotate(rads, glm::vec3(1.0f, 0.0f, 0.0f));
		EXPECT_TRUE(Math::eq(expectedMat, mat));
	}
	{
		// mat[2][1], -sin(T)
		auto rads = 0.8f;

		setValue_expectOk(rotXNode, -glm::sin(rads), {2, 1});

		auto mat = rotXNode->getData().getMat4();
		auto expectedMat = glm::rotate(rads, glm::vec3(1.0f, 0.0f, 0.0f));
		EXPECT_TRUE(Math::eq(expectedMat, mat));
	}
	{
		// mat[2][2], cos(T)
		auto rads = 1.0f;

		setValue_expectOk(rotXNode, glm::cos(rads), {2, 2});

		auto mat = rotXNode->getData().getMat4();
		auto expectedMat = glm::rotate(rads, glm::vec3(1.0f, 0.0f, 0.0f));
		EXPECT_TRUE(Math::eq(expectedMat, mat));
	}
}

TEST(GLM, RotateXAndEulerAngleXShouldBeSame)
{
  float initialRad = glm::radians(generateFloat());

	auto lhs = glm::rotate(initialRad, glm::vec3(1.0f, 0.0f, 0.0f));
	auto rhs = glm::eulerAngleX(initialRad);

	EXPECT_EQ(lhs, rhs);
}

TEST(GLM, GetAngleFromEulerX)
{
	for (int i = 0; i < 10; ++i)
  {
    float initialRad = glm::radians(generateFloat());

    auto eulerXMat = glm::eulerAngleX(initialRad);

    float angle = std::atan2(-eulerXMat[2][1], eulerXMat[2][2]);

    EXPECT_TRUE(Math::eq(initialRad, angle));
	}
}

TEST(EulerXTest, SetMatrixShuldBeValid)
{
	float initialRad = glm::radians(generateFloat());
	auto eulerX = Builder::createTransform<EulerRotX>(initialRad);

	auto mat = glm::eulerAngleX(generateFloat());

	setValue_expectOk(eulerX, mat);
	EXPECT_EQ(mat, eulerX->getData().getMat4());

	eulerX->reset();
	auto expectedMat = glm::eulerAngleX(initialRad);
	auto currentMat = eulerX->getData().getMat4();
	EXPECT_EQ(expectedMat, currentMat);
}

//===-- Euler rotation around Y axis --------------------------------------===//

TEST(EulerYTest, OneValueSet)
{
	auto rotYNode = Core::Builder::createTransform<EulerRotY>();

	{
		// mat[0][0], cos(T)
		auto rads = 0.5f;

		auto val = glm::cos(rads);

		setValue_expectOk(rotYNode, val, {0, 0});

		auto mat = rotYNode->getData().getMat4();
		auto expectedMat = glm::rotate(rads, glm::vec3(0.0f, 1.0f, 0.0f));
		EXPECT_TRUE(Math::eq(expectedMat, mat));
	}
	{
		// mat[2][0], sin(T)
		auto rads = 0.4f;

		setValue_expectOk(rotYNode, glm::sin(rads), {2, 0});

		auto mat = rotYNode->getData().getMat4();
		auto expectedMat = glm::rotate(rads, glm::vec3(0.0f, 1.0f, 0.0f));
		EXPECT_TRUE(Math::eq(expectedMat, mat));
	}
	{
		// mat[0][2], -sin(T)
		auto rads = 0.8f;

		setValue_expectOk(rotYNode, -glm::sin(rads), {0, 2});

		auto mat = rotYNode->getData().getMat4();
		auto expectedMat = glm::rotate(rads, glm::vec3(0.0f, 1.0f, 0.0f));
		EXPECT_TRUE(Math::eq(expectedMat, mat));
	}
	{
		// mat[2][2], cos(T)
		auto rads = 1.0f;

		setValue_expectOk(rotYNode, glm::cos(rads), {2, 2});

		auto mat = rotYNode->getData().getMat4();
		auto expectedMat = glm::rotate(rads, glm::vec3(0.0f, 1.0f, 0.0f));
		EXPECT_TRUE(Math::eq(expectedMat, mat));
	}
}

/*
/// \todo MH GLM_GetAngleFromEulerY won't pass.
TEST(GLM, GetAngleFromEulerY)
{
  for (int i = 0; i < 10; ++i)
  {
    float initialRad = generateFloat(-(M_PI / 2.0f), M_PI / 2.0f);

    auto mat = glm::eulerAngleY(initialRad);

    // float angle = std::atan2(sqrt(1 - std::pow(mat[2][2], 2)), mat[2][2]);
    float angle = std::asin(mat[2][0]);

    EXPECT_TRUE(Math::eq(initialRad, angle));
  }
}

/// \todo MH EulerYTest_SetMatrixShouldBeValid won't pass.
TEST(EulerYTest, SetMatrixShouldBeValid)
{
	for (int i = 0; i < 10; ++i)
  {
    float initialRad = generateFloat(-(M_PI / 2.0f), M_PI / 2.0f);
    auto eulerY = Builder::createTransform<EulerRotY>(initialRad);

    auto mat = glm::eulerAngleY(generateFloat());

    auto setResult = eulerY->setValue(mat);
    EXPECT_EQ(ValueSetResult::Status::Ok, setResult.status);
    EXPECT_EQ(mat, eulerY->getData().getMat4());

    eulerY->reset();
    auto expectedMat = glm::eulerAngleY(initialRad);
    auto currentMat = eulerY->getData().getMat4();
    EXPECT_EQ(expectedMat, currentMat);
	}
}
 */

//===-- Euler rotation around Z axis --------------------------------------===//

TEST(EulerZTest, OneValueSet)
{
	auto rotZNode = Core::Builder::createTransform<EulerRotZ>();

	{
		// mat[0][0], cos(T)
		auto rads = 0.5f;

		setValue_expectOk(rotZNode, glm::cos(rads), {0, 0});

		auto mat = rotZNode->getData().getMat4();
		auto expectedMat = glm::rotate(rads, glm::vec3(0.0f, 0.0f, 1.0f));
		EXPECT_TRUE(Math::eq(expectedMat, mat));
	}
	{
		// mat[0][1], sin(T)
		auto rads = 0.4f;

		setValue_expectOk(rotZNode, glm::sin(rads), {0, 1});

		auto mat = rotZNode->getData().getMat4();
		auto expectedMat = glm::rotate(rads, glm::vec3(0.0f, 0.0f, 1.0f));
		EXPECT_TRUE(Math::eq(expectedMat, mat));
	}
	{
		// mat[1][0], -sin(T)
		auto rads = 0.8f;

		setValue_expectOk(rotZNode, -glm::sin(rads), {1, 0});

		auto mat = rotZNode->getData().getMat4();
		auto expectedMat = glm::rotate(rads, glm::vec3(0.0f, 0.0f, 1.0f));
		EXPECT_TRUE(Math::eq(expectedMat, mat));
	}
	{
		// mat[1][1], cos(T)
		auto rads = 1.0f;

		setValue_expectOk(rotZNode, glm::cos(rads), {1, 1});

		auto mat = rotZNode->getData().getMat4();
		auto expectedMat = glm::rotate(rads, glm::vec3(0.0f, 0.0f, 1.0f));
		EXPECT_TRUE(Math::eq(expectedMat, mat));
	}
}

TEST(GLM, GetAngleFromEulerZ)
{
  for (int i = 0; i < 10; ++i)
  {
    float initialRad = glm::radians(generateFloat());

    auto mat = glm::eulerAngleZ(initialRad);

    float angle = glm::atan(mat[0][1], mat[0][0]);

    EXPECT_TRUE(Math::eq(initialRad, angle));
  }
}

TEST(EulerZTest, SetMatrixShouldBeValid)
{
  float initialRad = glm::radians(generateFloat());
  auto eulerZ = Builder::createTransform<EulerRotZ>(initialRad);

  auto mat = glm::eulerAngleZ(generateFloat());

  setValue_expectOk(eulerZ, mat);
  EXPECT_EQ(mat, eulerZ->getData().getMat4());

	eulerZ->reset();
  auto expectedMat = glm::eulerAngleZ(initialRad);
  auto currentMat = eulerZ->getData().getMat4();
  EXPECT_EQ(expectedMat, currentMat);
}

//===-- Euler rotation other tests ... ------------------------------------===//

TEST(EulerTest, XYZAngleSetShouldBeCorrect)
{
  std::array<NodePtr, 3> rots = {
      Builder::createTransform<EulerRotX>(),
      Builder::createTransform<EulerRotY>(),
      Builder::createTransform<EulerRotZ>(),
  };
  auto angle = glm::radians(85.0f);
  std::array<glm::mat4, 3> expectedMatrices = {
      glm::rotate(angle, glm::vec3(1.0f, 0.0f, 0.0f)),
      glm::rotate(angle, glm::vec3(0.0f, 1.0f, 0.0f)),
      glm::rotate(angle, glm::vec3(0.0f, 0.0f, 1.0f)),
  };

  for (int i = 0; i < 3; ++i)
  {
    setValue_expectOk(rots[i], angle);
    EXPECT_EQ(expectedMatrices[i], rots[i]->getData().getMat4());
  }
}

//===-- Axis angle test ---------------------------------------------------===//

TEST(AxisAngleTest, RotationMatrixAfterSetValueShouldBeValid)
{
	auto axisAngle = Builder::createTransform<AxisAngleRot>();

	auto rads = generateFloat();
	auto axis = generateVec3();

	auto expectedMat = glm::rotate(rads, axis);

  {
    setValue_expectOk(axisAngle, rads);
		EXPECT_EQ(rads, axisAngle->getRot());
  }
  {
    setValue_expectOk(axisAngle, axis);
    EXPECT_EQ(axis, axisAngle->getAxis());
  }
	EXPECT_EQ(expectedMat, axisAngle->getData().getMat4());
}
