#include "gtest/gtest.h"

// Project includes
#include "Core/Nodes/GraphManager.h"
#include "State/DumpVisitor.h"

// Test includes
#include "Utils.h"

TEST(CreateNodeTest, OperatorCanBeCreatedFromStringRepr)
{
	auto sceneRepr = createTestOperators();
	auto sceneData = load(sceneRepr);

	DumpVisitor visitor;
	auto expectedSceneRepr = visitor.dump({ sceneData.operators });

	EXPECT_EQ(expectedSceneRepr, sceneRepr);
}
