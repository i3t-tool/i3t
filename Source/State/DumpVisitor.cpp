#include "DumpVisitor.h"

#include <numeric>
#include <unordered_map>

#include "yaml-cpp/yaml.h"

#include "GuiAdapter.h"
#include "Core/Nodes/Cycle.h"
#include "Core/Nodes/GraphManager.h"
#include "GUI//Elements/Windows/WorkspaceWindow.h"
#include "GUI/Elements/Nodes/WorkspaceSequence.h"
#include "Logger/Logger.h"
#include "Utils/Format.h"

/// Base format string for node.
constexpr const char* g_baseFormatString = "  - id: {}\n"
																					 "    type: {}\n"
																					 "    position: [{}, {}]\n";

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
		str = Utils::toString(node->getData().getQuat());
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

std::string dumpTransformIds(const Ptr<GuiSequence>& sequence)
{
	std::vector<std::string> transformIds;
	auto& transforms = sequence->getNodebase()->as<Core::Sequence>()->getMatrices();

	for (const auto& transform : transforms) { transformIds.push_back(std::to_string(transform->getId())); }
	return "[" + Utils::concat(transformIds, ", ") + "]";
}

std::string dumpCycle(const Core::Cycle& cycle) { return fmt::format(""); }

NodeData dumpCamera(const Ptr<GuiCamera>& camera)
{
	return NodeData{};

	/*
	static std::string formatString = std::string(g_baseFormatString) +
			"    proj_transforms: {}\n"
			"    view_transforms: {}\n";

	std::string projTransforms = dumpTransformIds(camera->getProj());
	std::string viewTransforms = dumpTransformIds(camera->getView());

	NodeData data;
	data.node =
			fmt::format(formatString, camera->getId(), camera->getOperation()->keyWord, projTransforms, viewTransforms);
	for (const auto& in : camera->getInputPins())
	{
		if (in.isPluggedIn()) data.edges.push_back(dumpEdge(in));
	}
	return data;
	 */
}

NodeData dumpSequence(const Ptr<GuiSequence>& guiSequence)
{
	auto sequence = guiSequence->getNodebase()->as<Core::Sequence>();

	static std::string formatString = std::string(g_baseFormatString) + "    transforms: {}\n";

	std::string transforms = dumpTransformIds(guiSequence);

	NodeData data;
	data.node = fmt::format(formatString, sequence->getId(), sequence->getOperation()->keyWord,
													guiSequence->getNodePositionDiwne().x, guiSequence->getNodePositionDiwne().y,
													transforms);
	for (const auto& in : sequence->getInputPins())
	{
		if (in.isPluggedIn()) data.edges.push_back(dumpEdge(in));
	}
	return data;
}

/**
 * \param node
 * \return Nested block in YAML format.
 */
NodeData dumpOperator(Ptr<NodeClass> guiNode)
{
	auto node = guiNode->getNodebase();

#ifdef I3T_DEBUG
	auto operatorType = magic_enum::enum_cast<ENodeType>(node->getOperation()->keyWord);
	I3T_ASSERT(operatorType.has_value() && "Not an operator class.");
#endif

	/// \todo MH consider multiple editable inputs for operator.
	static std::string formatString = std::string(g_baseFormatString) + "    value: {}\n";

	NodeData data;
	data.node = fmt::format(formatString, node->getId(), node->getOperation()->keyWord,            // id and type
													guiNode->getNodePositionDiwne().x, guiNode->getNodePositionDiwne().y,  // position
													dumpValue(node->getOutPin(0).getType(), node));                        // value

	for (const auto& in : node->getInputPins())
	{
		if (in.isPluggedIn()) data.edges.push_back(dumpEdge(in));
	}

	return data;
}

std::optional<NodeData> dumpTransform(Ptr<TransformClass> guiTransform)
{
	auto transform = guiTransform->getNodebase()->as<Core::Transformation>();

	static std::string formatString = std::string(g_baseFormatString) +
			"    value: {}\n"
			"    synergies: {}\n"
			"    locked: {}\n";

	auto sequence = transform->getCurrentSequence();

	NodeData data;
	data.node = fmt::format(formatString, transform->getId(), transform->getOperation()->keyWord,
													guiTransform->getNodePositionDiwne().x, guiTransform->getNodePositionDiwne().y,  // position
													dumpValue(EValueType::Matrix, transform),
													transform->hasSynergies(), transform->isLocked());

	auto				defaultValues = transform->getDefaultValues();
	const auto& expectedKeys	= Core::getTransformDefaults(transform->getOperation()->keyWord);

	for (auto& [key, _] : expectedKeys)  // validate values
	{
		if (defaultValues.count(key) == 0)
		{
			Log::error("Member function of transform '{}' does not export default value with key '{}'.",
								 transform->getOperation()->keyWord, key);
			return std::nullopt;
		}
	}
	data.node += "    defaults: {" + Utils::concat(Utils::concat(defaultValues, ": "), ", ") + "}\n";

	return data;
}

std::string SceneRawData::toString() const
{
	std::string result;

	result += "operators:\n";
	for (const auto& op : operators) { result += op; }
	if (operators.empty()) result += "    {}\n";

	result += "transforms:\n";
	for (const auto& transform : transforms) { result += transform; }
	if (transforms.empty()) result += "    {}\n";

	result += "sequences:\n";
	for (const auto& sequence : sequences) { result += sequence; }
	if (sequences.empty()) result += "    {}\n";

	result += "cameras:\n";
	for (const auto& camera : cameras) { result += camera; }
	if (cameras.empty()) result += "    {}\n";

	result += "edges:\n";
	if (edges.empty()) result += "    {}\n";
	else result += Utils::concat(edges, "\n");

	return result;
}

/// \see addNodeToPosition(ImVec2 const)
std::map<std::string_view, std::function<void(ImVec2 const)>> createFns;
std::map<std::string_view, std::function<void(ImVec2 const)>> createTransformFns;

template <int N, int Max>
void doForOperator()
{
	constexpr auto enumValue = static_cast<ENodeType>(N);

	createFns[magic_enum::enum_name(enumValue)]
			= WorkspaceWindow::addNodeToPosition<WorkspaceOperator<enumValue>>;

	if constexpr (N < Max) { doForOperator<N + 1, Max>(); }
}

template <int N, int Max>
void doForTransform()
{
	constexpr auto enumValue = static_cast<ETransformType>(N);

	createTransformFns[magic_enum::enum_name(enumValue)] = createTransform<enumValue>;

	if constexpr (N < Max) { doForTransform<N + 1, Max>(); }
}

void initCreateFns()
{
	constexpr std::size_t operatorCount = magic_enum::enum_count<ENodeType>() - 1;
	doForOperator<0, operatorCount>();

	constexpr std::size_t transformCount = magic_enum::enum_count<ETransformType>() - 1;
	doForTransform<0, transformCount>();
}

bool DumpVisitor::m_isInitialized = false;

DumpVisitor::DumpVisitor()
{
	if (!m_isInitialized)
	{
		initCreateFns();
		m_isInitialized = true;
	}
}

std::string DumpVisitor::dump(const std::vector<Ptr<NodeClass>>& nodes)
{
	m_sceneData.clear();

	for (const auto& node : nodes)
	{
		node->accept(*this); // calls visit(node) and populates m_result variable.
	}

	return m_sceneData.toString();
}

void DumpVisitor::visit(const Ptr<GuiCamera>& node)
{
	// Dump camera sequences.
	m_sceneData.addCamera(dumpCamera(node));
}

void DumpVisitor::visit(const Ptr<GuiCycle>& node)
{

}

void DumpVisitor::visit(const Ptr<GuiOperator>& guiNode)
{
	auto node = guiNode->getNodebase();
	m_sceneData.addOperator(dumpOperator(guiNode));
}

void DumpVisitor::visit(const Ptr<GuiSequence>& node)
{
	// Dump sequence and its transforms.
	m_sceneData.addSequence(dumpSequence(node));
}

void DumpVisitor::visit(const Ptr<GuiTransform>& node)
{
	// Dump orphaned transform.
	auto result = dumpTransform(node);

	if (result.has_value()) m_sceneData.addTransform(*result);
}

bool isParsedSceneValid(YAML::Node& parsedScene) { return parsedScene["operators"] && parsedScene["edges"]; }

void createOperator(YAML::Node& node)
{
	if (node["type"])
	{
		auto enumVal = node["type"].as<std::string>();

		if (!createFns.contains(enumVal))
		{
			Log::error("Does not know how to load {} operator from file.", enumVal);
		}

		auto posX = node["position"][0].as<float>();
		auto posY = node["position"][1].as<float>();

		createFns[enumVal](ImVec2{ posX, posY });
	}
}

void parseTransform(YAML::Node& node)
{
	if (node["type"])
	{
		auto enumVal = node["type"].as<std::string>();

		if (!createTransformFns.contains(enumVal))
		{
			Log::error("Does not know how to load {} transform from file.", enumVal);
		}

		auto posX = node["position"][0].as<float>();
		auto posY = node["position"][1].as<float>();

		createTransformFns[enumVal](ImVec2{ posX, posY });
	}
}

void connectNodes(YAML::Node& sceneData, SceneData& scene)
{
	auto edges = sceneData["edges"];
	for (auto i = 0L; i < edges.size(); i++)
	{
		auto edge = edges[i];
		if (edge.size() != 4) continue; // Edge is not valid.

		auto lhs = scene.findNode(edge[0].as<unsigned int>());
		auto rhs = scene.findNode(edge[2].as<unsigned int>());
		if (lhs && rhs)
		{
			auto lhsPin = edge[1].as<unsigned int>();
			auto rhsPin = edge[3].as<unsigned int>();

			auto plugResult = Core::GraphManager::plug(lhs->getNodebase(), rhs->getNodebase(), lhsPin, rhsPin);
			if (plugResult != ENodePlugResult::Ok)
			{
				Log::info("Cannot connect pin{} to pin{} of nodes {} and {}", lhs->getNodebase()->getSig(), rhs->getNodebase()->getSig(), lhsPin, rhsPin);
			}
		}
	}
}

SceneData loadScene(const std::string& rawScene)
{
	SceneData scene;

	/// \todo MH
	DumpVisitor visitor;

	auto sceneData = YAML::Load(rawScene);
	if (isParsedSceneValid(sceneData))
	{
		// Process operators.
		auto operators = sceneData["operators"];
		for (auto&& op : operators)
			createOperator(op);

		// PRocess transforms
		auto transforms = sceneData["transforms"];
		for (auto&& transform : transforms)
			parseTransform(transform);

		// Connect all nodes.
		connectNodes(sceneData, scene);
	}
	return scene;
}

SceneData loadSceneFromFile(const std::string& sceneFile)
{
	std::ifstream f(sceneFile);
	std::string rawScene;
	if (f)
	{
		std::ostringstream ss;
		ss << f.rdbuf(); // reading data
		rawScene = ss.str();
	}
	return loadScene(rawScene);
}

bool saveScene(const std::string& filename, const SceneData& scene)
{
	DumpVisitor visitor;
	std::string rawState = visitor.dump(scene.nodes);
	std::ofstream f(filename + ".scene");
	f << rawState;

	return true;
}
