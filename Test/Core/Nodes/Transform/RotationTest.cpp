#include "gtest/gtest.h"

#include "Core/Nodes/GraphManager.h"
#include "Core/Nodes/Utils.h"
#include "Utils/Math.h"

// #include "../Utils.h"
#include "Common.h"
#include "Generator.h"
#include "Utils/Format.h"

using namespace Core;
// ----------------------------------
// ---- Tests of the definitions ----
// ----------------------------------

namespace rotations
{
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
	lhs = glm::rotate(initialRad, glm::vec3(0.0f, 0.0f, 1.0f));
	rhs = glm::eulerAngleZ(initialRad);
	EXPECT_EQ(lhs, rhs);
}

// angles
TEST(GLM, GetAngleFromEulerX)
{
	for (int i = 0; i < 10; ++i)
	{
		float initialRad = glm::radians(generateFloat());
		auto eulerXMat = glm::eulerAngleX(initialRad);
		float angleFromMatrix = std::atan2(-eulerXMat[2][1], eulerXMat[2][2]);

		EXPECT_TRUE(Math::eq(initialRad, angleFromMatrix));
	}
}

// ---- EulerY ----
TEST(GLM, GetAngleFromEulerY)
{
	for (int i = 0; i < 10; ++i)
	{
		float initialRad = glm::radians(generateFloat());
		auto eulerYMat = glm::eulerAngleY(initialRad);
		float angleFromMatrix = std::atan2(eulerYMat[2][0], eulerYMat[2][2]);

		EXPECT_TRUE(Math::eq(initialRad, angleFromMatrix));
	}
}

// ---- EulerZ ----
TEST(GLM, GetAngleFromEulerZ)
{
	for (int i = 0; i < 10; ++i)
	{
		float initialRad = glm::radians(generateFloat());
		auto eulerZMat = glm::eulerAngleZ(initialRad);
		float angleFromMatrix = glm::atan(eulerZMat[0][1], eulerZMat[0][0]);

		EXPECT_TRUE(Math::eq(initialRad, angleFromMatrix));
	}
}

TEST(EulerXTest, MatrixToDefaultsUpdateWithSynergies)
{
	auto rotXNode = Builder::createTransform<ETransformType::EulerX>();
	auto angle = glm::radians(generateFloat(0.0f, 90.0f));
	auto c = cosf(angle);
	auto s = sinf(angle);
	float val;

	// with synergies, initial quadrant is the first one (X+,Y+)
	// The matrix content:
	rotXNode->setValue(c, {1, 1});

	// set values od cos - precise
	val = rotXNode->getData().getMat4()[1][1];
	EXPECT_TRUE(Math::eq(c, val));

	val = rotXNode->getData().getMat4()[2][2];
	EXPECT_TRUE(Math::eq(c, val));

	// computed values of sin - not very precise
	val = rotXNode->getData().getMat4()[2][1];
	EXPECT_TRUE(Math::eq(-s, val, Math::FACTOR_ROUGHLY_SIMILAR));

	val = rotXNode->getData().getMat4()[1][2];
	EXPECT_TRUE(Math::eq(s, val, Math::FACTOR_ROUGHLY_SIMILAR));

	// The rotation
	float r = rotXNode->getDefaultValue("rotation").getFloat();
	EXPECT_TRUE(Math::eq(r, angle, Math::FACTOR_ROUGHLY_SIMILAR));

	auto expectedMatrix = glm::eulerAngleX(angle);
	EXPECT_TRUE(Math::eq(expectedMatrix, rotXNode->getData().getMat4(), Math::FACTOR_ROUGHLY_SIMILAR));
}

TEST(EulerXTest, MatrixToDefaultsUpdateWithoutSynergies)
{
	auto rotXNode = Builder::createTransform<ETransformType::EulerX>();
	auto angle = glm::radians(generateFloat(0.0f, 90.0f));
	auto c = cosf(angle);
	auto s = sinf(angle);
	float val;

	// without synergies, initial quadrant is the first one (X+,Y+)
	rotXNode->disableSynergies();
	// The matrix content:
	rotXNode->setValue(c, {1, 1});
	val = rotXNode->getData().getMat4()[1][1];
	EXPECT_TRUE(Math::eq(c, val));

	val = rotXNode->getData().getMat4()[2][2];
	EXPECT_TRUE(Math::eq(1.0f, val));

	val = rotXNode->getData().getMat4()[2][1];
	EXPECT_TRUE(Math::eq(0.0f, val));

	val = rotXNode->getData().getMat4()[1][2];
	EXPECT_TRUE(Math::eq(0.0f, val));

	// The rotation schould be unchanged
	float r = rotXNode->getDefaultValue("rotation").getFloat();
	EXPECT_TRUE(Math::eq(0.0f, r));
}

TEST(EulerYTest, MatrixToDefaultsUpdateWithSynergies)
{
	auto rotYNode = Builder::createTransform<ETransformType::EulerY>();
	auto angle = glm::radians(generateFloat(0.0, 90));
	auto c = cosf(angle);
	auto s = sinf(angle);
	float val;

	// with synergies, initial quadrant is the first one (X+,Y+)
	// The matrix content:
	rotYNode->setValue(c, {0, 0});
	val = rotYNode->getData().getMat4()[0][0];
	EXPECT_TRUE(Math::eq(c, val));
	// ASSERT_EQ(c, val);

	val = rotYNode->getData().getMat4()[2][2];
	EXPECT_TRUE(Math::eq(c, val));
	// ASSERT_EQ(c, val);

	val = rotYNode->getData().getMat4()[2][0];
	EXPECT_TRUE(Math::eq(s, val));
	// ASSERT_EQ(s, val);

	val = rotYNode->getData().getMat4()[0][2];
	EXPECT_TRUE(Math::eq(-s, val));
	// ASSERT_EQ(-s, val);

	// The rotation
	float r = rotYNode->getDefaultValue("rotation").getFloat();
	EXPECT_TRUE(Math::eq(r, angle));
	// ASSERT_EQ(r, angle);
}

TEST(EulerYTest, MatrixToDefaultsUpdateWithoutSynergies)
{
	auto rotYNode = Builder::createTransform<ETransformType::EulerY>();
	auto angle = glm::radians(generateFloat(0.0, 90));
	auto c = cosf(angle);
	auto s = sinf(angle);
	float val;

	// without synergies, initial quadrant is the first one (X+,Y+)
	rotYNode->disableSynergies();
	// The matrix content:
	rotYNode->setValue(c, {0, 0});
	val = rotYNode->getData().getMat4()[0][0];
	EXPECT_TRUE(Math::eq(c, val));

	val = rotYNode->getData().getMat4()[2][2];
	EXPECT_TRUE(Math::eq(1.0f, val));

	val = rotYNode->getData().getMat4()[2][0];
	EXPECT_TRUE(Math::eq(0.0f, val));

	val = rotYNode->getData().getMat4()[0][2];
	EXPECT_TRUE(Math::eq(0.0f, val));

	// The rotation should be unchanged
	float r = rotYNode->getDefaultValue("rotation").getFloat();
	EXPECT_TRUE(Math::eq(0.0f, r));
}

TEST(EulerZTest, MatrixToDefaultsUpdateWithSynergies)
{
	auto rotZNode = Builder::createTransform<ETransformType::EulerZ>();
	auto angle = glm::radians(generateFloat(0.0, 90));
	auto c = cosf(angle);
	auto s = sinf(angle);
	float val;

	// with synergies, initial quadrant is the first one (X+,Y+)
	// The matrix content:
	rotZNode->setValue(c, {0, 0});
	val = rotZNode->getData().getMat4()[0][0];
	EXPECT_TRUE(Math::eq(c, val));

	val = rotZNode->getData().getMat4()[1][1];
	EXPECT_TRUE(Math::eq(c, val));

	val = rotZNode->getData().getMat4()[1][0];
	EXPECT_TRUE(Math::eq(-s, val));

	val = rotZNode->getData().getMat4()[0][1];
	EXPECT_TRUE(Math::eq(s, val));

	// The rotation should be updated to the angle (from the cosine(angle) value)
	float r = rotZNode->getDefaultValue("rotation").getFloat();
	EXPECT_TRUE(Math::eq(r, angle));
}

TEST(EulerZTest, MatrixToDefaultsUpdateWithoutSynergies)
{
	auto rotZNode = Builder::createTransform<ETransformType::EulerZ>();
	auto angle = glm::radians(generateFloat(0.0, 90));
	auto c = cosf(angle);
	auto s = sinf(angle);
	float val;

	// without synergies, initial quadrant is the first one (X+,Y+)
	rotZNode->disableSynergies();
	// The matrix content:
	rotZNode->setValue(c, {0, 0});
	val = rotZNode->getData().getMat4()[0][0];
	EXPECT_TRUE(Math::eq(c, val));

	val = rotZNode->getData().getMat4()[1][1];
	EXPECT_TRUE(Math::eq(1.0f, val));

	val = rotZNode->getData().getMat4()[0][1];
	EXPECT_TRUE(Math::eq(0.0f, val));

	val = rotZNode->getData().getMat4()[1][0];
	EXPECT_TRUE(Math::eq(0.0f, val));

	// The rotation should be unchanged
	float r = rotZNode->getDefaultValue("rotation").getFloat();
	EXPECT_TRUE(Math::eq(0.0f, r));
}

//===-- Euler rotation around X axis --------------------------------------===//

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
		EXPECT_TRUE(rotXNode->isValid());
	}
	{
		// mat[1][2], sin(T)
		auto rads = 0.4f;

		setValue_expectOk(rotXNode, glm::sin(rads), {1, 2});

		auto mat = rotXNode->getData().getMat4();
		auto expectedMat = glm::rotate(rads, glm::vec3(1.0f, 0.0f, 0.0f));

		EXPECT_TRUE(Math::eq(expectedMat, mat));
		EXPECT_TRUE(rotXNode->isValid());
	}
	{
		// mat[2][1], -sin(T)
		auto rads = 0.8f;

		setValue_expectOk(rotXNode, -glm::sin(rads), {2, 1});

		auto mat = rotXNode->getData().getMat4();
		auto expectedMat = glm::rotate(rads, glm::vec3(1.0f, 0.0f, 0.0f));

		EXPECT_TRUE(Math::eq(expectedMat, mat));
		EXPECT_TRUE(rotXNode->isValid());
	}
	{
		// mat[2][2], cos(T)
		auto rads = 1.0f;

		setValue_expectOk(rotXNode, glm::cos(rads), {2, 2});

		auto mat = rotXNode->getData().getMat4();
		auto expectedMat = glm::rotate(rads, glm::vec3(1.0f, 0.0f, 0.0f));

		EXPECT_TRUE(Math::eq(expectedMat, mat));
		EXPECT_TRUE(rotXNode->isValid());
	}
}

/**
 * 1. start with identity -> should be valid
 * 2. disable Synergies - the other three rotation values remain unchanged
 * 3. set one of them to different value - the other three remain  unchanged ->
 *    invalid
 * 4. enable synergies - all four rotation values should be changed
 * 5. set one of them to different value - the other three must be updated ->
 *    valid
 *
 * \code
 *   1      0       0       0
 *   0    cos(T)  -sin(T)   0
 *   0   >sin(T)<  cos(T)   0
 *   0      0       0       1
 * \endcode
 */

TEST(EulerXTest,
     Synergies__Disabled_Invalid__Enabled_Valid__WRONG_ON_GITLAB) // fails
{
	auto rot = Builder::createTransform<ETransformType::EulerX>();

	EXPECT_TRUE(rot->isValid()); // should be the identity

	// noSynergies => single value invalidates the matrix
	rot->disableSynergies();

	// auto wrongVal = generateFloat(-1.0f, 1.0f);
	// EXPECT_TRUE(Math::withinInterval(wrongVal, -1.0f, 1.0f));

	auto wrongVal = 0.5f; // sin(60degs)

	setValue_expectOk(rot, wrongVal, {1, 2}); // should be sin(of some angle)

	// std::cerr << "Matrix[1,2] = " << rot->getData().getMat4()[1][2] << ", value should be " << wrongVal << std::endl;
	// std::cerr << "Matrix[2,1] = " << rot->getData().getMat4()[2][1] << ", value should be " << 0.0f << std::endl;
	EXPECT_TRUE(Math::eq(rot->getData().getMat4()[1][2], wrongVal));
	EXPECT_TRUE(Math::eq(rot->getData().getMat4()[1][1],
	                     1.0f)); // was identity, no synergies => should be unchanged

	// std::cerr << "--------------------- START WRONG TEST -------------------" << std::endl;
	EXPECT_FALSE(rot->isValid()); // todo PF gitlab returns valid for a corrupted
	                              // matrix - that is wrong
	// std::cerr << "--------------------- END WRONG TEST ---------------------" << std::endl;

	// synergies repair the matrix - update the angle
	rot->enableSynergies();
	setValue_expectOk(rot, wrongVal, {1, 2}); // synergies will use this as sin(angle)

	// std::cerr << "Matrix[1,2] = " << rot->getData().getMat4()[1][2] << ", value should be " << wrongVal << std::endl;
	// std::cerr << "Matrix[2,1] = " << rot->getData().getMat4()[2][1] << ", value should be " << -wrongVal <<
	// std::endl;

	EXPECT_TRUE(Math::eq(rot->getData().getMat4()[1][2], wrongVal));  //  sin
	EXPECT_TRUE(Math::eq(rot->getData().getMat4()[2][1], -wrongVal)); // -sin
	EXPECT_TRUE(rot->isValid());
}

TEST(EulerXTest, Unlocked_InvalidState__WRONG_ON_GITLAB)
{
	auto rot = Builder::createTransform<ETransformType::EulerX>();

	// single value invalidates the matrix

	rot->unlock();

	setValue_expectOk(rot, 2.0f, {0, 2}); // non-editable value - should be 0

	EXPECT_FALSE(rot->isValid());
}

//
// TEST(EulerXTest, Unlocked_WrongValue_InvalidState) // fails
//{
//	auto rot = Builder::createTransform<ETransformType::EulerX>();
//	rot->unlock();
//
//	auto rads = generateAngleFromDegs(0.0f,360.0f);
//
//	setValue_expectOk(rot, glm::sin(rads), {2, 3}); //
//
//	EXPECT_FALSE(rot->isValid());
//}

TEST(EulerXTest, SetMatrixShouldBeValid)
{
	float initialRad = generateAngleFromDegs(-90.0f, 90.0f);
	auto eulerX = Builder::createTransform<ETransformType::EulerX>();
	eulerX->setDefaultValue("rotation", initialRad);

	auto newRad = generateAngleFromDegs(-90.0f, 90.0f);
	auto mat = glm::eulerAngleX(newRad);

	setValue_expectOk(eulerX, mat);

	EXPECT_TRUE(Math::eq(mat, eulerX->getData().getMat4(), 100 * Math::FACTOR_ROUGHLY_SIMILAR))
	    << Utils::toString(mat) << std::endl
	    << "!=\n"
	    << Utils::toString(eulerX->getData().getMat4()) << std::endl;
	; // get what you set
	auto storedRad = eulerX->getDefaultValue("rotation").getFloat();
	EXPECT_FALSE(Math::eq(initialRad, storedRad, Math::FACTOR_ROUGHLY_SIMILAR)); // setValue updated the Default
	EXPECT_TRUE(Math::eq(newRad, storedRad, Math::FACTOR_ROUGHLY_SIMILAR));

	eulerX->resetMatrixFromDefaults();
	const auto expectedMat = glm::eulerAngleX(initialRad);
	const auto currentMat = eulerX->getData().getMat4();
	EXPECT_FALSE(Math::eq(expectedMat, currentMat, 10 * Math::FACTOR_ROUGHLY_SIMILAR)); //
}

//===-- Euler rotation around Y axis --------------------------------------===//

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

TEST(EulerYTest, SetMatrixShouldBeValid)
{
	// float initialRad = generateFloat(-(M_PI / 2.0f), M_PI / 2.0f);
	float initialRad = generateAngleFromDegs(-90.0f, 90.0f);
	// auto eulerY = Builder::createTransform<EulerRotY>(initialRad);
	auto eulerY = Builder::createTransform<ETransformType::EulerY>();
	eulerY->setDefaultValue("rotation", initialRad);

	auto newRad = generateAngleFromDegs(-90.0f, 90.0f);
	auto mat = glm::eulerAngleY(newRad);

	setValue_expectOk(eulerY, mat);
	// auto setResult = eulerY->setValue(mat);

	// EXPECT_EQ(mat, eulerY->getData().getMat4());
	// EXPECT_EQ(ValueSetResult::Status::Ok, setResult.status);
	EXPECT_TRUE(Math::eq(mat, eulerY->getData().getMat4())); // get what you set
	auto storedRad = eulerY->getDefaultValue("rotation").getFloat();

	EXPECT_FALSE(Math::eq(initialRad, storedRad)); // setValue updated the Default
	EXPECT_TRUE(Math::eq(newRad, storedRad));

	eulerY->resetMatrixFromDefaults();
	auto expectedMat = glm::eulerAngleZ(initialRad);
	auto currentMat = eulerY->getData().getMat4();
	EXPECT_FALSE(Math::eq(expectedMat, currentMat)); //
}

//===-- Euler rotation around Z axis --------------------------------------===//

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

TEST(EulerZTest, SetMatrixShouldBeValid)
{
	float initialRad = generateAngleFromDegs(-90.0f, 90.0f);
	auto eulerZ = Builder::createTransform<ETransformType::EulerZ>();
	eulerZ->setDefaultValue("rotation", initialRad);

	auto newRad = generateAngleFromDegs(-90.0f, 90.0f);
	auto mat = glm::eulerAngleZ(newRad);

	setValue_expectOk(eulerZ, mat);
	// EXPECT_EQ(mat, eulerZ->getData().getMat4());
	EXPECT_TRUE(Math::eq(mat, eulerZ->getData().getMat4())); // get what you set
	auto storedRad = eulerZ->getDefaultValue("rotation").getFloat();

	EXPECT_FALSE(Math::eq(initialRad, storedRad)); // setValue updated the Default
	EXPECT_TRUE(Math::eq(newRad, storedRad));

	eulerZ->resetMatrixFromDefaults();
	auto expectedMat = glm::eulerAngleZ(initialRad);
	auto currentMat = eulerZ->getData().getMat4();
	EXPECT_FALSE(Math::eq(expectedMat, currentMat)); //
}

//===-- Euler rotation other tests ... ------------------------------------===//

TEST(EulerTest, XYZAngleSetShouldBeCorrect)
{
	// setting of euler angles to matrices creates the as matrices as rotating
	// around individual axes
	std::array<Ptr<Node>, 3> rots = {
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
	auto axisAngle =
	    Builder::createTransform<ETransformType::AxisAngle>()->as<TransformImpl<ETransformType::AxisAngle>>();

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
/* To test:
 * - isValid  --- norm = 1  (plati i pro nesmyslnou matici)
 * - hasMenuSynergies - true
 *   po nahrani - hasSynergies true - jednotkova
 * - setValue(vec4)  - quat + setInternalValue(toMat4)   /// does not work - do
 * not use it
 * - setValue(quat)  - quat + setInternalValue(toMat4)
 *
 * quat representation (w,x,y,z) or (s, v) - scalar + vector
 */
TEST(QuatRotTest, NewQuat__HasSynergies_isValid_isNormalized)
{
	auto rot = Builder::createTransform<ETransformType::Quat>()->as<TransformImpl<ETransformType::Quat>>();

	EXPECT_TRUE(rot->hasSynergies());
	EXPECT_TRUE(rot->isValid()); // of unit size
	EXPECT_EQ(rot->getQuat(),
	          rot->getNormalizedQuat()); // normalized default = m_normalized

	// matrix is OK
	const glm::quat quat0 = rot->getDefaultValue("quat").getQuat();
	EXPECT_EQ(rot->getData().getMat4(), glm::toMat4(glm::normalize(quat0)));
}

TEST(QuatRotTest, getNormalized_returns_normalized)
{
	auto rot = Builder::createTransform<ETransformType::Quat>()->as<TransformImpl<ETransformType::Quat>>();

	// setValue WITH SYNERGIES - should normalize m_normalize regardless of synergies and stored quat due to synergies
	auto vec = generateNonUnitVec4();
	auto quat = glm::quat(vec.w, vec.x, vec.y,
	                      vec.z); // not normalized + synergies=> normalized

	setValue_expectOk(rot, quat);

	EXPECT_TRUE(Math::isNormalized(rot->getNormalizedQuat())); // m_normalized is normalized (by definition)
	EXPECT_TRUE(Math::isNormalized(rot->getQuat()));           // stored quat is normalized (due to synergies)


	// setValue WITHOUT SYNERGIES - should NOT normalize the stored quat
	// -----------------------------------------------------
	rot->disableSynergies(); // not of unit length
	quat = glm::quat(vec.w, vec.x, vec.y,
	                 vec.z); // not normalized - synergies=> NOT normalized
	setValue_expectOk(rot, quat);

	EXPECT_TRUE(Math::isNormalized(rot->getNormalizedQuat())); // m_normalized is normalized (by definition)
	EXPECT_FALSE(Math::isNormalized(rot->getQuat()));          // stored quat is normalized (due to synergies)
}

TEST(QuatRotTest, isValidPerPartesTest_Identity)
{
	auto rot = Builder::createTransform<ETransformType::Quat>()->as<TransformImpl<ETransformType::Quat>>();


	// internal matrix test
	auto& mat = rot->data().getMat4();

	// det = 1
	EXPECT_TRUE(Math::eqToOne(glm::determinant(mat))) << std::endl
	                                                  << "determinant " << glm::determinant(mat) << " != 1" << std::endl
	                                                  << Utils::toString(mat, true) << std::endl;


	// quaternion is defined and normalized
	const glm::quat quaternion1 = rot->getDefaultValue("quat").getQuat();
	const glm::quat quaternion2 = rot->getNormalizedQuat();

	EXPECT_TRUE(Math::isNormalized(quaternion1)) << "quat NOT normalized" << std::endl
	                                             << Utils::toString(quaternion1) << std::endl;

	EXPECT_TRUE(Math::isNormalized(quaternion2)) << "quat NOT normalized" << std::endl
	                                             << Utils::toString(quaternion2) << std::endl;
	EXPECT_EQ(quaternion1, quaternion2);


	// mat and default quaternion match
	EXPECT_EQ(mat, glm::toMat4(quaternion1)) << "stored mat != mat(quaternion)" << std::endl
	                                         << Utils::toString(mat, true) << std::endl;
}

TEST(QuatRotTest, isValidPerPartesTest_Modified)
{
	auto rot = Builder::createTransform<ETransformType::Quat>()->as<TransformImpl<ETransformType::Quat>>();

	glm::quat q = glm::quat(1.0f, 2.0f, 3.0f, 4.0f);
	rot->setValue(q);

	// internal matrix test
	auto& mat = rot->data().getMat4();

	// det = 1
	EXPECT_TRUE(Math::eqToOne(glm::determinant(mat))) << std::endl
	                                                  << "determinant " << glm::determinant(mat) << " != 1" << std::endl
	                                                  << Utils::toString(mat, true) << std::endl;


	// quaternion is defined and normalized
	const glm::quat quaternion1 = rot->getDefaultValue("quat").getQuat();
	const glm::quat quaternion2 = rot->getNormalizedQuat();

	EXPECT_TRUE(Math::isNormalized(quaternion1)) << "quat NOT normalized" << std::endl
	                                             << Utils::toString(quaternion1) << std::endl;

	EXPECT_TRUE(Math::isNormalized(quaternion2)) << "quat NOT normalized" << std::endl
	                                             << Utils::toString(quaternion2) << std::endl;
	EXPECT_EQ(quaternion1, quaternion2);


	// mat and default quaternion match
	EXPECT_EQ(mat, glm::toMat4(quaternion1)) << "stored mat != mat(quaternion)" << std::endl
	                                         << Utils::toString(mat, true) << std::endl;
}

TEST(QuatRotTest, DataGetters_withoutHysteresis)
{
	auto rot = Builder::createTransform<ETransformType::Quat>()->as<TransformImpl<ETransformType::Quat>>();
	auto unitQuat = generateUnitQuat();

	rot->setDefaultValue("quat", unitQuat); // bound to 1.2 and then normalized in setDefaultValueWithSynergies()

	auto& mat1 = rot->getInternalData().getMat4();
	auto& mat2 = rot->getData().getMat4();
	auto& mat3 = rot->data().getMat4();

	EXPECT_PRED_FORMAT2(AssertEqualMatrices, mat1, mat2);
	EXPECT_PRED_FORMAT2(AssertEqualMatrices, mat1, mat3);
	EXPECT_EQ(mat1, mat2);
	EXPECT_EQ(mat1, mat3);
}
TEST(QuatRotTest, DataGetters_withHysteresis)
{
	constexpr float HYSTERESIS_ONE =
	    1.2f; // same as is in TransformImpl<ETransformType::Quat>::setDefaultValueWithSynergies()

	auto rot = Builder::createTransform<ETransformType::Quat>()->as<TransformImpl<ETransformType::Quat>>();
	auto unitQuat = generateUnitQuat();

	auto q = unitQuat;
	q.x = HYSTERESIS_ONE + 0.1f;
	rot->setDefaultValue("quat", q); // bound to 1.2 and then normalized in setDefaultValueWithSynergies()
	EXPECT_TRUE(Math::eqToOne(rot->getQuat().x));
	EXPECT_TRUE(Math::eqToZero(rot->getQuat().y));
	EXPECT_TRUE(Math::eqToZero(rot->getQuat().z));
	EXPECT_TRUE(Math::eqToZero(rot->getQuat().w));

	q = unitQuat;
	q.y = HYSTERESIS_ONE + 0.1f;
	rot->setDefaultValue("quat", q); // bound to 1.2 and then normalized in setDefaultValueWithSynergies()
	EXPECT_TRUE(Math::eqToOne(rot->getQuat().y));
	EXPECT_TRUE(Math::eqToZero(rot->getQuat().x));
	EXPECT_TRUE(Math::eqToZero(rot->getQuat().z));
	EXPECT_TRUE(Math::eqToZero(rot->getQuat().w));

	q = unitQuat;
	q.z = HYSTERESIS_ONE + 0.1f;
	rot->setDefaultValue("quat", q); // bound to 1.2 and then normalized in setDefaultValueWithSynergies()
	EXPECT_TRUE(Math::eqToOne(rot->getQuat().z));
	EXPECT_TRUE(Math::eqToZero(rot->getQuat().x));
	EXPECT_TRUE(Math::eqToZero(rot->getQuat().y));
	EXPECT_TRUE(Math::eqToZero(rot->getQuat().w));

	q = unitQuat;
	q.w = HYSTERESIS_ONE + 0.1f;
	rot->setDefaultValue("quat", q); // bound to 1.2 and then normalized in setDefaultValueWithSynergies()
	EXPECT_TRUE(Math::eqToOne(rot->getQuat().w));
	EXPECT_TRUE(Math::eqToZero(rot->getQuat().x));
	EXPECT_TRUE(Math::eqToZero(rot->getQuat().y));
	EXPECT_TRUE(Math::eqToZero(rot->getQuat().z));
}
TEST(QuatRotTest, HandlingSynergies_setValue_expectOk) // internal function setValue_expectOk
{
	// std::cerr << "Entering QuatRotTest" << std::endl;
	// EXPECT_TRUE(false) << "Entering QuatRotTest" << std::endl;

	auto rot = Builder::createTransform<ETransformType::Quat>()->as<TransformImpl<ETransformType::Quat>>();

	// auto vec = generateNonUnitVec4();
	// auto quat = glm::quat(vec.w, glm::vec3(vec)); // not normalized
	auto nonUnitQuat = generateNonUnitQuat();

	// setValue WITH SYNERGIES - should normalize, be valid
	// -----------------------------------------------------
	setValue_expectOk(rot, nonUnitQuat); // synergies => normalized in resetMatrixFromDefaults()

	EXPECT_TRUE(rot->isValid());                         // default quat of unit size
	EXPECT_EQ(rot->getQuat(), rot->getNormalizedQuat()); // is normalized
	                                                     // (default = m_normalized)

	// the matrix is set correctly based on the normalized quaternion
	EXPECT_EQ(rot->getData().getMat4(),
	          glm::toMat4(glm::normalize(nonUnitQuat))); // same matrix as from normalized


	// setValue WITHOUT SYNERGIES - should NOT normalize, be NOT valid
	// -----------------------------------------------------
	rot->disableSynergies(); // not of unit length

	setValue_expectOk(rot, nonUnitQuat); // NO synergies=> not normalized

	EXPECT_FALSE(rot->isValid()); // NOT of unit size
	EXPECT_NE(rot->getQuat(),
	          rot->getNormalizedQuat()); // default quat is left non-normalized

	// the matrix is set correctly based on the normalized quaternion
	EXPECT_EQ(rot->getData().getMat4(),
	          glm::toMat4(glm::normalize(nonUnitQuat))); // same matrix as from normalized
}

TEST(QuatRotTest, Hysteresis)
{
	auto rot = Builder::createTransform<ETransformType::Quat>()->as<TransformImpl<ETransformType::Quat>>();
	rot->setDefaultValue("quat", glm::quat(2.0f, 1.0f, 1.0f, 1.0f)); // hysteresis on X

	auto q = rot->getQuat();
}
TEST(QuatRotTest, HandlingSynergies_setDefaultValueWithSynergiesAndHysteresis)
{
	auto rot = Builder::createTransform<ETransformType::Quat>()->as<TransformImpl<ETransformType::Quat>>();
	EXPECT_TRUE(rot->hasSynergies()); // by definition

	auto nonUnitQuat = generateNonUnitQuat();

	// ----------------------------------------------------------------------
	// setValue WITH SYNERGIES - should normalize (change) also the default
	// ----------------------------------------------------------------------

	rot->setDefaultValue("quat", nonUnitQuat); // normalized in setDefaultValueWithSynergies()

	EXPECT_TRUE(rot->hasSynergies()); // flag survives

	EXPECT_TRUE(Math::isNormalized(rot->getQuat()));
	EXPECT_TRUE(Math::isNormalized(rot->getNormalizedQuat()));

	EXPECT_EQ(rot->getQuat(), rot->getNormalizedQuat()); // normalized default = m_normalized

	// invalid due to hysteresis replacement in setDefaultValueWithSynergies()
	// EXPECT_PRED_FORMAT2(AssertEqualMatrices, rot->getData().getMat4(),
	//                     glm::toMat4(glm::normalize(nonUnitQuat))); // same matrix as from normalized

	EXPECT_PRED_FORMAT2(
	    AssertEqualMatrices, rot->getData().getMat4(),
	    glm::toMat4(glm::normalize(rot->getQuat()))); // same matrix as from quat, that is also normalized

	EXPECT_PRED_FORMAT2(AssertEqualMatrices, rot->getData().getMat4(),
	                    glm::toMat4(rot->getNormalizedQuat())); // same matrix as from normalized
}

TEST(QuatRotTest, HandlingSynergies_setDefaultValueNoSynergies)
{
	auto rot = Builder::createTransform<ETransformType::Quat>()->as<TransformImpl<ETransformType::Quat>>();
	EXPECT_TRUE(rot->hasSynergies()); // by definition

	auto nonUnitQuat = generateNonUnitQuat();

	// ----------------------------------------------------------------------
	// setValue WITHOUT SYNERGIES - should NOT normalize
	// ----------------------------------------------------------------------

	rot->disableSynergies();           // not of unit length
	EXPECT_FALSE(rot->hasSynergies()); // flag just changed

	rot->setDefaultValue("quat", nonUnitQuat); // set NOT normalized

	EXPECT_FALSE(rot->hasSynergies()); // flag survives

	EXPECT_FALSE(Math::isNormalized(rot->getQuat()));
	EXPECT_TRUE(Math::isNormalized(rot->getNormalizedQuat()));

	EXPECT_NE(rot->getQuat(), rot->getNormalizedQuat()); // default is left non-normalized

	// invalid due to hysteresis replacement in setDefaultValueWithSynergies()
	// EXPECT_PRED_FORMAT2(AssertEqualMatrices, rot->getData().getMat4(),
	//                     glm::toMat4(glm::normalize(nonUnitQuat))); // same matrix as from normalized

	EXPECT_PRED_FORMAT2(AssertEqualMatrices, rot->getData().getMat4(),
	                    glm::toMat4(glm::normalize(rot->getQuat()))); // same matrix as from normalized(quat)

	EXPECT_PRED_FORMAT2(AssertEqualMatrices, rot->getData().getMat4(),
	                    glm::toMat4(rot->getNormalizedQuat())); // same matrix as from normalized
}

TEST(GLM, quaternion_ConstructorVec4versusQuat)
{
	// quat from four floats does not work - it creates a quaternion as if x,y,z were euler angles ignoring w
	// auto vec = generateNonUnitVec4();
	// auto quat = glm::quat(vec); // wrong - it creates a quaternion as if x,y,z were euler angles ignoring w
	// auto quat = glm::quat(vec.w, glm::vec3(vec)); // correct
	glm::vec4 vec = generateNonUnitVec4();

	auto quatFromFloats = glm::quat(vec.w, vec.x, vec.y, vec.z); // not normalized
	auto quatFromVec4 = glm::quat(vec.w, glm::vec3(vec));        // uses vec3 as axis and w as angle
	// wrong - takes vec3 as euler angles
	// auto quatFromVec4 = glm::quat(vec);                          // uses vec3 as axis and w as angle

	// quat from components == quat from vec4
	EXPECT_TRUE(assertionCompare(quatFromFloats, quatFromVec4,
	                             Math::FACTOR_NEARLY_EQUAL)); // should be OK - test of the constructors
	EXPECT_EQ(quatFromFloats.x, vec.x);
	EXPECT_EQ(quatFromFloats.y, vec.y);
	EXPECT_EQ(quatFromFloats.z, vec.z);
	EXPECT_EQ(quatFromFloats.w, vec.w);

	// Order of normalization is not important
	auto q1 = glm::normalize(quatFromVec4);
	auto q2 = glm::quat(glm::normalize(vec));

	EXPECT_FALSE(assertionCompare(q1, q2, Math::FACTOR_NEARLY_EQUAL));
}
TEST(GLM, quaternionGenerator)
{
	auto unitQuat = generateUnitQuat();
	auto nonUnitQuat = generateNonUnitQuat();

	EXPECT_TRUE(Math::eqToOne(glm::length(unitQuat)));
	EXPECT_TRUE(Math::isNormalized(unitQuat));

	EXPECT_FALSE(Math::eqToOne(glm::length(nonUnitQuat)));
	EXPECT_FALSE(Math::isNormalized(nonUnitQuat));
}

} // namespace rotations
