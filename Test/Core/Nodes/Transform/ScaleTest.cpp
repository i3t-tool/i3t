#include "gtest/gtest.h"

#include "Core/Nodes/GraphManager.h"

#include "../Utils.h"
#include "Generator.h"

/* Test setValue
 *    - float  - setDefault + update matrix - respect synergies
 *		- vec3   - setDefault + update matrix - respect synergies
 *		- vec4   - calls vec3 - OK
 *		- mat    - selective copy of values respecting TransformMask
 *		         - not clear, if it clears the rest and respects synergies
 *		- (float, coord) - set single unlocked matrix element
 *		                 - when synergies - may update other synergistic elements
 *		                 .................- must update defaults
 *		                 - must not change defaults and the matrix when changing
 *non-synergistic element Test initDefaults Test setDefaultValue
 */

using namespace Core;

glm::mat4 createFreeTransform()
{
	// Create free transform.
	glm::mat4 mat(1.0f);
	mat[1][3] = generateFloat();
	mat[2][2] = generateFloat();
	mat[3][1] = generateFloat();

	return mat;
}
// ----------------------------------------------------------------------
// ------------- state variables ----------------------------------------
// ----------------------------------------------------------------------
TEST(ScaleTest, NewNodePredicates)
{
	const auto scale = Builder::createTransform<ETransformType::Scale>();

	EXPECT_TRUE(scale->hasMenuSynergies());
	EXPECT_TRUE(scale->hasSynergies());
	EXPECT_TRUE(scale->isLocked());
	EXPECT_TRUE(scale->isValid());
	EXPECT_FALSE(scale->isInSequence());
}
TEST(ScaleTest, ResetToInitialValues)
{
	const auto scale = generateVec3();

	// Create non-uniform scale with initial value.
	auto scaleNode =
	    Core::Builder::createTransform<ETransformType::Scale>()->as<TransformImpl<ETransformType::Scale>>();

	scaleNode->disableSynergies();              // to avoid uniform scale
	scaleNode->setDefaultValue("scale", scale); // default and matrix
	EXPECT_TRUE(scaleNode->isValid());

	// Set free transformation node.
	scaleNode->free(); // unlock, disable synergies

	glm::mat4 mat(1.0f);
	mat[1][3] = 165.0f;

	setValue_expectOk(scaleNode->as<Transform>(), mat);
	{
		const glm::mat4 data = scaleNode->getData().getMat4();

		EXPECT_TRUE(Math::eq(data, mat));
		EXPECT_FALSE(scaleNode->isValid());
	}
	{
		// --------Reset to initial values (identity) and state.
		// -----------------------
		scaleNode->enableSynergies();
		scaleNode->initDefaults();            // S(1,1,1)
		scaleNode->resetMatrixFromDefaults(); // locks

		/// \todo Should resetMatrixFromDefaults on scale node switch its synergies
		/// on?
		// EXPECT_TRUE(scaleNode->hasSynergies());

		const auto expected = glm::scale(glm::vec3(1.0f)); // identity
		const auto actual = scaleNode->getData().getMat4();

		EXPECT_TRUE(Math::eq(expected, actual));
		EXPECT_TRUE(scaleNode->isLocked());
	}
}
TEST(ScaleTest, Unlocked_SetFreeTransform_InvalidState)
{
	// Create non-uniform scale and unlock it.
	auto scale = Builder::createTransform<ETransformType::Scale>();
	scale->disableSynergies();
	scale->unlock();

	auto mat = createFreeTransform();
	setValue_expectOk(scale, mat);

	EXPECT_FALSE(scale->isValid());
}

// ----------------------------------------------------------------------
// ------------- setValue and synergies ---------------------------------
// ----------------------------------------------------------------------

// ---- mat4 ---- is a Transform method
// \todo  sets up canEdit values only, the rest is unchanged
TEST(ScaleTest, UniformScale_SetValidValue_Mat4_AsTransformation_Ok)
{
	// Create a uniform scale.
	auto scale = Builder::createTransform<ETransformType::Scale>();

	scale->enableSynergies();

	// Set new uniform scale as vec3
	auto val = generateFloat();
	const auto scaleValue = glm::vec3(val, val, val); // uniform scale

	setValue_expectOk(scale, glm::scale(scaleValue));

	EXPECT_TRUE(Math::eq(glm::scale(scaleValue), scale->getData().getMat4()));
	EXPECT_TRUE(scale->isValid());
}

TEST(ScaleTest, UniformScale_SetInvalidValue_Mat4_AsTransformation_ShouldNotBePermited_butWorks)
{
	// Create uniform scale.
	auto scale = Builder::createTransform<ETransformType::Scale>();

	scale->enableSynergies();

	// Set new non-uniform scale, action is possible but the result can be
	// undefined. PF: In the case of uniform scaling value on main diagonal will
	// be the last value set, i.e. 3. \todo - wouldn' it be better if it returns
	// an error?

	//------------------------------------------
	const auto scaleValue = generateVec3(); // non-uniform scale
	//------------------------------------------
	auto result = scale->setValue(glm::scale(scaleValue)); // init by a non-uniform matrix -> uniform

	EXPECT_EQ(ValueSetResult::Status::Ok, result.status);
	EXPECT_TRUE(scale->isValid()); // is uniform

	EXPECT_TRUE(Math::eq(glm::scale(glm::vec3(scaleValue.z)),
	                     scale->getData().getMat4())); // last of non-uniform vec3
}

// ----	float ----
TEST(ScaleTest, UniformScale_SetValidValue_Float_asScale_Ok)
{
	// Create a uniform scale.
	auto scale = Builder::createTransform<ETransformType::Scale>()->as<TransformImpl<ETransformType::Scale>>();

	scale->enableSynergies();

	// Set new uniform scale as float
	auto val = generateFloat();
	// setValue_expectOk(scale, val);
	auto result = scale->setValue(val);
	EXPECT_EQ(ValueSetResult::Status::Ok, result.status);
	EXPECT_TRUE(Math::eq(glm::scale(glm::vec3(val, val, val)), scale->getData().getMat4()));
	EXPECT_TRUE(scale->isValid());
}

// ----	vec3 ----
TEST(ScaleTest, UniformScale_SetValidValue__Vec3_Uniform_Ok)
{
	// Create a uniform scale.
	auto scale = Builder::createTransform<ETransformType::Scale>()->as<TransformImpl<ETransformType::Scale>>();

	scale->enableSynergies();

	// Set new uniform scale as vec3
	auto val = generateFloat();
	const auto scaleVec3 = glm::vec3(val, val, val);

	////setValue_expectOk(scale, glm::scale(scaleVec3));   // works for
	/// Transformation, not for TransformImpl
	auto result = scale->setValue(scaleVec3);

	EXPECT_TRUE(Math::eq(glm::scale(scaleVec3), scale->getData().getMat4()));
	EXPECT_TRUE(scale->isValid());
}

// uniform scale will not accept setting via non-uniform vector
TEST(ScaleTest, UniformScale_SetInvalidValue__Vec3_Non_uniform_fail)
// TEST(ScaleTest, DISABLED_UniformScale_SetValidValue_Vec3_asScale_Wrong)
{
	// Create a uniform scale.
	auto scale = Builder::createTransform<ETransformType::Scale>()->as<TransformImpl<ETransformType::Scale>>();

	scale->enableSynergies(); // should be implicit
	const auto before = scale->getData().getMat4();

	// Set new uniform scale as vec3
	// TEST: setting a non-uniform vec3 scale to uniform-scale matrix must fail
	const auto scaleVec3 = generateVec3();

	// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
	const auto result = scale->setValue(scaleVec3); // the test
	// ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
	EXPECT_EQ(ValueSetResult::Status::Err_ConstraintViolation, result.status);

	const auto after = scale->getData().getMat4();

	// And the matrix must remain unchanged (identity matrix remains)
	EXPECT_TRUE(scale->isValid());                        // matrix not changed - validity must remain true
	EXPECT_TRUE(Math::eq(before, after));                 // the matrix did not change
	EXPECT_FALSE(Math::eq(glm::scale(scaleVec3), after)); // non-uniform scale matrix was NOT set
	EXPECT_FALSE(                                         // we do not use just scale.x as the uniform scale value
	    Math::eq(glm::scale(glm::vec3(scaleVec3.x)), scale->getData().getMat4()));
}

// non-uniform scale switched to uniform scale (via enable synergies) invalidates the scale box
TEST(ScaleTest, Uniform_WithNonUniformValues_IsInvalid)
{
	// Create non-uniform scale.
	auto scale = Builder::createTransform<ETransformType::Scale>()->as<TransformImpl<ETransformType::Scale>>();
	EXPECT_TRUE(scale->isValid()); // it IS uniform
	scale->disableSynergies();     // -> non-uniform scale

	setValue_expectOk(scale, generateVec3()); // should set a non-uniform scale
	EXPECT_TRUE(scale->isValid());            // with disabled synergies accepts non-uniform vec3 values

	scale->enableSynergies();
	EXPECT_FALSE(scale->isValid()); // should FAIL, as it is not uniform anymore
}

// ----	float on coords ----
TEST(ScaleTest, UniformScaleMatrixValueAndSynergies)
{
	auto scaleValue = generateFloat();
	const auto scale = glm::vec3(scaleValue);
	const auto scaleMat = glm::scale(scale);

	auto scaleNode = Builder::createTransform<ETransformType::Scale>()->as<TransformImpl<ETransformType::Scale>>();

	{
		// Invalid coordinates.
		const auto result = scaleNode->setValue(scaleValue, {3, 1});
		EXPECT_EQ(ValueSetResult::Status::Err_ConstraintViolation, result.status);
	}
	{
		// Valid coordinates.
		setValue_expectOk(scaleNode, scaleValue, {1, 1}); // write the whole diagonal
		const auto data = scaleNode->getData().getMat4();

		EXPECT_EQ(data, scaleMat);
		EXPECT_TRUE(scaleNode->isValid());
	}
}
TEST(ScaleTest, ScaleMatrixSetsDefaults_diagonal)
{
	auto scaleValue = generateFloat();
	auto scaleNode = Builder::createTransform<ETransformType::Scale>()->as<TransformImpl<ETransformType::Scale>>();

	scaleNode->lock(); // should be
	int coord = generateInt(0, 2);

	// synergies set all three scales
	{
		scaleNode->enableSynergies(); // should be

		auto result = scaleNode->setValue(scaleValue, {coord, coord});
		EXPECT_EQ(ValueSetResult::Status::Ok, result.status);
		auto v = scaleNode->getDefaultValue("scale").getVec3();
		EXPECT_EQ(v.x, scaleValue);
		EXPECT_EQ(v.y, scaleValue);
		EXPECT_EQ(v.z, scaleValue);
	}

	// no-synergies set just one value - on coord
	{
		scaleNode->disableSynergies();
		scaleValue = generateFloat();

		auto result = scaleNode->setValue(scaleValue, {coord, coord});
		EXPECT_EQ(ValueSetResult::Status::Ok, result.status);
		auto v = scaleNode->getDefaultValue("scale").getVec3();
		for (int i = 0; i < 3; i++)
		{
			if (i == coord)
				EXPECT_EQ(v[i], scaleValue);
			else
			{
				EXPECT_NE(v[i], scaleValue);
			}
		}
	}
}

// ----------------------------------------------------------------------
// ------------- setDefaults --------------------------------------------
// ----------------------------------------------------------------------

// ---- scale ----
TEST(ScaleTest, GettersAndSetterShouldBeOk)
{
	auto scale = Core::Builder::createTransform<ETransformType::Scale>()->as<TransformImpl<ETransformType::Scale>>();

	auto vec = generateVec3();

	// Uniform scale.
	{
		scale->enableSynergies();

		scale->setDefaultValue("scale", vec); // default and the matrix

		EXPECT_EQ(glm::scale(glm::vec3{vec.x, vec.x, vec.x}), scale->getData().getMat4());
	}
	// non-uniform scale.
	{
		scale->disableSynergies();

		scale->setDefaultValue("scale", vec);

		EXPECT_EQ(glm::scale(vec), scale->getData().getMat4());
	}
}

TEST(ScaleTest, ScaleDefaultsSetMatrix)
{
	auto scaleValue = generateFloat();
	auto scaleVector = generateVec3();

	///	auto scaleMat   = glm::scale(scale);

	auto scaleNode = Builder::createTransform<ETransformType::Scale>()->as<TransformImpl<ETransformType::Scale>>();

	// Uniform scale.
	// scalar sets matrix
	{
		scaleNode->enableSynergies();
		scaleNode->setDefaultUniformScale(scaleValue); // scalar
		EXPECT_NE(scaleNode->getDefaultValue("scale").getVec3(),
		          scaleVector); // different vector is different
		EXPECT_EQ(scaleNode->getDefaultValue("scale").getVec3(),
		          glm::vec3(scaleValue)); // uniform
		EXPECT_EQ(glm::scale(glm::vec3(scaleValue)), scaleNode->getData().getMat4());
	}
	// vector sets matrix based on .x only
	{
		scaleNode->enableSynergies();
		scaleNode->setDefaultValue("scale", scaleVector); // vector

		auto v = scaleNode->getDefaultValue("scale").getVec3();
		EXPECT_NE(v, scaleVector); // v should be a uniform scale

		EXPECT_EQ(v.x, scaleVector.x);          // uniform scale based on .x
		EXPECT_EQ(v, glm::vec3(scaleVector.x)); // uniform scale based on .x
		EXPECT_TRUE(Math::eq(glm::scale(glm::vec3(scaleVector.x)), scaleNode->getData().getMat4()));
	}
	// vector sets matrix - correct
	{
		scaleNode->disableSynergies();
		scaleNode->setDefaultValue("scale", scaleVector); // vector
		// EXPECT_EQ(scaleNode->getDefaultValue("scale"), scaleVector);
		EXPECT_EQ(glm::scale(scaleVector), scaleNode->getData().getMat4());
	}
}
