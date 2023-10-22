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

TEST(TransformMaskTest, _)
{
	auto scale = Core::Builder::createTransform<Core::ETransformType::Scale>();

	scale->lock();
	scale->enableSynergies();
	EXPECT_EQ(Core::EValueState::EditableSyn, scale->getValueState({0, 0}));
	EXPECT_EQ(Core::EValueState::EditableSyn, scale->getValueState({1, 1}));
	EXPECT_EQ(Core::EValueState::EditableSyn, scale->getValueState({2, 2}));
	EXPECT_EQ(Core::EValueState::Locked, scale->getValueState({3, 3}));

	scale->disableSynergies();
	EXPECT_EQ(Core::EValueState::Editable, scale->getValueState({0, 0}));
	EXPECT_EQ(Core::EValueState::Editable, scale->getValueState({1, 1}));
	EXPECT_EQ(Core::EValueState::Editable, scale->getValueState({2, 2}));
	EXPECT_EQ(Core::EValueState::Locked, scale->getValueState({3, 3}));

	scale->unlock();
	EXPECT_EQ(Core::EValueState::Editable, scale->getValueState({3, 3}));
}
