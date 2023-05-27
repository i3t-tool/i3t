#include "gtest/gtest.h"

#include "Core/Nodes/GraphManager.h"
#include "State/SerializationVisitor.h"

#include "Core/Nodes/Utils.h"

// TEST(SerializationVisitor, GivesExpectedOutputForOperator)
//{
//	SerializationVisitor visitor;
//
//	auto result = visitor.dump({
// Core::GraphManager::createOperator<EOperatorType::FloatToFloat>() }); // \todo JH
//\todo MH not work
//
//	std::string expected = fmt::format("- {}"
//																		 "    position: [0.0, 0.0]",
//																		 1);
//	std::string pica = "";
// }

// TEST(SerializationVisitor, SimpleSceneGetsSerializedCorrectly)
//{
//	auto float1 = Core::GraphManager::createOperator<EOperatorType::FloatToFloat>();
//	auto float2 = Core::GraphManager::createOperator<EOperatorType::FloatToFloat>();
//
//	plug_expectOk(float1, float2, 0, 0);
//
//	std::string expectedOutput = fmt::format(""
//	"operators:\n"
//	"  - id: {}\n"
//	"    type: FloatToFloat\n"
//	"    position: [0.0, 0.0]\n"
//	"    value: 0.0\n"
//	"  - id: {}\n"
//	"    type: FloatToFloat\n"
//	"    position: [0.0, 0.0]\n"
//	"    value: 0.0\n"
//	"edges:\n"
//	"  - [{}, {}, {}, {}]",
//	float1->getId(), float2->getId(),
//   float1->getId(), 0, float2->getId(), 0);
//
//	SerializationVisitor visitor;
//
//	auto result = visitor.dump({ float1, float2 }); /* \todo JH \todo MH not
// work */
//
//	EXPECT_EQ(result, expectedOutput);
// }
