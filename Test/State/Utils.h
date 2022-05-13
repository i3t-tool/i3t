#pragma once

#include <string>

#include "Core/Nodes/Utils.h"

inline std::string createTestOperators()
{
	auto float1 = Core::GraphManager::createNode<ENodeType::FloatToFloat>();
	auto float2 = Core::GraphManager::createNode<ENodeType::FloatToFloat>();

	plug_expectOk(float1, float2, 0, 0);

	return "operators:\n"
				 "  - id: " + std::to_string(float1->getId()) + "\n"
				 "    type: FloatToFloat\n"
				 "    position: [0.0, 0.0]\n"
				 "    value: 0.0\n"
				 "  - id: " + std::to_string(float2->getId()) + "\n"
				 "    type: FloatToFloat\n"
				 "    position: [0.0, 0.0]\n"
				 "    value: 0.0\n"
				 "edges:\n"
				 "  - [" + std::to_string(float1->getId()) + ", 0, " + std::to_string(float2->getId()) + ", 0]";
}
