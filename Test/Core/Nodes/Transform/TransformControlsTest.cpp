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

#include "Core/Nodes/Utils.h"
#include "Generator.h"

using namespace Core;

TEST(TransformControlsTest, _)
{
	auto translation = GraphManager::createTransform<ETransformType::Translation>();

	translation->setDefaultValue("translation", generateVec3());
	auto expectedValues = translation->data().getMat4();

	translation->saveValue();

	translation->unlock();
	setValue_expectOk(translation, 5.0f, {1, 1});

	translation->reloadValue();

	EXPECT_TRUE(Math::eq(expectedValues, translation->data().getMat4()));
}
