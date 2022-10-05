#include "State/NodeDeserializer.h"

#include "Core/Nodes/Transform.h"
#include "GUI/Elements/Nodes/Builder.h"
#include "Utils/JSON.h"

#ifdef WIN32
#undef GetObject
#endif

static OperatorBuilder g_OperatorBuilder;
static TransformBuilder g_TransformBuilder;

namespace NodeDeserializer
{
Ptr<GuiOperator> createOperator(const rapidjson::Value& value)
{
	const auto& type = value["type"].GetString();

	const auto node = g_OperatorBuilder(type);
	const auto coreNode = node->getNodebase();

	assignCommon(value, node);

	if (value.HasMember("value"))
	{
		const auto maybeData =
		    JSON::getData(value["value"], coreNode->getOperation()->inputTypes[0]);
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

	return sequence;
}

Ptr<GuiTransform> createTransform(const rapidjson::Value& value)
{
	I3T_ASSERT(value.IsObject());

	const auto& type = value["type"].GetString();

	const auto node = g_TransformBuilder(type);
	const auto coreNode = node->getNodebase()->as<Core::Transformation>();

	assignCommon(value, node);

	value["synergies"].GetBool() ? coreNode->enableSynergies()
	                             : coreNode->disableSynergies();
	value["locked"].GetBool() ? coreNode->lock() : coreNode->unlock();

	const auto maybeLOD = EnumUtils::value<WorkspaceLevelOfDetail>(
	    std::string(value["LOD"].GetString()));
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

		I3T_ASSERT(coreNode->getDefaultTypes().count(key) == 1);

		switch (coreNode->getDefaultTypes().at(key))
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
	const auto id = value["id"].GetInt();
	const auto position = JSON::getVec2(value["position"].GetArray());

	node->getNodebase()->changeId(id);
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
