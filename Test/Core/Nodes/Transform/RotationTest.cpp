#include "gtest/gtest.h"

#include "Core/Nodes/GraphManager.h"
#include "Core/Nodes/Utils.h"

#include "Generator.h"
#include "../Utils.h"

using namespace Core;
// ----------------------------------
// ---- Tests of the definitions ----
// ----------------------------------

TEST(GLM, RotateAndEulerAngle_Matrices_ShouldBeSame)
{
	float initialRad = glm::radians(generateFloat());

  // ---- EulerX ----
	auto lhs = glm::rotate(initialRad, glm::vec3(1.0f, 0.0f, 0.0f));
	auto rhs = glm::eulerAngleX(initialRad);
	EXPECT_EQ(lhs, rhs);

	// ---- EulerY ----
	lhs = glm::rotate(initialRad, glm::vec3(0.0f, 1.0f, 0.0f));
	rhs = glm::eulerAngleY(initialRad);
	EXPECT_EQ(lhs, rhs);

	// ---- EulerZ ----
	lhs = glm::rotate(initialRad, glm::vec3(1.0f, 0.0f, 1.0f));
	rhs = glm::eulerAngleZ(initialRad);
	EXPECT_EQ(lhs, rhs);
}


// angles
TEST(GLM, GetAngleFromEulerX)
{
	for (int i = 0; i < 10; ++i)
	{
		float initialRad      = glm::radians(generateFloat());
		auto  eulerXMat       = glm::eulerAngleX(initialRad);
		float angleFromMatrix = std::atan2(-eulerXMat[2][1], eulerXMat[2][2]);

		EXPECT_TRUE(Math::eq(initialRad, angleFromMatrix));
	}
}

// ---- EulerY ----
TEST(GLM, GetAngleFromEulerY)
{
	for (int i = 0; i < 10; ++i)
	{
		float initialRad      = glm::radians(generateFloat());
		auto  eulerYMat       = glm::eulerAngleY(initialRad);
		float angleFromMatrix = std::atan2(eulerYMat[2][0], eulerYMat[2][2]);

		EXPECT_TRUE(Math::eq(initialRad, angleFromMatrix));
	}
}

// ---- EulerZ ----
TEST(GLM, GetAngleFromEulerZ)
{
	for (int i = 0; i < 10; ++i)
	{
		float initialRad      = glm::radians(generateFloat());
		auto  eulerZMat       = glm::eulerAngleZ(initialRad);
		float angleFromMatrix = glm::atan(eulerZMat[0][1], eulerZMat[0][0]);

		EXPECT_TRUE(Math::eq(initialRad, angleFromMatrix));
	}
}





//===-- Euler rotation around X axis --------------------------------------===//

/*
TEST(EulerXTest, Synergies_OneCorrectValue_Ok)
{
	auto rotXNode = Builder::createTransform<ETransformType::EulerX>();

	{
		// mat[1][1], cos(T)
		auto rads = 0.5f;

		setValue_expectOk(rotXNode, glm::cos(rads), {1, 1});

		auto mat = rotXNode->getData().getMat4();
		auto expectedMat = glm::rotate(rads, glm::vec3(1.0f, 0.0f, 0.0f));

		EXPECT_TRUE(Math::eq(expectedMat, mat));
		EXPECT_EQ(ETransformState::Valid, rotXNode->isValid());
	}
	{
		// mat[1][2], sin(T)
		auto rads = 0.4f;

		setValue_expectOk(rotXNode, glm::sin(rads), {1, 2});

		auto mat = rotXNode->getData().getMat4();
		auto expectedMat = glm::rotate(rads, glm::vec3(1.0f, 0.0f, 0.0f));

		EXPECT_TRUE(Math::eq(expectedMat, mat));
		EXPECT_EQ(ETransformState::Valid, rotXNode->isValid());
	}
	{
		// mat[2][1], -sin(T)
		auto rads = 0.8f;

		setValue_expectOk(rotXNode, -glm::sin(rads), {2, 1});

		auto mat = rotXNode->getData().getMat4();
		auto expectedMat = glm::rotate(rads, glm::vec3(1.0f, 0.0f, 0.0f));

		EXPECT_TRUE(Math::eq(expectedMat, mat));
		EXPECT_EQ(ETransformState::Valid, rotXNode->isValid());
	}
	{
		// mat[2][2], cos(T)
		auto rads = 1.0f;

		setValue_expectOk(rotXNode, glm::cos(rads), {2, 2});

		auto mat = rotXNode->getData().getMat4();
		auto expectedMat = glm::rotate(rads, glm::vec3(1.0f, 0.0f, 0.0f));

		EXPECT_TRUE(Math::eq(expectedMat, mat));
		EXPECT_EQ(ETransformState::Valid, rotXNode->isValid());
	}
}
 */

/// \todo Fix this test.
/*
TEST(EulerXTest, SynergiesDisabled_OneCorrectValue_InvalidState)
{
	auto rot = Builder::createTransform<ETransformType::EulerX>();
	rot->disableSynergies();

	auto rads = generateFloat();

	setValue_expectOk(rot, glm::sin(rads), {1, 2});

	EXPECT_EQ(ETransformState::Invalid, rot->isValid());
}
 */

/// \todo Fix this test.
/*
TEST(EulerXTest, Unlocked_WrongValue_InvalidState)
{
	auto rot = Builder::createTransform<ETransformType::EulerX>();
	rot->unlock();

	auto rads = generateFloat();

	setValue_expectOk(rot, glm::sin(rads), {2, 3});

	EXPECT_EQ(ETransformState::Invalid, rot->isValid());
}
 */


/// \todo Fix this test.
/*
TEST(EulerXTest, SetMatrixShouldBeValid)
{
	float initialRad = glm::radians(generateFloat());
	auto eulerX = Builder::createTransform<ETransformType::EulerX>();
	eulerX->setDefaultValue("rotation", initialRad);

	auto mat = glm::eulerAngleX(generateFloat());

	setValue_expectOk(eulerX, mat);
	EXPECT_TRUE(Math::eq(mat, eulerX->getData().getMat4()));

	eulerX->resetMatrixFromDefaults();
	auto expectedMat = glm::eulerAngleX(initialRad);
	auto currentMat  = eulerX->getData().getMat4();
	EXPECT_TRUE(Math::eq(expectedMat, currentMat));
}
 */

//===-- Euler rotation around Y axis --------------------------------------===//

/*
TEST(EulerYTest, OneValueSet)
{
	auto rotYNode = Core::Builder::createTransform<ETransformType::EulerY>();

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
 */

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

    eulerY->resetMatrixFromDefaults();
    auto expectedMat = glm::eulerAngleY(initialRad);
    auto currentMat = eulerY->getData().getMat4();
    EXPECT_EQ(expectedMat, currentMat);
	}
}
 */

//===-- Euler rotation around Z axis --------------------------------------===//

/*

TEST(EulerZTest, OneValueSet)
{
	auto rotZNode = Core::Builder::createTransform<ETransformType::EulerZ>();

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
 */

/*
TEST(EulerZTest, SetMatrixShouldBeValid)
{
  float initialRad = glm::radians(generateFloat());
  auto eulerZ = Builder::createTransform<ETransformType::EulerZ>();
	eulerZ->setDefaultValue("rotation", initialRad);

  auto mat = glm::eulerAngleZ(generateFloat());

  setValue_expectOk(eulerZ, mat);
  EXPECT_EQ(mat, eulerZ->getData().getMat4());

	eulerZ->resetMatrixFromDefaults();
  auto expectedMat = glm::eulerAngleZ(initialRad);
  auto currentMat = eulerZ->getData().getMat4();
  EXPECT_EQ(expectedMat, currentMat);
}
 */

//===-- Euler rotation other tests ... ------------------------------------===//

TEST(EulerTest, XYZAngleSetShouldBeCorrect)
{
  std::array<NodePtr, 3> rots = {
      Builder::createTransform<ETransformType::EulerX>(),
      Builder::createTransform<ETransformType::EulerY>(),
      Builder::createTransform<ETransformType::EulerZ>(),
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
	auto axisAngle = Builder::createTransform<ETransformType::AxisAngle>()
	    ->as<TransformImpl<ETransformType::AxisAngle>>();

	auto rads = generateFloat();
	auto axis = generateVec3();

	auto expectedMat = glm::rotate(rads, axis);

  {
    setValue_expectOk(axisAngle, rads);
		EXPECT_EQ(rads, axisAngle->getDefaultValue("rotation").getFloat());
  }
  {
    setValue_expectOk(axisAngle, axis);
    EXPECT_EQ(axis, axisAngle->getDefaultValue("axis").getVec3());
  }
	EXPECT_EQ(expectedMat, axisAngle->getData().getMat4());
}

//===-- Quaternion rotation test ------------------------------------------===//

TEST(QuatRotTest, RotShouldBeValid)
{
	auto rot = Builder::createTransform<ETransformType::Quat>()
	    ->as<TransformImpl<ETransformType::Quat>>();

	auto vec = generateVec3();
	auto quat = glm::quat(1.0f, vec.x, vec.y, vec.z);

	setValue_expectOk(rot, quat);

	EXPECT_EQ(rot->getData().getMat4(), glm::toMat4(glm::normalize(quat)));
}

TEST(QuatRotTest, NodeValueShouldBeNormalized)
{
	auto rot = Builder::createTransform<ETransformType::Quat>()
	    ->as<TransformImpl<ETransformType::Quat>>();

  auto vec = generateVec4();
  auto quat = glm::quat(vec.x, vec.y, vec.z, vec.w);

	setValue_expectOk(rot, quat);

	auto normalized = glm::normalize(quat);

  EXPECT_EQ(normalized, rot->getNormalized());
}
