#include "gtest/gtest.h"

#include "Core/Nodes/GraphManager.h"

using namespace Core;

TEST(LabelTest, GetLabelReturnNamedPinLabelOnNamedPin)
{
	auto node = Builder::createOperator<EOperatorType::MatrixMulMatrix>();

	const char* label = node->getInputPins()[0].getLabel();
	const char* expected = defaultIoNames[static_cast<size_t>(EValueType::Matrix)];

	EXPECT_TRUE(strcmp(label, expected) == 0);
}

TEST(LabelTest, GetLabelReturnDefaultPinLabelOnUnnamedPin)
{
	auto node = Builder::createOperator<EOperatorType::FloatsToVector3>();

	const char* label = node->getInputPins()[0].getLabel();
	const char* expected = "x";

	EXPECT_TRUE(strcmp(label, expected) == 0);
}
