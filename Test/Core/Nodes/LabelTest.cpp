/**
 * \file
 * \brief
 * \author Martin Herich
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

using namespace Core;

TEST(LabelTest, GetLabelReturnNamedPinLabelOnNamedPin)
{
	auto node = Builder::createOperator<EOperatorType::MatrixMulMatrix>();

	const auto& label = node->getInputPins()[0].getLabel();
	// Now it returns empty string for non-custom pin names.
	// const auto& expected = defaultIoNames[static_cast<size_t>(EValueType::Matrix)];
	const auto expected = "";

	EXPECT_EQ(label, expected);
}

TEST(LabelTest, GetLabelReturnDefaultPinLabelOnUnnamedPin)
{
	auto node = Builder::createOperator<EOperatorType::FloatsToVector3>();

	const auto& label = node->getInputPins()[0].getLabel();
	const auto& expected = "x";

	EXPECT_EQ(label, expected);
}
