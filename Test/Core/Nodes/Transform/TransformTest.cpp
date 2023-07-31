#include "gtest/gtest.h"

#include "Core/Nodes/GraphManager.h"

#include "../Utils.h"
#include "Generator.h"

using namespace Core;

TEST(TransformTest, LookAt_SetValue_Ok)
{
	// Create uniform scale.
	auto lookAt = Builder::createTransform<ETransformType::LookAt>();
	EXPECT_TRUE(lookAt->isValid());

	auto eye = lookAt->getDefaultValue("eye").getVec3();
	auto center = lookAt->getDefaultValue("center").getVec3();
	auto up = lookAt->getDefaultValue("up").getVec3();

	auto mat = lookAt->getInternalData().getMat4();
	auto mat2 = lookAt->getData().getMat4();

	EXPECT_EQ(mat, mat2);

	// nothing can be set
	lookAt->lock();
	auto val = generateFloat();
	setValue_expectWrong(lookAt, val, {1, 1});
	EXPECT_TRUE(lookAt->isValid());

	// corrupt the matrix
	// PF - todo - dává na gitlabu true, na PC dává false
	// lookAt->unlock();
	// setValue_expectOk(lookAt, val, {1, 1});
	// EXPECT_FALSE(lookAt->isValid());

	// expected matrix
	auto expectedMat = glm::lookAt(eye, center, up);
	EXPECT_TRUE(Math::eq(expectedMat, mat));
	EXPECT_EQ(expectedMat, mat);
}
