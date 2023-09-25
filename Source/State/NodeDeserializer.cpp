#include "State/NodeDeserializer.h"

#include <algorithm>

#include "Core/Nodes/Transform.h"
#include "GUI/Elements/Nodes/Builder.h"
#include "GUI/Elements/Nodes/Tools.h"
#include "Utils/JSON.h"
#include "Viewport/entity/nodes/SceneModel.h"

#ifdef WIN32
#undef GetObject
#endif

static OperatorBuilder g_OperatorBuilder;
static TransformBuilder g_TransformBuilder;

namespace NodeDeserializer
{
std::vector<Ptr<GuiNode>> createFrom(const Memento& memento)
{
	std::map<Core::ID, Ptr<WorkspaceNodeWithCoreData>> createdNodes;

	const auto& operators = memento["workspace"]["operators"];
	for (auto& value : operators.GetArray())
	{
		const auto node = NodeDeserializer::createOperator(value);
		NodeDeserializer::assignCommon(value, node);
		createdNodes[value["id"].GetInt()] = node;
	}

	//

	for (auto& value : memento["workspace"]["sequences"].GetArray())
	{
		const auto node = NodeDeserializer::createSequence(value);
		NodeDeserializer::assignCommon(value, node);
		createdNodes[value["id"].GetInt()] = node;
	}

	//

	for (auto& value : memento["workspace"]["cycles"].GetArray())
	{
		const auto cycle = addNodeToNodeEditorNoSave<WorkspaceCycle>();
		const auto coreCycle = cycle->getNodebase()->as<Core::Cycle>();
		createdNodes[value["id"].GetInt()] = cycle;
		NodeDeserializer::assignCommon(value, cycle);

		if (value.HasMember("from"))
		{
			const auto from = value["from"].GetFloat();
			coreCycle->setFrom(from);
		}

		if (value.HasMember("to"))
		{
			const auto to = value["to"].GetFloat();
			coreCycle->setTo(to);
		}

		if (value.HasMember("manualStep"))
		{
			const auto step = value["manualStep"].GetFloat();
			coreCycle->setManualStep(step);
		}

		if (value.HasMember("step"))
		{
			const auto multiplier = value["step"].GetFloat();
			coreCycle->setStep(multiplier);
		}

		if (value.HasMember("stepDuration"))
		{
			const auto duration = value["stepDuration"].GetFloat();
			coreCycle->setStepDuration(duration);
		}

		if (value.HasMember("smooth"))
		{
			const auto smooth = value["smooth"].GetBool();
			coreCycle->setSmoothStep(smooth);
		}
	}

	//

	for (auto& value : memento["workspace"]["cameras"].GetArray())
	{
		const auto camera = addNodeToNodeEditorNoSave<WorkspaceCamera>();
		NodeDeserializer::assignCommon(value, camera);
		createdNodes[value["id"].GetInt()] = camera;

		const auto& viewValue = value["sequences"].GetArray()[0];
		NodeDeserializer::assignSequence(viewValue, camera->getView());
		createdNodes[viewValue["id"].GetInt()] = camera->getView();

		const auto& projValue = value["sequences"].GetArray()[1];
		NodeDeserializer::assignSequence(projValue, camera->getProjection());
		createdNodes[projValue["id"].GetInt()] = camera->getProjection();
	}

	//

	for (auto& value : memento["workspace"]["screens"].GetArray())
	{
		const auto screen = addNodeToNodeEditorNoSave<WorkspaceScreen>();
		NodeDeserializer::assignCommon(value, screen);
		createdNodes[value["id"].GetInt()] = screen;

		if (value.HasMember("aspect"))
		{
			const auto aspect = JSON::getVec2(value["aspect"].GetArray());
			screen->setAspect(aspect);
		}
	}

	//

	for (auto& value : memento["workspace"]["models"].GetArray())
	{
		const auto model = addNodeToNodeEditorNoSave<WorkspaceModel>();
		NodeDeserializer::assignCommon(value, model);
		createdNodes[value["id"].GetInt()] = model;

		auto mesh = model->viewportModel().lock();

		if (value.HasMember("model"))
		{
			const auto* alias = value["model"].GetString();
			mesh->setModel(alias);
		}
		else
		{
			LOG_WARN("Model node {} has no model set, using the default one.", model->getNodebase()->getId());
		}

		if (value.HasMember("visible"))
		{
			mesh->m_visible = value["visible"].GetBool();
		}

		if (value.HasMember("showAxes"))
		{
			mesh->m_showAxes = value["showAxes"].GetBool();
		}

		if (value.HasMember("opaque"))
		{
			mesh->m_opaque = value["opaque"].GetBool();
		}

		if (value.HasMember("opacity"))
		{
			mesh->m_opacity = value["opacity"].GetFloat();
		}

		if (value.HasMember("tint"))
		{
			mesh->m_tint = JSON::getVec3(value["tint"].GetArray());
		}

		if (value.HasMember("tintStrength"))
		{
			mesh->m_tintStrength = value["tintStrength"].GetFloat();
		}
	}

	//

	const auto& transforms = memento["workspace"]["transforms"];
	for (auto& value : transforms.GetArray())
	{
		const auto transform = NodeDeserializer::createTransform(value);
		NodeDeserializer::assignCommon(value, transform);
		createdNodes[value["id"].GetInt()] = transform;
	}

	// connect edges

	const auto& edges = memento["workspace"]["edges"];
	auto& workspaceNodes = App::getModule<UIModule>()
	                           .getWindowManager()
	                           .getWindowPtr<WorkspaceWindow>()
	                           ->getNodeEditor()
	                           .m_workspaceCoreNodes;

	for (auto& edge : edges.GetArray())
	{
		if (!createdNodes.contains(edge[0].GetInt()) || !createdNodes.contains(edge[2].GetInt()))
		{
			LOG_ERROR("Unable to deserialize link between nodes with indexes {} and {}, nodes not found in JSON data.",
			          edge[0].GetInt(), edge[2].GetInt());
			continue;
		}

		auto lhs = createdNodes.at(edge[0].GetInt());
		auto rhs = createdNodes.at(edge[2].GetInt());

		auto lhsPin = edge[1].GetInt();
		auto rhsPin = edge[3].GetInt();

		connectNodesNoSave(lhs, rhs, lhsPin, rhsPin);
	}

	std::vector<Ptr<WorkspaceNodeWithCoreData>> result;
	std::transform(createdNodes.begin(), createdNodes.end(), std::back_inserter(result), [](auto& pair) {
		return pair.second;
	});

	return result;
}

Ptr<GuiOperator> createOperator(const rapidjson::Value& value)
{
	const auto& type = value["type"].GetString();

	Ptr<GuiOperator> node;

	// Workaround for #311
	if (type == n(Core::EOperatorType::AngleAxisToQuat))
	{
		auto result = addNodeToNodeEditorNoSave<WorkspaceAngleAxisToQuat>();
		if (value.HasMember("halfAngle"))
		{
			const auto halfAngle = value["halfAngle"].GetBool();
			result->m_halfAngle = halfAngle;
		}
		node = result;
	}
	else
	{
		node = g_OperatorBuilder(type);
	}

	node->setSelected(true);
	node->processSelect();
	const auto coreNode = node->getNodebase();

	assignCommon(value, node);

	if (value.HasMember("value"))
	{
		using namespace Core;

		const auto maybeData = JSON::getData(value["value"], coreNode->getOperation()->inputTypes[0]);
		if (maybeData.has_value())
		{
			const auto& data = *maybeData;
			switch (data.opValueType)
			{
			case Core::EValueType::Float:
			{
				coreNode->setValue(data.getFloat());
				break;
			}
			case Core::EValueType::Vec3:
			{
				coreNode->setValue(data.getVec3());
				break;
			}
			case Core::EValueType::Vec4:
			{
				coreNode->setValue(data.getVec4());
				break;
			}
			case Core::EValueType::Matrix:
			{
				coreNode->setValue(data.getMat4());
				break;
			}
			case Core::EValueType::Quat:
			{
				coreNode->setValue(data.getQuat());
			}
			case Core::EValueType::Pulse:
			case Core::EValueType::MatrixMul:
			case Core::EValueType::Screen:
			case Core::EValueType::Ptr:
				break;
			}
		}
	}
	return node;
}

Ptr<GuiSequence> createSequence(const rapidjson::Value& value)
{
	auto sequence = addNodeToNodeEditorNoSave<WorkspaceSequence>();

	assignCommon(value, sequence);
	assignSequence(value, sequence);
	sequence->setSelected(true);
	sequence->processSelect();
	return sequence;
}

Ptr<GuiTransform> createTransform(const rapidjson::Value& value)
{
	I3T_ASSERT(value.IsObject(), "Invalid value type");

	const auto& type = value["type"].GetString();

	const auto node = g_TransformBuilder(type);
	node->setSelected(true);
	node->processSelect();
	const auto coreNode = node->getNodebase()->as<Core::Transform>();

	assignCommon(value, node);

	value["synergies"].GetBool() ? coreNode->enableSynergies() : coreNode->disableSynergies();
	value["locked"].GetBool() ? coreNode->lock() : coreNode->unlock();

	const auto maybeLOD = EnumUtils::value<WorkspaceLevelOfDetail>(std::string(value["LOD"].GetString()));
	if (maybeLOD.has_value())
	{
		node->setLevelOfDetail(maybeLOD.value());
	}

	if (value.HasMember("savedValue"))
	{
		coreNode->setSavedValue(JSON::getMat(value["savedValue"]));
	}
	coreNode->setValue(JSON::getMat(value["value"]));

	if (!value.HasMember("defaultValues"))
	{
		return node;
	}

	for (const auto& defaultValue : value["defaultValues"].GetObject())
	{
		const std::string key(defaultValue.name.GetString());

		const auto defaultValueType = coreNode->properties()->getDefaultValueType(key);
		if (!defaultValueType.has_value())
		{
			LOG_ERROR("Unknown default value {}", key);
			continue;
		}

		switch (defaultValueType.value().type)
		{
		case Core::EValueType::Float:
		{
			coreNode->setDefaultValue(key, defaultValue.value.GetFloat());
			break;
		}
		case Core::EValueType::Vec3:
		{
			coreNode->setDefaultValue(key, JSON::getVec3(defaultValue.value));
			break;
		}
		case Core::EValueType::Vec4:
		{
			coreNode->setDefaultValue(key, JSON::getVec4(defaultValue.value));
			break;
		}
		case Core::EValueType::Matrix:
		{
			coreNode->setDefaultValue(key, JSON::getMat(defaultValue.value));
			break;
		}
		case Core::EValueType::Quat:
		case Core::EValueType::Pulse:
		case Core::EValueType::MatrixMul:
		case Core::EValueType::Screen:
		case Core::EValueType::Ptr:
			break;
		}
	}
	return node;
}

//

void assignCommon(const rapidjson::Value& value, Ptr<GuiNode> node)
{
	if (value.HasMember("label"))
	{
		node->setTopLabel(value["label"].GetString());
	}

	if (value.HasMember("render"))
	{
		node->setRender(value["render"].GetBool());
	}

	if (value.HasMember("numberOfDecimals"))
	{
		node->setNumberOfVisibleDecimal(value["numberOfDecimals"].GetInt());
	}

	if (value.HasMember("LOD"))
	{
		const std::string LODName = value["LOD"].GetString();
		node->setLevelOfDetail(
		    EnumUtils::value<WorkspaceLevelOfDetail>(LODName).value_or(WorkspaceLevelOfDetail::Full));
	}

	const auto position = JSON::getVec2(value["position"].GetArray());
	node->setNodePositionDiwne(position);
}

void assignSequence(const rapidjson::Value& value, Ptr<GuiSequence> sequence)
{
	std::vector<Ptr<WorkspaceTransformation>> transforms;
	for (const auto& transform : value["transforms"].GetArray())
	{
		transforms.push_back(NodeDeserializer::createTransform(transform));
	}

	// WorkspaceSequence::moveNodeToSequence function pushes new matrix
	// at the beginning of the sequence.
	std::reverse(transforms.begin(), transforms.end());

	for (auto& transform : transforms)
	{
		sequence->moveNodeToSequence(transform);
	}
}
} // namespace NodeDeserializer
