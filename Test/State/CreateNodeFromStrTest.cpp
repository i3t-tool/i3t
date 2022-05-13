#include "gtest/gtest.h"

// Project includes
#include "Core/Nodes/GraphManager.h"
#include "State/SerializationVisitor.h"

// Test includes
#include "Utils.h"


//TEST(CreateNodeTest, OperatorCanBeCreatedFromStringRepr)
//{
//	auto sceneRepr = createTestOperators();
//	auto sceneData = load(sceneRepr);
//
//	SerializationVisitor visitor;
//	auto expectedSceneRepr = visitor.dump({ sceneData.operators }); // \todo JH not work
//
//	EXPECT_EQ(expectedSceneRepr, sceneRepr);
//}
