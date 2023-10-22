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

#include "../Utils.h"
#include "Generator.h"

using namespace Core;

TEST(TranslationTest, InvalidValues_ShouldNotBePermitted)
{
	auto translationNode =
	    Builder::createTransform<ETransformType::Translation>()->as<TransformImpl<ETransformType::Translation>>();

	// Invalid coordinates.
	auto result = translationNode->setValue(generateFloat(), {0, 3});

	EXPECT_EQ(ValueSetResult::Status::Err_ConstraintViolation, result.status);
	EXPECT_TRUE(translationNode->isValid());
}

TEST(TranslationTest, ValidValues_Ok)
{
	auto translationNode = Builder::createTransform<ETransformType::Translation>();

	// Valid coordinates.
	setValue_expectOk(translationNode, -2.0f, {3, 0});
	setValue_expectOk(translationNode, -3.0f, {3, 1});
	setValue_expectOk(translationNode, -4.0f, {3, 2});

	auto translMat = glm::translate(glm::vec3(-2.0f, -3.0f, -4.0f));
	auto data = translationNode->getData().getMat4();

	EXPECT_EQ(translMat, data);
	EXPECT_TRUE(translationNode->isValid());
}

TEST(TranslationTest, Unlocked_InvalidValues_InvalidState)
{
	auto translationNode = Builder::createTransform<ETransformType::Translation>();
	translationNode->unlock();

	// Invalid coordinates.
	setValue_expectOk(translationNode, generateFloat(10.0f, 20.0f), {1, 1});

	EXPECT_FALSE(translationNode->isValid());
}

TEST(TranslationTest, GettersAndSetterShouldBeOk)
{
	auto translation =
	    Builder::createTransform<ETransformType::Translation>()->as<TransformImpl<ETransformType::Translation>>();

	auto vec = generateVec3();

	translation->setDefaultValue("translation", vec);

	EXPECT_EQ(glm::translate(vec), translation->getData().getMat4());
}

TEST(TranslationTest, SetValueInMatrixUpdatesDefaultValue)
{
	auto translation =
	    Builder::createTransform<ETransformType::Translation>()->as<TransformImpl<ETransformType::Translation>>();

	auto vec = generateVec3();
	auto val = generateFloat();
	auto val2 = 2.0f * val;
	auto val3 = 3.0f * val;

	// Valid coordinates.
	translation->setValue(val); // force synergies - NO - translation has no synergies
	EXPECT_EQ(translation->getDefaultValue("translation").getVec3(),
	          glm::vec3(val)); // val -> vec

	translation->setValue(vec);
	EXPECT_EQ(translation->getDefaultValue("translation").getVec3(),
	          vec); // vec -> vec

	translation->disableSynergies();
	setValue_expectWrong(translation, val, glm::ivec2(0, 0)); // wrong coord

	translation->setValue(val, glm::ivec2(3, 0)); // scale x alone
	EXPECT_EQ(translation->getDefaultValue("translation").getVec3().x,
	          val); // vec -> vec
}

TEST(TranslationTest, SetDefaultValueUpdatesMatrix)
{
	auto vec = generateVec3();
	auto val = generateFloat();

	auto translation =
	    Builder::createTransform<ETransformType::Translation>()->as<TransformImpl<ETransformType::Translation>>();

	translation->setDefaultValue("translation", vec);

	EXPECT_EQ(glm::translate(vec), translation->getData().getMat4());
}