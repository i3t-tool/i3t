#include "gtest/gtest.h"

#include "Core/Nodes/GraphManager.h"

#include "Generator.h"
#include "../Utils.h"

using namespace Core;

TEST(TransformTest, LookAt_SetValue_Ok)
{
	// Create uniform scale.
	auto lookAt = Builder::createTransform<ETransformType::LookAt>();
	EXPECT_EQ(ETransformState::Valid, lookAt->isValid());

	auto eye    = lookAt->getDefaultValue("eye").getVec3();
	auto center = lookAt->getDefaultValue("center").getVec3();
	auto up     = lookAt->getDefaultValue("up").getVec3();

	auto mat    = lookAt->getInternalData().getMat4();
	auto mat2   = lookAt->getData().getMat4();
		
	EXPECT_EQ(mat, mat2);

	// nothing can be set
	lookAt->lock();
	auto val = generateFloat();
	setValue_expectWrong(lookAt, val, {1, 1});  
	EXPECT_EQ(ETransformState::Valid, lookAt->isValid());

	// corrupt the matrix
	lookAt->unlock();
	setValue_expectOk(lookAt, val, {1, 1});  
	EXPECT_NE(ETransformState::Valid, lookAt->isValid());

	// expected matrix
	auto expectedMat = glm::lookAt(eye, center, up);
	EXPECT_TRUE( Math::eq(expectedMat, mat));
	EXPECT_EQ(expectedMat, mat);
}
