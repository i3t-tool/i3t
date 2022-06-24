#include "SerializationVisitor.h"

#include <numeric>
#include <unordered_map>

#include "yaml-cpp/yaml.h"

#include "Core/Nodes/GraphManager.h"
#include "Core/Nodes/Operations.h"
#include "GUI/Elements/Nodes/WorkspaceSequence.h"
#include "GuiAdapter.h"
#include "Logger/Logger.h"
#include "Utils/Format.h"

/// Base format string for node.
constexpr const char* g_baseFormatString = "  - id: {}\n"
                                           "    type: {}\n"
                                           "    position: [{}, {}]\n";

std::optional<NodeData> dumpTransform(Ptr<GuiTransform> guiTransform);

std::string dumpValue(EValueType value, const Core::NodePtr& node)
{
	std::string str;

	switch (value)
	{
	case EValueType::Pulse:
		break;
	case EValueType::Float:
		str = Utils::toString(node->getData().getFloat());
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
	case EValueType::Screen:
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
	auto&                    transforms = sequence->getNodebase()->as<Core::Sequence>()->getMatrices();

	for (const auto& transform : transforms) { transformIds.push_back(std::to_string(transform->getId())); }
	return "[" + Utils::concat(transformIds, ", ") + "]";
}

std::string dumpCycle(const Core::Cycle& cycle) { return fmt::format(""); }

NodeData dumpCamera(const Ptr<GuiCamera>& camera)
{
	static std::string formatString = std::string(g_baseFormatString) +
	    "    proj_transforms: {}\n"
	    "    view_transforms: {}\n";

	std::string projTransforms = dumpTransformIds(camera->getProjection());
	std::string viewTransforms = dumpTransformIds(camera->getView());

	const auto& node = camera->getNodebase();
	NodeData    data;
	data.node =
	    fmt::format(formatString, camera->getId(), node->getOperation()->keyWord, camera->getNodePositionDiwne().x,
	                camera->getNodePositionDiwne().y, projTransforms, viewTransforms);
	for (const auto& in : node->getInputPins())
	{
		if (in.isPluggedIn()) data.edges.push_back(dumpEdge(in));
	}
	return data;
}

NodeData dumpSequence(const Ptr<GuiSequence>& guiSequence)
{
	auto sequence = guiSequence->getNodebase()->as<Core::Sequence>();

	static std::string formatString = std::string(g_baseFormatString) + "    transforms: {}\n";

	std::string transforms = dumpTransformIds(guiSequence);

	NodeData data;
	data.node = fmt::format(formatString, sequence->getId(), sequence->getOperation()->keyWord,
	                        guiSequence->getNodePositionDiwne().x, guiSequence->getNodePositionDiwne().y, transforms);
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
NodeData dumpOperator(Ptr<GuiNode> guiNode)
{
	auto node = guiNode->getNodebase();

#ifdef I3T_DEBUG
	auto operatorType = magic_enum::enum_cast<ENodeType>(node->getOperation()->keyWord);
	I3T_ASSERT(operatorType.has_value() && "Not an operator class.");
#endif

	/// \todo MH consider multiple editable inputs for operator.
	static std::string formatString = std::string(g_baseFormatString) + "    value: {}\n";

	static std::string noOutputValueFormatString = std::string(g_baseFormatString);

	NodeData data;

	if (!node->getOutputPins().empty())
	{
		data.node = fmt::format(formatString, node->getId(), node->getOperation()->keyWord,           // id and type
		                        guiNode->getNodePositionDiwne().x, guiNode->getNodePositionDiwne().y, // position
		                        dumpValue(node->getOutPin(0).getType(), node));                       // value
	}
	else
	{
		data.node = fmt::format(noOutputValueFormatString, node->getId(), node->getOperation()->keyWord,
		                        guiNode->getNodePositionDiwne().x, guiNode->getNodePositionDiwne().y);
	}

	for (const auto& in : node->getInputPins())
	{
		if (in.isPluggedIn()) data.edges.push_back(dumpEdge(in));
	}

	return data;
}

std::optional<NodeData> dumpTransform(Ptr<GuiTransform> guiTransform)
{
	auto transform = guiTransform->getNodebase()->as<Core::Transformation>();

	static std::string formatString = std::string(g_baseFormatString) +
	    "    value: {}\n"
	    "    synergies: {}\n" //PF todo hasMenuSynergies
	    "    locked: {}\n";

	auto sequence = transform->getCurrentSequence();

	NodeData data;
	data.node = fmt::format(formatString, transform->getId(), transform->getOperation()->keyWord,
	                        guiTransform->getNodePositionDiwne().x, guiTransform->getNodePositionDiwne().y, // position
	                        dumpValue(EValueType::Matrix, transform), transform->hasSynergies(), transform->isLocked());

	auto        defaultValues = transform->getDefaultValues();
	const auto& expectedKeys  = Core::getTransformDefaults(transform->getOperation()->keyWord);

	for (auto& [key, _] : expectedKeys) // validate values
	{
		if (defaultValues.count(key) == 0)
		{
			Log::error("Member function of transform '{}' does not export default value with key '{}'.",
			           transform->getOperation()->keyWord, key);
			return std::nullopt;
		}
	}
	data.node += "    defaults: {" + Utils::concat(Utils::concat(defaultValues, ": "), ", ") + "}\n";

	if (transform->hasSavedValue()) { data.node += "    saved_value: " + Utils::toString(transform->getSavedValue()); }

	return data;
}

std::optional<NodeData> dumpScreen(Ptr<GuiScreen> guiScreen)
{
	auto coreNode = guiScreen->getNodebase();

	static std::string formatString = std::string(g_baseFormatString) + "    size: {}\n";

	return dumpOperator(guiScreen);
}

std::optional<NodeData> dumpModel(const Ptr<GuiModel>& guiModel) { return dumpOperator(guiModel); }

//===----------------------------------------------------------------------===//

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

	result += "screens:\n";
	for (const auto& screen : screens) { result += screen; }
	if (screens.empty()) result += "    {}\n";

	result += "edges:\n";
	if (edges.empty()) result += "    {}\n";
	else
		result += Utils::concat(edges, "\n");

	return result;
}

//===----------------------------------------------------------------------===//

/// \see addNodeToPosition(ImVec2 const)
std::map<std::string_view, std::function<Ptr<WorkspaceNodeWithCoreData>(ImVec2 const)>> createFns;

std::map<std::string_view, std::function<Ptr<WorkspaceNodeWithCoreData>(ImVec2 const)>> createTransformFns;

template <int N, int Max>
void doForOperator()
{
	constexpr auto enumValue                    = static_cast<ENodeType>(N);
	createFns[magic_enum::enum_name(enumValue)] = addNodeToNodeEditor<WorkspaceOperator<enumValue>>;

	if constexpr (N < Max) { doForOperator<N + 1, Max>(); }
}

template <int N, int Max>
void doForTransform()
{
	constexpr auto enumValue                             = static_cast<ETransformType>(N);
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

//===-- Visitors ----------------------------------------------------------===//

bool SerializationVisitor::m_isInitialized = false;

SerializationVisitor::SerializationVisitor()
{
	if (!m_isInitialized)
	{
		initCreateFns();
		createFns[n(ENodeType::Model)] = addNodeToNodeEditor<WorkspaceModel>;
		m_isInitialized                = true;
	}
}

std::string SerializationVisitor::dump(const std::vector<Ptr<GuiNode>>& nodes)
{
	m_sceneData.clear();

	for (const auto& node : nodes)
	{
		node->accept(*this); // calls visit(node) and populates m_result variable.
	}

	return m_sceneData.toString();
}

void SerializationVisitor::visit(const Ptr<GuiCamera>& node)
{
	// Dump camera sequences.
	m_sceneData.addCamera(dumpCamera(node));

	for (const auto& transform : node->getProjection()->getInnerWorkspaceNodes())
		if (auto result = dumpTransform(std::static_pointer_cast<GuiTransform>(transform)))
			m_sceneData.addTransform(*result);

	for (const auto& transform : node->getView()->getInnerWorkspaceNodes())
		if (auto result = dumpTransform(std::static_pointer_cast<GuiTransform>(transform)))
			m_sceneData.addTransform(*result);
}

void SerializationVisitor::visit(const Ptr<GuiCycle>& node) {}

void SerializationVisitor::visit(const Ptr<GuiOperator>& guiNode)
{
	auto node = guiNode->getNodebase();
	m_sceneData.addOperator(dumpOperator(guiNode));
}

void SerializationVisitor::visit(const Ptr<GuiSequence>& node)
{
	// Dump sequence and its transforms.
	m_sceneData.addSequence(dumpSequence(node));

	for (const auto& transform : node->getInnerWorkspaceNodes())
		if (auto result = dumpTransform(std::static_pointer_cast<GuiTransform>(transform)))
			m_sceneData.addTransform(*result);
}

void SerializationVisitor::visit(const Ptr<GuiTransform>& node)
{
	// Dump orphaned transform.
	auto result = dumpTransform(node);

	if (result.has_value()) m_sceneData.addTransform(*result);
}

void SerializationVisitor::visit(const Ptr<GuiScreen>& node)
{
	if (auto result = dumpScreen(node)) m_sceneData.addScreen(*result);
}

void SerializationVisitor::visit(const Ptr<GuiModel>& node)
{
	if (auto result = dumpModel(node)) m_sceneData.addScreen(*result);
}

bool isParsedSceneValid(YAML::Node& parsedScene) { return parsedScene["operators"] && parsedScene["edges"]; }

//===-- Builder helpers ---------------------------------------------------===//

GuiNodePtr findNode(std::vector<GuiNodePtr>& nodes, Core::ID id)
{
	for (const auto& node : nodes)
		if (node->getNodebase()->getId() == id) return node;

	return nullptr;
}

void insertTransformsToSequence(YAML::Node& transformIds, Ptr<GuiSequence> sequence, std::vector<GuiNodePtr>& nodes)
{
	int i = 0;

	for (auto&& transformIdRaw : transformIds)
	{
		auto transformId = transformIdRaw.as<int>();
		auto transform   = findNode(nodes, transformId);

		if (transform)
		{
			sequence->moveNodeToSequence(transform, i);
			++i;
		}
	}
}

glm::vec3 buildVec3(YAML::Node& node) { return glm::make_vec3(node.as<std::vector<float>>().data()); }

glm::vec4 buildVec4(YAML::Node& node) { return glm::make_vec4(node.as<std::vector<float>>().data()); }

glm::quat buildQuat(YAML::Node& node) { return glm::make_quat(node.as<std::vector<float>>().data()); }


glm::vec3 buildVec3(YAML::Node&& node)
{
	auto vec = node.as<std::vector<float>>();

	return glm::make_vec3(vec.data());
}

glm::vec4 buildVec4(YAML::Node&& node) { return glm::make_vec4(node.as<std::vector<float>>().data()); }

glm::quat buildQuat(YAML::Node&& node)
{
	return glm::make_quat(node.as<std::vector<float>>().data());
}



glm::mat4 buildMat4(YAML::Node& node)
{
	glm::mat4 result;

	for (int i = 0; i < 4; ++i) result[i] = buildVec4(node[i]);

	return result;
}

glm::mat4 buildMat4(YAML::Node&& node)
{
	glm::mat4 result;

	for (int i = 0; i < 4; ++i) result[i] = buildVec4(node[i]);

	return result;
}

//===-- Builders ----------------------------------------------------------===//

void buildOperator(YAML::Node& node)
{
	if (node["type"])
	{
		auto enumVal = node["type"].as<std::string>();

		if (!createFns.contains(enumVal)) Log::error("Does not know how to load {} operator from file.", enumVal);

		auto id = node["id"].as<int>();

		auto posX = node["position"][0].as<float>();
		auto posY = node["position"][1].as<float>();

		auto op       = createFns[enumVal](ImVec2{posX, posY});
		auto coreNode = op->getNodebase();

		const auto* props = coreNode->getOperation();

		ValueSetResult valueResult;

		if (props->isConstructor)
		{
			const auto outputType = props->outputTypes[0];

			switch (outputType)
			{
			case EValueType::Float:
				valueResult = coreNode->setValue(node["value"].as<float>());
				break;
			case EValueType::Vec3:
				valueResult = coreNode->setValue(buildVec3(node["value"]));
				break;
			case EValueType::Vec4:
				valueResult = coreNode->setValue(buildVec4(node["value"]));
				break;
			case EValueType::Matrix:
				valueResult = coreNode->setValue(buildMat4(node["value"]));
				break;
			default:
				break;
			}
		}

		I3T_ASSERT(valueResult.status == ValueSetResult::Status::Ok);

		if (coreNode->getOperation()->keyWord == n(ENodeType::Model))
		{
			// op->initialize();
		}

		op->getNodebase()->changeId(id);
	}
}

void buildCamera(YAML::Node& node, std::vector<GuiNodePtr>& workspaceNodes)
{
	if (node["type"])
	{
		auto enumVal = node["type"].as<std::string>();

		auto id = node["id"].as<int>();

		auto posX = node["position"][0].as<float>();
		auto posY = node["position"][1].as<float>();

		auto camera = addNodeToNodeEditor<WorkspaceCamera>(ImVec2{posX, posY});

		auto projTransforms = node["proj_transforms"];
		insertTransformsToSequence(projTransforms, camera->getProjection(), workspaceNodes);

		auto viewTransforms = node["view_transforms"];
		insertTransformsToSequence(viewTransforms, camera->getView(), workspaceNodes);

		camera->getNodebase()->changeId(id);
	}
}

void buildTransform(YAML::Node& node)
{
	if (node["type"])
	{
		auto enumVal = node["type"].as<std::string>();

		if (!createTransformFns.contains(enumVal)) Log::error("Does not know how to load {} transform from file.", enumVal);

		auto id = node["id"].as<int>();

		auto posX = node["position"][0].as<float>();
		auto posY = node["position"][1].as<float>();

		auto transform = createTransformFns[enumVal](ImVec2{posX, posY});

		auto coreTransform = transform->getNodebase()->as<Core::Transformation>();

		// PF: moved above setValues - necessary for correct reading of nodes without synergies
		// as setValue() calls resetMatrixFormDefaults() that normalizes, e.g, the quaternion
		node["synergies"].as<bool>() ? coreTransform->enableSynergies()
		                             : coreTransform->disableSynergies(); //PF todo hasSynergies
		// transform default values
		const auto& transformDefaults = Core::getTransformDefaults(enumVal);
		// for (auto&& [key, val] : node["defaults"])
		for (auto it = node["defaults"].begin(); it != node["defaults"].end(); ++it)
		{
			auto key = it->first.as<std::string>();
			auto val = it->second;

			auto type = transformDefaults.at(key);
			switch (type)
			{
			case EValueType::Float:
				coreTransform->setDefaultValue(key, val.as<float>());
				break;
			case EValueType::Vec3:
				coreTransform->setDefaultValue(key, buildVec3(val));
				break;
			case EValueType::Quat:
			{
				//const auto vec4 = buildVec4(val);
				//coreTransform->setDefaultValue(key, glm::make_quat(glm::value_ptr(vec4)));
				const auto quat = buildQuat(val);
				coreTransform->setDefaultValue(key, quat);
				///const auto quat = glm::quat(val[3], val[2], val[1], val[0]);

				// todo set LOD SetValues
				
				break;
			}
			default:
				break;
			}
			// coreTransform->setDefaultValue(keyStr, node["defaults"][key]);
		}

		coreTransform->getInternalData(0).setValue(buildMat4(node["value"]));

		if (node["saved_value"]) { coreTransform->setSavedValue(buildMat4(node["saved_value"])); }

		node["locked"].as<bool>() ? coreTransform->lock() : coreTransform->unlock();

		coreTransform->changeId(id);
	}
}

/// Should be called after processing transformation.
void buildSequence(YAML::Node& node, std::vector<GuiNodePtr>& workspaceNodes)
{
	if (node["type"])
	{
		auto enumVal = node["type"].as<std::string>();
		I3T_ASSERT(enumVal == Core::g_sequence.keyWord);

		auto id = node["id"].as<int>();

		auto posX = node["position"][0].as<float>();
		auto posY = node["position"][1].as<float>();

		auto sequence =
		    I3T::getWindowPtr<WorkspaceWindow>()->getNodeEditor().addNodeToPosition<WorkspaceSequence>({posX, posY});

		auto transformIds = node["transforms"];
		insertTransformsToSequence(transformIds, sequence, workspaceNodes);

		sequence->getNodebase()->changeId(id);
	}
}

void buildScreen(YAML::Node& node)
{
	if (node["type"])
	{
		auto id = node["id"].as<int>();

		auto posX = node["position"][0].as<float>();
		auto posY = node["position"][1].as<float>();

		auto screen = addNodeToNodeEditor<GuiScreen>(ImVec2{posX, posY});

		screen->getNodebase()->changeId(id);
	}
}

//===----------------------------------------------------------------------===//

void connectNodes(YAML::Node& sceneData, SceneData& scene, GuiNodes& workspaceNodes)
{
	auto edges = sceneData["edges"];
	for (auto&& edge : edges)
	{
		if (edge.size() != 4) continue; // Edge is not valid.

		auto lhs = findNode(workspaceNodes, edge[0].as<unsigned int>());
		auto rhs = findNode(workspaceNodes, edge[2].as<unsigned int>());
		if (lhs && rhs)
		{
			auto lhsPin = edge[1].as<unsigned int>();
			auto rhsPin = edge[3].as<unsigned int>();

			auto plugResult = Core::GraphManager::plug(lhs->getNodebase(), rhs->getNodebase(), lhsPin, rhsPin);
			if (plugResult != ENodePlugResult::Ok)
				Log::info("Cannot connect pin{} to pin{} of nodes {} and {}", lhs->getNodebase()->getSig(),
				          rhs->getNodebase()->getSig(), lhsPin, rhsPin);
			else
				std::static_pointer_cast<WorkspaceNodeWithCoreDataWithPins>(rhs)
				    ->getInputs()
				    .at(rhsPin)
				    ->setConnectedWorkspaceOutput(
				        std::static_pointer_cast<WorkspaceNodeWithCoreDataWithPins>(lhs)->getOutputs().at(lhsPin).get());
		}
	}
}

SceneData buildScene(const std::string& rawScene, GuiNodes& workspaceNodes)
{
	SceneData scene;

	/// \todo MH
	SerializationVisitor visitor;

	auto sceneData = YAML::Load(rawScene);
	if (isParsedSceneValid(sceneData))
	{
		// Process operators.
		auto operators = sceneData["operators"];
		for (auto&& op : operators) buildOperator(op);

		// Process transforms
		auto transforms = sceneData["transforms"];
		for (auto&& transform : transforms) buildTransform(transform);

		auto sequences = sceneData["sequences"];
		for (auto&& sequence : sequences) buildSequence(sequence, workspaceNodes);

		auto cameras = sceneData["cameras"];
		for (auto&& camera : cameras) buildCamera(camera, workspaceNodes);

		auto screens = sceneData["screens"];
		for (auto&& screen : screens) buildScreen(screen);

		// Connect all nodes.
		connectNodes(sceneData, scene, workspaceNodes);
	}
	return scene;
}
