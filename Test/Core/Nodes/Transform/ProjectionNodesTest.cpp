/**
 * \file
 * \brief
 * \authors Martin Herich, Petr Felkel
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

#include "Generator.h"

using namespace Core;

//--- Ortho -------------------------------------------------------------

TEST(OrthoProjTest, ShouldContainBeOk)
{
	auto ortho = Builder::createTransform<ETransformType::Ortho>();
	ortho->setDefaultValue("left", -10.0f);
	ortho->setDefaultValue("right", 10.0f);
	ortho->setDefaultValue("bottom", -5.0f);
	ortho->setDefaultValue("top", 5.0f);
	ortho->setDefaultValue("near", 1.0f);
	ortho->setDefaultValue("far", 100.0f);

	auto expectedMat = glm::ortho(-10.0f, 10.0f, -5.0f, 5.0f, 1.0f, 100.0f);
	auto resultMat = ortho->getData().getMat4();

	EXPECT_EQ(expectedMat, resultMat);
}

TEST(OrthoProjTest, Problem_Set22_changes00)
{
	auto ortho = Builder::createTransform<ETransformType::Ortho>();
	auto mat = ortho->getData().getMat4();
	ortho->initDefaults();

	auto left = ortho->getDefaultValue("left").getFloat();
	auto right = ortho->getDefaultValue("right").getFloat();
	auto bottom = ortho->getDefaultValue("bottom").getFloat();
	auto top = ortho->getDefaultValue("top").getFloat();
	auto near = ortho->getDefaultValue("near").getFloat();
	auto far = ortho->getDefaultValue("far").getFloat();

	auto expectedMat = glm::ortho(left, right, bottom, top, near, far);
	auto resultMat = ortho->getData().getMat4();
	EXPECT_EQ(expectedMat, resultMat);

	auto mat00 = mat[0][0];
	auto mat22 = mat[2][2];
	ortho->setValue(mat22 + 0.1f, glm::ivec2(2, 2)); // changing zscale
	auto mat00_after = mat[0][0];

	EXPECT_EQ(mat00, mat00_after); // should not change xscale
}

//--- Perspective -------------------------------------------------------------

TEST(PerspectiveProjTest, ShouldBeOk)
{
	auto perspective = Builder::createTransform<ETransformType::Perspective>();
	perspective->setDefaultValue("fovy", glm::radians(150.0f));
	perspective->setDefaultValue("aspect", 1.5f);
	perspective->setDefaultValue("near", 0.01f);
	perspective->setDefaultValue("far", 100.0f);

	auto expectedMat = glm::perspective(glm::radians(150.0f), 1.5f, 0.01f, 100.0f);
	auto resultMat = perspective->getData().getMat4();

	EXPECT_EQ(expectedMat, resultMat);
}

TEST(PerspectiveProjTest, GettersAndSettersShouldBeOk)
{
	auto perspective =
	    Builder::createTransform<ETransformType::Perspective>()->as<TransformImpl<ETransformType::Perspective>>();

	float fovy = generateFloat();
	float aspect = generateFloat();
	float nearZ = generateFloat();
	float farZ = generateFloat();

	perspective->setDefaultValue("fovy", fovy);
	EXPECT_EQ(fovy, perspective->getDefaultValue("fovy").getFloat());

	perspective->setDefaultValue("aspect", aspect);
	EXPECT_EQ(aspect, perspective->getDefaultValue("aspect").getFloat());

	perspective->setDefaultValue("near", nearZ);
	EXPECT_EQ(nearZ, perspective->getDefaultValue("near").getFloat());

	perspective->setDefaultValue("far", farZ);
	EXPECT_EQ(farZ, perspective->getDefaultValue("far").getFloat());

	EXPECT_EQ(glm::perspective(fovy, aspect, nearZ, farZ), perspective->getData().getMat4());
}

//--- Frustum -----------------------------------------------------------------
TEST(FrustumTest, ShouldBeOk)
{
	auto frustum = Builder::createTransform<ETransformType::Frustum>();
	frustum->setDefaultValue("left", -15.0f);
	frustum->setDefaultValue("right", 15.0f);
	frustum->setDefaultValue("bottom", -10.0f);
	frustum->setDefaultValue("top", 10.0f);
	frustum->setDefaultValue("near", 0.01f);
	frustum->setDefaultValue("far", 100.0f);

	auto expectedMat = glm::frustum(-15.0f, 15.0f, -10.0f, 10.0f, 0.01f, 100.0f);
	auto resultMat = frustum->getData().getMat4();

	EXPECT_EQ(expectedMat, resultMat);
}

TEST(FrustumTest, DISABLED_GettersAndSettersShouldBeOk_NoSynergies)
{
	auto frustum = Builder::createTransform<ETransformType::Frustum>()->as<TransformImpl<ETransformType::Frustum>>();

	float left = generateFloat();
	float right = generateFloat();
	float bottom = generateFloat();
	float top = generateFloat();
	float near = generateFloat();
	float far = generateFloat();

	frustum->disableSynergies();

	frustum->setDefaultValue("left", left);
	EXPECT_EQ(left, frustum->getDefaultValue("left").getFloat());

	frustum->setDefaultValue("right", right);
	EXPECT_EQ(right, frustum->getDefaultValue("right").getFloat());

	frustum->setDefaultValue("bottom", bottom);
	EXPECT_EQ(bottom, frustum->getDefaultValue("bottom").getFloat());

	frustum->setDefaultValue("top", top);
	EXPECT_EQ(top, frustum->getDefaultValue("top").getFloat());

	frustum->setDefaultValue("near", near);
	EXPECT_EQ(near, frustum->getDefaultValue("near").getFloat());

	frustum->setDefaultValue("far", far);
	EXPECT_EQ(far, frustum->getDefaultValue("far").getFloat());

	EXPECT_EQ(glm::frustum(left, right, bottom, top, near, far), frustum->getData().getMat4());
}

TEST(FrustumTest, DISABLED_GettersAndSettersShouldBeOk_Synergies)
{
	auto frustum = Builder::createTransform<ETransformType::Frustum>()->as<TransformImpl<ETransformType::Frustum>>();

	float left = generateFloat();
	float right = generateFloat();
	float bottom = generateFloat();
	float top = generateFloat();
	float near = generateFloat();
	float far = generateFloat();

	frustum->enableSynergies();

	frustum->setDefaultValue("left", left);
	EXPECT_EQ(left, frustum->getDefaultValue("left").getFloat());
	EXPECT_EQ(-left, frustum->getDefaultValue("right").getFloat());

	// frustum->setDefaultValue("right", right);
	// EXPECT_EQ(right, frustum->getDefaultValue("right").getFloat());
	// EXPECT_EQ(-right, frustum->getDefaultValue("left").getFloat());
	// todo - resetMatrixFromDefaults should have a parameter to allow full
	// symmetry

	frustum->setDefaultValue("bottom", bottom);
	EXPECT_EQ(bottom, frustum->getDefaultValue("bottom").getFloat());
	EXPECT_EQ(-bottom, frustum->getDefaultValue("top").getFloat());

	// frustum->setDefaultValue("top", top);
	// EXPECT_EQ(top, frustum->getDefaultValue("top").getFloat());
	// EXPECT_EQ(-top, frustum->getDefaultValue("bottom").getFloat());
	// todo - resetMatrixFromDefaults should have a parameter to allow full
	// symmetry

	frustum->setDefaultValue("near", near);
	EXPECT_EQ(near, frustum->getDefaultValue("near").getFloat());

	frustum->setDefaultValue("far", far);
	EXPECT_EQ(far, frustum->getDefaultValue("far").getFloat());

	// mixture of default values
	// EXPECT_EQ(glm::frustum(left, right, bottom, top, near, far),
	// frustum->getData().getMat4());
}
