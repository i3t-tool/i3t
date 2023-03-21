#include "State/NodeDeserializer.h"

#include "Core/Nodes/Transform.h"
#include "GUI/Elements/Nodes/Builder.h"
#include "GUI/Elements/Nodes/Tools.h"
#include "Utils/JSON.h"
#include "Viewport/entity/SceneModel.h"

#ifdef WIN32
#undef GetObject
#endif

static OperatorBuilder g_OperatorBuilder;
static TransformBuilder g_TransformBuilder;

namespace NodeDeserializer
{
void createFrom(const Memento& memento)
{
	// for edges between nodes
	std::map<std::size_t, Core::ID> oldToNewId;

	const auto& operators = memento["workspace"]["operators"];
	for (auto& value : operators.GetArray())
	{
		const auto node = NodeDeserializer::createOperator(value);
		oldToNewId[value["id"].GetInt()] = node->getNodebase()->getId();
	}

	//

	for (auto& value : memento["workspace"]["sequences"].GetArray())
	{
		const auto node = NodeDeserializer::createSequence(value);
		oldToNewId[value["id"].GetInt()] = node->getNodebase()->getId();
	}

	//

	for (auto& value : memento["workspace"]["cycles"].GetArray())
	{
		const auto cycle = addNodeToNodeEditorNoSave<WorkspaceCycle>();
		cycle->setSelected(true);
		NodeDeserializer::assignCommon(value, cycle);
		oldToNewId[value["id"].GetInt()] = cycle->getNodebase()->getId();
	}

	//

	for (auto& value : memento["workspace"]["cameras"].GetArray())
	{
		const auto camera = addNodeToNodeEditorNoSave<WorkspaceCamera>();
		camera->setSelected(true);
		NodeDeserializer::assignCommon(value, camera);
		oldToNewId[value["id"].GetInt()] = camera->getNodebase()->getId();

		const auto& viewValue = value["sequences"].GetArray()[0];
		NodeDeserializer::assignSequence(viewValue, camera->getView());

		const auto& projValue = value["sequences"].GetArray()[1];
		NodeDeserializer::assignSequence(projValue, camera->getProjection());
	}

	//

	for (auto& value : memento["workspace"]["screens"].GetArray())
	{
		const auto screen = addNodeToNodeEditorNoSave<WorkspaceScreen>();
		screen->setSelected(true);
		NodeDeserializer::assignCommon(value, screen);
		oldToNewId[value["id"].GetInt()] = screen->getNodebase()->getId();
	}

	//

	for (auto& value : memento["workspace"]["models"].GetArray())
	{
		const auto model = addNodeToNodeEditorNoSave<WorkspaceModel>();
		model->setSelected(true);
		NodeDeserializer::assignCommon(value, model);
		oldToNewId[value["id"].GetInt()] = model->getNodebase()->getId();

		if (value.HasMember("model"))
		{
			const auto* alias = value["model"].GetString();
			model->viewportModel().lock()->setModel(alias);
		}
		else
		{
			LOG_WARN("Model node {} has no model set, using the default one.", model->getNodebase()->getId());
		}
	}

	//

	const auto& transforms = memento["workspace"]["transforms"];
	for (auto& value : transforms.GetArray())
	{
		const auto transform = NodeDeserializer::createTransform(value);
		oldToNewId[value["id"].GetInt()] = transform->getNodebase()->getId();
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
		if (!oldToNewId.contains(edge[0].GetInt()) || !oldToNewId.contains(edge[2].GetInt()))
		{
			LOG_ERROR("Unable to deserialize link between nodes with indexes {} and {}.", edge[0].GetInt(), edge[2].GetInt());
			continue;
		}

		auto lhsID = oldToNewId.at(edge[0].GetInt());
		auto rhsID = oldToNewId.at(edge[2].GetInt());

		auto maybeLhs = findNodeById(workspaceNodes, lhsID);
		auto maybeRhs = findNodeById(workspaceNodes, rhsID);
		if (maybeLhs.has_value() && maybeRhs.has_value())
		{
			const auto lhs = maybeLhs.value();
			const auto rhs = maybeRhs.value();

			auto lhsPin = edge[1].GetInt();
			auto rhsPin = edge[3].GetInt();

			connectNodesNoSave(lhs, rhs, lhsPin, rhsPin);
		}
	}
}

Ptr<GuiOperator> createOperator(const rapidjson::Value& value)
{
	const auto& type = value["type"].GetString();

	const auto node = g_OperatorBuilder(type);
	node->setSelected(true);
	const auto coreNode = node->getNodebase();

	assignCommon(value, node);

	if (value.HasMember("value"))
	{
		const auto maybeData = JSON::getData(value["value"], coreNode->getOperation()->inputTypes[0]);
		if (maybeData.has_value())
		{
			const auto& data = *maybeData;
			switch (data.opValueType)
			{
			case EValueType::Float:
			{
				coreNode->setValue(data.getFloat());
				break;
			}
			case EValueType::Vec3:
			{
				coreNode->setValue(data.getVec3());
				break;
			}
			case EValueType::Vec4:
			{
				coreNode->setValue(data.getVec4());
				break;
			}
			case EValueType::Matrix:
			{
				coreNode->setValue(data.getMat4());
				break;
			}
			case EValueType::Quat:
			case EValueType::Pulse:
			case EValueType::MatrixMul:
			case EValueType::Screen:
			case EValueType::Ptr:
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
	return sequence;
}

Ptr<GuiTransform> createTransform(const rapidjson::Value& value)
{
	I3T_ASSERT(value.IsObject(), "Invalid value type");

	const auto& type = value["type"].GetString();

	const auto node = g_TransformBuilder(type);
	node->setSelected(true);
	const auto coreNode = node->getNodebase()->as<Core::Transformation>();

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
		return node;

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
		case EValueType::Float:
		{
			coreNode->setDefaultValue(key, defaultValue.value.GetFloat());
			break;
		}
		case EValueType::Vec3:
		{
			coreNode->setDefaultValue(key, JSON::getVec3(defaultValue.value));
			break;
		}
		case EValueType::Vec4:
		{
			coreNode->setDefaultValue(key, JSON::getVec4(defaultValue.value));
			break;
		}
		case EValueType::Matrix:
		{
			coreNode->setDefaultValue(key, JSON::getMat(defaultValue.value));
			break;
		}
		case EValueType::Quat:
		case EValueType::Pulse:
		case EValueType::MatrixMul:
		case EValueType::Screen:
		case EValueType::Ptr:
			break;
		}
	}
	return node;
}

//

void assignCommon(const rapidjson::Value& value, Ptr<GuiNode> node)
{
	const auto position = JSON::getVec2(value["position"].GetArray());
	node->setNodePositionDiwne(position);
}

void assignSequence(const rapidjson::Value& value, Ptr<GuiSequence> sequence)
{
	for (const auto& transform : value["transforms"].GetArray())
	{
		sequence->moveNodeToSequence(NodeDeserializer::createTransform(transform));
	}
}
} // namespace NodeDeserializer
