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
	lhs = glm::rotate(initialRad, glm::vec3(0.0f, 0.0f, 1.0f));
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

TEST(EulerXTest, MatrixToDefaultsUpdateWithSynergies)
{
	auto rotXNode = Builder::createTransform<ETransformType::EulerX>();
	auto angle    = glm::radians(generateFloat(0.0,90));
	auto  c        = cosf(angle);
	auto  s        = sinf(angle);
	float val;

	// with synergies, initial quadrant is the first one (X+,Y+)
	// The matrix content:
	rotXNode->setValue(c, {1, 1});
	val = rotXNode->getData().getMat4()[1][1];
	EXPECT_TRUE(Math::eq(c, val));

	val = rotXNode->getData().getMat4()[2][2];
	EXPECT_TRUE(Math::eq(c, val));

	val = rotXNode->getData().getMat4()[2][1];
	EXPECT_TRUE(Math::eq(-s, val));

	val = rotXNode->getData().getMat4()[1][2];
	EXPECT_TRUE(Math::eq(s, val));

	// The rotation
	float r = rotXNode->getDefaultValue("rotation").getFloat();
	EXPECT_TRUE(Math::eq(r, angle));

	auto expectedMatrix = glm::eulerAngleX(angle);
	EXPECT_TRUE(Math::eq(expectedMatrix, rotXNode->getData().getMat4()));
}

TEST(EulerXTest, MatrixToDefaultsUpdateWithoutSynergies)
{
	auto  rotXNode = Builder::createTransform<ETransformType::EulerX>();
	auto  angle    = glm::radians(generateFloat(0.0, 90));
	auto  c        = cosf(angle);
	auto  s        = sinf(angle);
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
	auto  rotYNode = Builder::createTransform<ETransformType::EulerY>();
	auto  angle    = glm::radians(generateFloat(0.0, 90));
	auto  c        = cosf(angle);
	auto  s        = sinf(angle);
	float val;

	// with synergies, initial quadrant is the first one (X+,Y+)
	// The matrix content:
	rotYNode->setValue(c, {0, 0});
	val = rotYNode->getData().getMat4()[0][0];
	EXPECT_TRUE(Math::eq(c, val));

	val = rotYNode->getData().getMat4()[2][2];
	EXPECT_TRUE(Math::eq(c, val));

	val = rotYNode->getData().getMat4()[2][0];
	EXPECT_TRUE(Math::eq(s, val));

	val = rotYNode->getData().getMat4()[0][2];
	EXPECT_TRUE(Math::eq(-s, val));

	// The rotation
	float r = rotYNode->getDefaultValue("rotation").getFloat();
	EXPECT_TRUE(Math::eq(r, angle));
}

TEST(EulerYTest, MatrixToDefaultsUpdateWithoutSynergies)
{
	auto  rotYNode = Builder::createTransform<ETransformType::EulerY>();
	auto  angle    = glm::radians(generateFloat(0.0, 90));
	auto  c        = cosf(angle);
	auto  s        = sinf(angle);
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

	// The rotation schould be unchanged
	float r = rotYNode->getDefaultValue("rotation").getFloat();
	EXPECT_TRUE(Math::eq(0.0f, r));
}

TEST(EulerZTest, MatrixToDefaultsUpdateWithSynergies)
{
	auto  rotZNode = Builder::createTransform<ETransformType::EulerZ>();
	auto  angle    = glm::radians(generateFloat(0.0, 90));
	auto  c        = cosf(angle);
	auto  s        = sinf(angle);
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

	// The rotation
	float r = rotZNode->getDefaultValue("rotation").getFloat();
	EXPECT_TRUE(Math::eq(r, angle));
}

TEST(EulerZTest, MatrixToDefaultsUpdateWithoutSynergies)
{
	auto  rotZNode = Builder::createTransform<ETransformType::EulerZ>();
	auto  angle    = glm::radians(generateFloat(0.0, 90));
	auto  c        = cosf(angle);
	auto  s        = sinf(angle);
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

	// The rotation schould be unchanged
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

TEST(EulerXTest, SynergiesDisabled_OneCorrectValue_InvalidState__WRONG_ON_GITLAB) // fails
{
	auto rot = Builder::createTransform<ETransformType::EulerX>();


	// noSynergies => single value invalidates the matrix
	rot->disableSynergies();

	auto wrongVal = generateFloat(-1.0f, 1.0f);
	EXPECT_TRUE(Math::withinInterval(wrongVal, -1.0f, 1.0f));

	setValue_expectOk(rot, wrongVal, {1, 2}); // should be sin(of some angle)

	EXPECT_EQ(ETransformState::Invalid, rot->isValid());  //todo PF gitlab returns valid for a corrupted matrix - that is wrong


	// synergies repair the matrix
	rot->enableSynergies(); 
	setValue_expectOk(rot, wrongVal, {1, 2});  // synergies will use this as sin(angle)
	EXPECT_EQ(ETransformState::Valid, rot->isValid());
}

TEST(EulerXTest, Unlocked_InvalidState__WRONG_ON_GITLAB)
{
	auto rot = Builder::createTransform<ETransformType::EulerX>();

	// single value invalidates the matrix

	rot->unlock();

	setValue_expectOk(rot, 2.0f, {1, 0}); // non-editable value

	EXPECT_EQ(ETransformState::Invalid, rot->isValid());
}
 
//
//TEST(EulerXTest, Unlocked_WrongValue_InvalidState) // fails
//{
//	auto rot = Builder::createTransform<ETransformType::EulerX>();
//	rot->unlock();
//
//	auto rads = generateAngleFromDegs(0.0f,360.0f);
//
//	setValue_expectOk(rot, glm::sin(rads), {2, 3}); //
//
//	EXPECT_EQ(ETransformState::Invalid, rot->isValid());
//}


TEST(EulerXTest, SetMatrixShouldBeValid)
{
	float initialRad = generateAngleFromDegs(-90.0f, 90.0f);
	auto eulerX = Builder::createTransform<ETransformType::EulerX>();
	eulerX->setDefaultValue("rotation", initialRad);

	auto newRad = generateAngleFromDegs(-90.0f, 90.0f);
	auto mat = glm::eulerAngleX(newRad);

	setValue_expectOk(eulerX, mat);

	EXPECT_TRUE(Math::eq(mat, eulerX->getData().getMat4()));   // get what you set
	auto storedRad = eulerX->getDefaultValue("rotation").getFloat();
	EXPECT_FALSE(Math::eq(initialRad, storedRad));  // setValue updated the Default
	EXPECT_TRUE(Math::eq(newRad, storedRad));

	eulerX->resetMatrixFromDefaults();
	const auto expectedMat = glm::eulerAngleX(initialRad);
	const auto currentMat  = eulerX->getData().getMat4();
	EXPECT_FALSE(Math::eq(expectedMat, currentMat));  // 
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
	//float initialRad = generateFloat(-(M_PI / 2.0f), M_PI / 2.0f);
	float initialRad = generateAngleFromDegs(-90.0f, 90.0f);
	//auto eulerY = Builder::createTransform<EulerRotY>(initialRad);
	auto eulerY = Builder::createTransform<ETransformType::EulerY>();
	eulerY->setDefaultValue("rotation", initialRad);

	auto newRad = generateAngleFromDegs(-90.0f, 90.0f);
	auto mat    = glm::eulerAngleY(newRad);

	setValue_expectOk(eulerY, mat);
	//auto setResult = eulerY->setValue(mat);

	//EXPECT_EQ(mat, eulerY->getData().getMat4());
	//EXPECT_EQ(ValueSetResult::Status::Ok, setResult.status);
	EXPECT_TRUE(Math::eq(mat, eulerY->getData().getMat4())); // get what you set
	auto storedRad = eulerY->getDefaultValue("rotation").getFloat();

	EXPECT_FALSE(Math::eq(initialRad, storedRad)); // setValue updated the Default
	EXPECT_TRUE(Math::eq(newRad, storedRad));

	eulerY->resetMatrixFromDefaults();
	auto expectedMat = glm::eulerAngleZ(initialRad);
	auto currentMat  = eulerY->getData().getMat4();
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
	auto  eulerZ     = Builder::createTransform<ETransformType::EulerZ>();
	eulerZ->setDefaultValue("rotation", initialRad);

	auto newRad = generateAngleFromDegs(-90.0f, 90.0f);
	auto mat    = glm::eulerAngleZ(newRad);

	setValue_expectOk(eulerZ, mat);
	//EXPECT_EQ(mat, eulerZ->getData().getMat4());
	EXPECT_TRUE(Math::eq(mat, eulerZ->getData().getMat4())); // get what you set
	auto storedRad = eulerZ->getDefaultValue("rotation").getFloat();

	EXPECT_FALSE(Math::eq(initialRad, storedRad)); // setValue updated the Default
	EXPECT_TRUE(Math::eq(newRad, storedRad));

	eulerZ->resetMatrixFromDefaults();
	auto expectedMat = glm::eulerAngleZ(initialRad);
	auto currentMat  = eulerZ->getData().getMat4();
	EXPECT_FALSE(Math::eq(expectedMat, currentMat)); //
}

//===-- Euler rotation other tests ... ------------------------------------===//

TEST(EulerTest, XYZAngleSetShouldBeCorrect)
{
	// setting of euler angles to matrices creates the as matrices as rotating around individual axes
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
/* To test:
 * - isValid  --- norm = 1  (platí i pro nesmyslnou matici)
 * - hasMenuSynergies - true
 *   po nahrání - hasSynergies true - jednotkový
 * - setValue(vec4)  - quat + setInternalValue(toMat4)   /// does not work - do not use it
 * - setValue(quat)  - quat + setInternalValue(toMat4)
 *
 * quat representation (w,x,y,z) or (s, v) - scalar + vector
 */
TEST(QuatRotTest, NewQuat__HasSynergies_isValid_isNormalized)
{
	auto rot = Builder::createTransform<ETransformType::Quat>()->as<TransformImpl<ETransformType::Quat>>();


	EXPECT_TRUE(rot->hasSynergies());
	EXPECT_EQ(rot->isValid(), ETransformState::Valid); // of unit size
	EXPECT_EQ(rot->getQuat(), rot->getNormalized());   // normalized default = m_normalized

	// matrix is OK
	const glm::quat quat0 = rot->getDefaultValue("quat").getQuat();
	EXPECT_EQ(rot->getData().getMat4(), glm::toMat4(glm::normalize(quat0)));
}

TEST(QuatRotTest, getNormalized_returns_normalized)
{
	auto rot = Builder::createTransform<ETransformType::Quat>()->as<TransformImpl<ETransformType::Quat>>();

	// setValue WITH SYNERGIES - should normalize regardless of synergies
	auto vec  = generateVec4();
	auto quat = glm::quat(vec.w, vec.x, vec.y, vec.z); // not normalized + synergies=> normalized

	setValue_expectOk(rot, quat);

	EXPECT_TRUE(Math::isNormalized(rot->getNormalized()));

	// setValue WITHOUT SYNERGIES - should NOT normalize, ne NOT valid
	rot->disableSynergies(); // not of unit length
	vec  = generateVec4();
	quat = glm::quat(vec.w, vec.x, vec.y, vec.z); // not normalized + synergies=> normalized
	setValue_expectOk(rot, quat);

	EXPECT_TRUE(Math::isNormalized(rot->getNormalized()));
}

TEST(QuatRotTest, HandlingSynergies_setValueQuat)
{
	auto rot = Builder::createTransform<ETransformType::Quat>()->as<TransformImpl<ETransformType::Quat>>();


	// setValue WITH SYNERGIES - should normalize, be valid
	auto vec  = generateVec4();
	auto quat = glm::quat(vec.w, vec.x, vec.y, vec.z); // not normalized + synergies=> normalized

	setValue_expectOk(rot, quat);                   // synergies=> normalized

	EXPECT_EQ(rot->isValid(), ETransformState::Valid); // of unit size
	EXPECT_EQ(rot->getQuat(), rot->getNormalized());   // default quat is normalized (default = m_normalized)

	EXPECT_EQ(rot->getData().getMat4(), glm::toMat4(glm::normalize(quat))); //same matrix as from normalized


	// setValue WITHOUT SYNERGIES - should NOT normalize, ne NOT valid
	rot->disableSynergies(); // not of unit length
	vec  = generateVec4();
	quat = glm::quat(vec.w, vec.x, vec.y, vec.z); // not normalized 

	setValue_expectOk(rot, quat);              // NO synergies=> left not normalized

	EXPECT_NE(rot->isValid(), ETransformState::Valid); // NOT of unit size
	EXPECT_NE(rot->getQuat(), rot->getNormalized());   // default quat is left non-normalized

	auto L = rot->getData().getMat4();
	auto R = glm::toMat4(glm::normalize(quat));
	EXPECT_EQ(L, R); //the matrix is set correctly based on normalized quaternion
}

TEST(QuatRotTest, HandlingSynergies_setDefaultValue)
{
	auto rot = Builder::createTransform<ETransformType::Quat>()->as<TransformImpl<ETransformType::Quat>>();


	// setValue WITH SYNERGIES - should normalize (change) also the default
	auto vec  = generateVec4();
	auto quat = glm::quat(vec.w, vec.x, vec.y, vec.z); // not normalized quat + synergies=> normalized default 

	rot->setDefaultValue("quat", quat);  // set NOT normalized - must be normalized

	EXPECT_TRUE(rot->hasSynergies());
	EXPECT_EQ(rot->isValid(), ETransformState::Valid); // of unit size
	EXPECT_EQ(rot->getQuat(), rot->getNormalized());   // normalized default = m_normalized

	//EXPECT_EQ(glm::length2(rot->getNormalized()), 1.0f);  //normalized default |q| == 1.0
	EXPECT_TRUE(Math::isNormalized(rot->getNormalized()));
	EXPECT_EQ(rot->getData().getMat4(), glm::toMat4(glm::normalize(quat))); //sam matrix

	
	// setValue WITHOUT SYNERGIES - should NOT normalize
	rot->disableSynergies(); // not of unit length
	vec  = generateVec4();
	quat = glm::quat(vec.w, vec.x, vec.y, vec.z); // not normalized + synergies=> normalized

	rot->setDefaultValue("quat", quat); // set NOT normalized - must be left not normalized
	
	EXPECT_FALSE(rot->hasSynergies());
	EXPECT_NE(rot->isValid(), ETransformState::Valid); // not of unit size
	EXPECT_NE(rot->getQuat(), rot->getNormalized());   // default is left non-normalized

	auto L = rot->getData().getMat4();
	auto R = glm::toMat4(glm::normalize(quat));
	EXPECT_EQ(L, R); //the matrix is set correctly based on normalized quaternion
}

TEST(QuatRotTest, setValueVec4versusQuat)
{
	//auto vec = generateVec4();
	auto vec            = glm::vec4(1, 2, 3, 4);
	glm::vec4 vecNormalized  = glm::normalize(vec);
	auto quat           = glm::quat(vec.w, vec.x, vec.y, vec.z); // not normalized
	auto quatNormalized = glm::normalize(quat);

	auto quatVec = glm::quat(vec); // uses vec4 as axis and vec3 angle
	//auto quatVecNormalized = glm::normalize(quatVec);
	//auto vecWXYZ               = glm::vec4(vec.w, vec.x, vec.y, vec.z);
	//auto vecWXYZNormalized     = glm::normalize(vecWXYZ);        
	//auto quatVecWXYZ           = glm::quat(vecWXYZ);
	//auto quatVecWXYZNormalized = glm::normalize(quatVecWXYZ);

	EXPECT_NE(quat, quatVec); // uses vec4 as axis and vec3 angle

	EXPECT_EQ(quat.x, vec.x);
	EXPECT_EQ(quat.y, vec.y);
	EXPECT_EQ(quat.z, vec.z);
	EXPECT_EQ(quat.w, vec.w);
	/*
	 * vec	{x=4.84617853 r=4.84617853 s=4.84617853 ...}	glm::vec<4,float,0>
	 * vecNormalized	{x=0.457824409 r=0.457824409 s=0.457824409 ...}	glm::vec<4,float,0>
	 * quat	{x=4.84617853 y=-8.96076679 z=-0.264633060 ...}	glm::qua<float,0>
	 * quatNormalized	{x=0.457824409 y=-0.846534610 z=-0.0250002090 ...}	glm::qua<float,0>
	 *
	 * quatVecWXYZ	{x=-0.260383546 y=0.704501808 z=-0.251635522 ...}	glm::qua<float,0>
	 * quatVecWXYZNormalized	{x=-0.260383546 y=0.704501808 z=-0.251635522 ...}	glm::qua<float,0>
	 * quatVec	{x=-0.246685773 y=-0.706250250 z=-0.657858253 ...}	glm::qua<float,0>
	 * quatVecNormalized	{x=-0.246685773 y=-0.706250250 z=-0.657858253 ...}	glm::qua<float,0>
	 */
}

//TEST(QuatRotTest, setValueVec4)
//{
//	auto rot = Builder::createTransform<ETransformType::Quat>()->as<TransformImpl<ETransformType::Quat>>();
//
//
//	// setValue WITH SYNERGIES - should normalize, be valid
//	auto vec = generateVec4();
//
//	setValue_expectOk(rot, vec);    // setting quat(vec4) NORMALIZES THE QUATERNION
//
//
//	auto quat = glm::quat(vec.w, vec.x, vec.y, vec.z); // not normalized
//	EXPECT_NE(rot->getQuat(), quat);
//	auto A = rot->getQuat();
//	auto B = glm::normalize(quat);
//	EXPECT_EQ(rot->getQuat(), glm::normalize(quat));
//
//	// setValue WITHOUT SYNERGIES - should NOT normalize, ne NOT valid
//	rot->disableSynergies(); // not of unit length
//
//	setValue_expectOk(rot, vec);
//
//	quat = glm::quat(vec.w, vec.x, vec.y, vec.z); // not normalized
//	EXPECT_EQ(rot->getQuat(), quat);
//
//
//}



