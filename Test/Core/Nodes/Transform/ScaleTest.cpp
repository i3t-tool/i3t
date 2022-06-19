#include "gtest/gtest.h"

#include "Core/Nodes/GraphManager.h"

#include "Generator.h"
#include "../Utils.h"

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

TEST(ScaleTest, UniformScale_SetValidValue_Ok)
{
	// Create uniform scale.
	auto scale = Builder::createTransform<ETransformType::Scale>();
	scale->enableSynergies();

	// Set new uniform scale.
	const auto val = generateFloat();
	const auto scaleValue = glm::vec3(val, val, val);

	setValue_expectOk(scale, glm::scale(scaleValue));

	EXPECT_TRUE(Math::eq(glm::scale(scaleValue), scale->getData().getMat4()));
	EXPECT_EQ(ETransformState::Valid, scale->isValid());
}

TEST(ScaleTest, UniformScale_SetInvalidValue_ShouldNotBePermited_butWorks)
{
	constexpr auto scaleValue = glm::vec3(1.0f, 5.0f, 3.0f);

	// Create uniform scale.
	auto scale = Builder::createTransform<ETransformType::Scale>();

	scale->enableSynergies();

	// Set new non-uniform scale, action is possible but the result can be undefined.
	// PF: In the case of uniform scaling value on main diagonal will be the last value set, i.e. 3.
	// \todo - wouldn' it be better if it returns an error?

	auto result = scale->setValue(glm::scale(scaleValue));  // init by a non-uniform matrix -> uniform

	EXPECT_EQ(ValueSetResult::Status::Ok, result.status);  
	EXPECT_EQ(ETransformState::Valid, scale->isValid());  // is uniform

	EXPECT_TRUE(Math::eq(glm::scale(glm::vec3(scaleValue.z)), scale->getData().getMat4())); // last of non-uniform vec3
	EXPECT_FALSE(Math::eq(glm::scale(scaleValue), scale->getData().getMat4()));  // complete vec3
}

TEST(ScaleTest, Uniform_WithNonUniformValues_IsInvalid)
{
	// Create non-uniform scale.
	auto scale = Builder::createTransform<ETransformType::Scale>()
	    ->as<TransformImpl<ETransformType::Scale>>();
	scale->disableSynergies();

	setValue_expectOk(scale, generateVec3());

	scale->enableSynergies();
	EXPECT_EQ(ETransformState::Invalid, scale->isValid());
}

TEST(ScaleTest, Unlocked_SetFreeTransform_InvalidState)
{
	// Create non-uniform scale and unlock it.
	auto scale = Builder::createTransform<ETransformType::Scale>();
	scale->disableSynergies();
	scale->unlock();

	auto mat = createFreeTransform();
	setValue_expectOk(scale, mat);

	EXPECT_EQ(ETransformState::Invalid, scale->isValid());
}

TEST(ScaleTest, ResetToInitialValues)
{
	const auto scale = generateVec3();

	// Create non-uniform scale with initial value.
	auto scaleNode = Core::Builder::createTransform<ETransformType::Scale>()->as<TransformImpl<ETransformType::Scale>>();

	scaleNode->disableSynergies();   // to avoid uniform scale
	scaleNode->setDefaultValue("scale", scale);  // default and matrix
	EXPECT_EQ(ETransformState::Valid, scaleNode->isValid());  

	// Set free transformation node.
	scaleNode->free();  //unlock, disable synergies

	glm::mat4 mat(1.0f);
	mat[1][3] = 165.0f;

	setValue_expectOk(scaleNode->as<Transformation>(), mat);
	{
		const glm::mat4 data = scaleNode->getData().getMat4();

		EXPECT_TRUE(Math::eq(data, mat));
		EXPECT_NE(ETransformState::Valid, scaleNode->isValid());
	}
	{
		// --------Reset to initial values (identity) and state. -----------------------
		scaleNode->enableSynergies();
		scaleNode->initDefaults(); //S(1,1,1)
		scaleNode->resetMatrixFromDefaults(); // locks

		/// \todo Should resetMatrixFromDefaults on scale node switch its synergies on?
		// EXPECT_TRUE(scaleNode->hasSynergies());

		const auto expected = glm::scale(glm::vec3(1.0f)); // identity
		const auto actual   = scaleNode->getData().getMat4();

		EXPECT_TRUE(Math::eq(expected, actual));
		EXPECT_TRUE(scaleNode->isLocked());
	}
}	

TEST(ScaleTest, UniformScaleMatrixValueAndSynergies)
{
	auto       scaleValue = generateFloat();
	const auto scale      = glm::vec3(scaleValue);
	const auto scaleMat   = glm::scale(scale);

	auto scaleNode = Builder::createTransform<ETransformType::Scale>()
	    ->as<TransformImpl<ETransformType::Scale>>();

	{
		// Invalid coordinates.
		const auto result = scaleNode->setValue(scaleValue, {3, 1});
		EXPECT_EQ(ValueSetResult::Status::Err_ConstraintViolation, result.status);
	}
	{
		// Valid coordinates.
		setValue_expectOk(scaleNode, scaleValue, {1, 1});  // write the whole diagonal
		const auto data = scaleNode->getData().getMat4();

		EXPECT_EQ(data, scaleMat);
		EXPECT_EQ(ETransformState::Valid, scaleNode->isValid());
	}
}

TEST(ScaleTest, GettersAndSetterShouldBeOk)
{
  auto scale = Core::Builder::createTransform<ETransformType::Scale>()
      ->as<TransformImpl<ETransformType::Scale>>();

  auto vec = generateVec3();

	// Uniform scale.
  {
		scale->enableSynergies();

    scale->setDefaultValue("scale", vec);  // default and the matrix

    EXPECT_EQ(glm::scale(glm::vec3{vec.x, vec.x, vec.x}), scale->getData().getMat4());
	}
  {
		scale->disableSynergies();

		scale->setDefaultValue("scale", vec);

    EXPECT_EQ(glm::scale(vec), scale->getData().getMat4());
	}
}


//PF -----------------------------------------
TEST(ScaleTest, ScaleDefaultsSetMatrix)
{
	auto scaleValue = generateFloat();
	auto scaleVector = generateVec3();

///	auto scaleMat   = glm::scale(scale);

	auto scaleNode = Builder::createTransform<ETransformType::Scale>()
	     ->as<TransformImpl<ETransformType::Scale>>();

	// Uniform scale.
	// scalar sets matrix
	{
		scaleNode->enableSynergies();
		scaleNode->setDefaultUniformScale(scaleValue);  //scalar
		EXPECT_NE(scaleNode->getDefaultValue("scale").getVec3(), scaleVector);
		EXPECT_EQ(scaleNode->getDefaultValue("scale").getVec3(), glm::vec3(scaleValue));  //uniform
		EXPECT_EQ(glm::scale(glm::vec3(scaleValue)), scaleNode->getData().getMat4());
	}
	// vector sets matrix based on .x only
	{
		scaleNode->enableSynergies();
		scaleNode->setDefaultValue("scale", scaleVector); //vector

		auto v = scaleNode->getDefaultValue("scale").getVec3();
		//EXPECT_NE(v.x, scaleVector.x);
		EXPECT_EQ(glm::scale(glm::vec3(scaleVector.x)), scaleNode->getData().getMat4());
	}
	// vector sets matrix - correct
	{
		scaleNode->disableSynergies();
		scaleNode->setDefaultValue("scale", scaleVector); //vector
		//EXPECT_EQ(scaleNode->getDefaultValue("scale"), scaleVector);
		EXPECT_EQ(glm::scale(scaleVector), scaleNode->getData().getMat4());
	}

}
TEST(ScaleTest, ScaleMatrixSetsDefaults_diagonal)
{
	auto scaleValue  = generateFloat();
	//auto scaleVector = generateVec3();

	///	auto scaleMat   = glm::scale(scale);

	auto scaleNode = Builder::createTransform<ETransformType::Scale>()
	     ->as<TransformImpl<ETransformType::Scale>>();
	scaleNode->lock();   // should be
	int coord = generateInt(0,2);

	// synergies set all three scales
	{
		scaleNode->enableSynergies();

		auto result = scaleNode->setValue(scaleValue, {coord, coord});
		EXPECT_EQ(ValueSetResult::Status::Ok, result.status);
		auto v = scaleNode->getDefaultValue("scale").getVec3();
		EXPECT_EQ(v.x, scaleValue);
		EXPECT_EQ(v.y, scaleValue);
		EXPECT_EQ(v.z, scaleValue);
	}

	//no-synergies set just one value
	{
		scaleNode->disableSynergies();
		scaleValue = generateFloat();

		auto result = scaleNode->setValue(scaleValue, {coord, coord });
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
