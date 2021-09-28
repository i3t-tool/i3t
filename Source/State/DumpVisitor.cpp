#include "DumpVisitor.h"

#include <numeric>
#include <unordered_map>

#include "yaml-cpp/yaml.h"

#include "Core/Nodes/Cycle.h"
#include "Core/Nodes/GraphManager.h"
#include "Logger/Logger.h"
#include "Utils/Format.h"

/// Base format string for node.
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

	return fmt::format("  - [{}, {}, {}, {}]", leftPin->getOwner()->getId(), leftPin->getIndex(), pin.getOwner()->getId(),
										 pin.getIndex());
}

/**
 * \param node
 * \return Nested block in YAML format.
 */
NodeData dumpOperator(const Core::NodePtr& node)
{
#ifdef I3T_DEBUG
	auto operatorType = magic_enum::enum_cast<ENodeType>(node->getOperation()->keyWord);
	I3T_ASSERT(operatorType.has_value());
#endif

	/// \todo MH consider multiple editable inputs for operator.
	static std::string formatString = std::string(g_baseFormatString) + "    value: {}\n";
	NodeData					 data;
	data.node = fmt::format(formatString, node->getId(), node->getOperation()->keyWord,
													dumpValue(node->getOutPin(0).getType(), node));

	for (const auto& in : node->getInputPins())
	{
		if (in.isPluggedIn()) data.edges.push_back(dumpEdge(in));
	}

	return data;
}

std::string dumpCycle(const Core::Cycle& cycle) { return fmt::format(""); }

std::string dumpTransform(const Core::TransformPtr& transform)
{
	return fmt::format("- {}:"
										 "    synergies: {}"
										 "    locked: {}");
}

std::string SceneRawData::toString() const
{
	std::string result;

	result += "operators:\n";
	for (const auto& op : operators) { result += op; }

	result += "edges:\n";
	result += Utils::concat(edges, "\n");

	return result;
}

std::map<std::string_view, std::function<Core::NodePtr()>> createFns;

template <int N, int Max>
void do_for()
{
	Core::GraphManager::createNode<static_cast<ENodeType>(N)>();

	if constexpr (N < Max)
	{
		do_for<N + 1, Max>();
	}
}

bool DumpVisitor::m_isInitialized = false;

void initCreateFns()
{
	constexpr std::size_t count = magic_enum::enum_count<ENodeType>() - 1;

	do_for<0, count>();
}

DumpVisitor::DumpVisitor()
{
	if (!m_isInitialized)
	{
		initCreateFns();
		m_isInitialized = true;
	}
}

std::string DumpVisitor::dump(const std::vector<Core::NodePtr>& nodes)
{
	m_sceneData.clear();

	for (const auto& node : nodes)
	{
		node->accept(*this); // calls visit(node) and populates m_result variable.
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
	{}
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

bool isParsedSceneValid(YAML::Node& parsedScene) { return parsedScene["operators"] && parsedScene["edges"]; }

/**
 * \return nullptr if node can't be created.
 */
Core::NodePtr createOperator(YAML::Node& node)
{
	if (node["type"])
	{
		auto enumVal = node["type"].as<std::string>();

		if (!createFns.contains(enumVal)) return nullptr;

		return createFns[enumVal]();
	}
	return nullptr;
}

void connectNodes(YAML::Node& sceneData, SceneData& scene)
{
	auto edges = sceneData["edges"];
	for (auto i = 0L; i < edges.size(); i++)
	{
		auto edge = edges[i];
		if (edge.size() != 4) continue;  // Edge is not valid.

		auto lhs = scene.findNode(edge[0].as<unsigned int>());
		auto rhs = scene.findNode(edge[2].as<unsigned int>());
		if (lhs && rhs)
		{
			auto lhsPin = edge[1].as<unsigned int>();
			auto rhsPin = edge[3].as<unsigned int>();

			auto plugResult = Core::GraphManager::plug(lhs, rhs, lhsPin, rhsPin);
			if (plugResult != ENodePlugResult::Ok)
			{
				Log::info("Cannot connect pin{} to pin{} of nodes {} and {}", lhs->getSig(), rhs->getSig(), lhsPin, rhsPin);
			}
		}
	}
}

SceneData load(const std::string& rawScene)
{
	SceneData scene;

	auto sceneData = YAML::Load(rawScene);
	if (isParsedSceneValid(sceneData))
	{
		// Process operators.
		auto operators = sceneData["operators"];
		for (auto i = 0L; i < operators.size(); i++)
		{
			auto opNode = operators[i];
			auto op			= createOperator(opNode);
			if (op) scene.operators.push_back(op);
		}

		// Connect all nodes.
		connectNodes(sceneData, scene);
	}
	return scene;
}
