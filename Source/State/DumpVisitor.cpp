#include "DumpVisitor.h"

#include <numeric>
#include <unordered_map>

#include "Core/Nodes/Cycle.h"
#include "Core/Nodes/GraphManager.h"
#include "Logger/Logger.h"
#include "Utils/Format.h"

constexpr const char* g_baseFormatString = "  - id: {}\n"
																					 "    type: {}\n"
																					 "    position: [0.0, 0.0]\n";

std::string dumpValue(EValueType value, const Core::NodePtr& node)
{
	std::string str;

	switch (value)
	{
	case EValueType::Pulse:
		break;
	case EValueType::Float:
		str = fmt::format("{}", node->getData().getFloat());
		break;
	case EValueType::Vec3:
		str = Utils::toString(node->getData().getVec3());
		break;
	case EValueType::Vec4:
		str = Utils::toString(node->getData().getVec4());
		break;
	case EValueType::Matrix:
		str = Utils::toString(node->getData().getMat4());
		break;
	case EValueType::Quat:
		break;
	case EValueType::MatrixMul:
		break;
	case EValueType::Screen:
		break;
	case EValueType::Ptr:
		break;
	}

	return str;
}

/**
 * \param pin Right pin – must be plugged!
 * \return Value in format: - [leftNode, leftNodePin, rightNode, rightNodePin]
 */
std::string dumpEdge(const Core::Pin& pin)
{
	auto* leftPin = pin.getParentPin();
	I3T_ASSERT(leftPin != nullptr && "Pin is not plugged, cannot create edge entry.");

	return fmt::format("  - [{}, {}, {}, {}]",
										 leftPin->getOwner()->getId(), leftPin->getIndex(),
										 pin.getOwner()->getId(), pin.getIndex());
}

/**
 * @param node
 * @return Nested block in YAML fomat.
 */
NodeData dumpOperator(const Core::NodePtr& node)
{
#ifdef I3T_DEBUG
	auto operatorType = magic_enum::enum_cast<ENodeType>(node->getOperation()->keyWord);
	I3T_ASSERT(operatorType.has_value());
#endif

	/// \todo MH consider multiple editable inputs for operator.
	static std::string formatString = std::string(g_baseFormatString) +
																		"    value: {}\n";
	NodeData data;
	data.node = fmt::format(
			formatString,
			node->getId(),
			node->getOperation()->keyWord,
			dumpValue(node->getOutPin(0).getType(), node)
	);

	for (const auto& in : node->getInputPins())
	{
		if (in.isPluggedIn()) data.edges.push_back(dumpEdge(in));
	}

	return data;
}

std::string dumpCycle(const Core::Cycle& cycle)
{
	return fmt::format("");
}

std::string dumpTransform(const Core::TransformPtr& transform)
{
	return fmt::format(
			"- {}:"
			"    synergies: {}"
			"    locked: {}"
	);
}

std::string SceneData::toString() const
{
	std::string result;

	result += "operators:\n";
	for (const auto& op : operators)
	{
		result += op;
	}

	result += "edges:\n";
	result += Utils::concat(edges, "\n");

	return result;
}

std::string DumpVisitor::dump(const std::vector<Core::NodePtr>& nodes)
{
	m_sceneData.clear();

	for (const auto& node : nodes)
	{
		node->accept(*this);  // calls visit(node) and populates m_result variable.
	}

	return m_sceneData.toString();
}

void DumpVisitor::visit(const Core::NodePtr& node)
{
	if (Core::isSequence(node))
	{
		// Dump sequence and its transforms.
	}
	else if (node->getOperation() == &Core::g_CycleProperties)
	{

	}
	else if (node->getOperation() == &Core::g_cameraProperties)
	{
		// Dump camera sequences.
	}
	else if (Core::isTransform(node))
	{
		// Dump orphaned transform.
	}
	else if (Core::isOperator(node))
	{
		m_sceneData.addOperator(dumpOperator(node));
	}
}
