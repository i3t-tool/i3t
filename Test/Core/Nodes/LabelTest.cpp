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
