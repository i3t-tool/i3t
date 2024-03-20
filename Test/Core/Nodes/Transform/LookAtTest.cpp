/**
 * \file
 * \brief
 * \author Petr Felkel
 * \copyright Copyright (C) 2016-2023 I3T team, Department of Computer Graphics
 * and Interaction, FEE, Czech Technical University in Prague, Czech Republic
 *
 * This file is part of I3T - An Interactive Tool for Teaching Transformations
 * http://www.i3t-tool.org
 *
 * GNU General Public License v3.0 (see LICENSE.txt or https://www.gnu.org/licenses/gpl-3.0.txt)
 */
#include "gtest/gtest.h"

#include "Core/Nodes/GraphManager.h"

#include "../Utils.h" // setValueExpectXxx()
#include "Generator.h"

using namespace Core;

//--- Look At -----------------------------------------------------------------
TEST(LookAtTest, ShouldBeOk)
{
	auto lookAt = Builder::createTransform<ETransformType::LookAt>();
	lookAt->setDefaultValue("eye", glm::vec3{-10.0f, 5.0f, 1.0f});
	lookAt->setDefaultValue("center", glm::vec3{10.0f, 8.0f, -4.0f});
	lookAt->setDefaultValue("up", glm::vec3{0.0f, 1.0f, 0.0f});

	auto expectedMat =
	    glm::lookAt(glm::vec3{-10.0f, 5.0f, 1.0f}, glm::vec3{10.0f, 8.0f, -4.0f}, glm::vec3{0.0f, 1.0f, 0.0f});
	auto resultMat = lookAt->data().getMat4();

	EXPECT_EQ(expectedMat, resultMat);
}

TEST(LookAtTest, GettersAndSettersShouldBeOk)
{
	auto lookAt = Builder::createTransform<ETransformType::LookAt>()->as<TransformImpl<ETransformType::LookAt>>();

	auto eye = generateVec3();
	lookAt->setDefaultValue("eye", eye);
	EXPECT_EQ(eye, lookAt->getDefaultValue("eye").getVec3());

	auto center = generateVec3();
	lookAt->setDefaultValue("center", center);
	EXPECT_EQ(center, lookAt->getDefaultValue("center").getVec3());

	auto up = generateVec3();
	lookAt->setDefaultValue("up", up);
	EXPECT_EQ(up, lookAt->getDefaultValue("up").getVec3());

	EXPECT_EQ(glm::lookAt(eye, center, up), lookAt->data().getMat4());
}


TEST(LookAtTest, SetValue_and_Lock_Unlock)
{
	// Create an identity matrix
	auto lookAt = Builder::createTransform<ETransformType::LookAt>();
	EXPECT_TRUE(lookAt->isValid());

	auto eye = lookAt->getDefaultValue("eye").getVec3();
	auto center = lookAt->getDefaultValue("center").getVec3();
	auto up = lookAt->getDefaultValue("up").getVec3();

	auto mat = lookAt->getInternalData().getMat4();
	auto mat2 = lookAt->data().getMat4();

	EXPECT_EQ(mat, mat2);

	// created LookAt matrix should be equal to LookAt matrix created from its eye, center, and up vectors
	auto expectedMat = glm::lookAt(eye, center, up);
	EXPECT_TRUE(Math::eq(expectedMat, mat));
	EXPECT_EQ(expectedMat, mat);

	// nothing can be set
	lookAt->lock(); // isLocked==true is a default value
	auto val = generateFloat();
	setValue_expectWrong(lookAt, val, {1, 1});
	EXPECT_TRUE(lookAt->isValid());

	// corrupt the matrix
	// PF - todo - dava na gitlabu true, na PC dava false
	lookAt->unlock();
	setValue_expectOk(lookAt, val, {1, 1});

	auto wrongValue = lookAt->data().getMat4()[1][1];
	EXPECT_NE(wrongValue, 1.0f);
	EXPECT_EQ(wrongValue, val);

	EXPECT_FALSE(lookAt->isValid());
}
