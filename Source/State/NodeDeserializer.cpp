/**
 * \file
 * \brief
 * \author Martin Herich <martin.herich@phire.cz>
 * \copyright Copyright (C) 2016-2023 I3T team, Department of Computer Graphics
 * and Interaction, FEE, Czech Technical University in Prague, Czech Republic
 *
 * This file is part of I3T - An Interactive Tool for Teaching Transformations
 * http://www.i3t-tool.org
 *
 * GNU General Public License v3.0 (see LICENSE.txt or https://www.gnu.org/licenses/gpl-3.0.txt)
 */
#include "State/NodeDeserializer.h"

#include <algorithm>

#include "Core/Nodes/Transform.h"
#include "GUI/Elements/Windows/WorkspaceWindow.h"
#include "GUI/Workspace/Builder.h"
#include "GUI/Workspace/Nodes/ScriptingNode.h"
#include "GUI/Workspace/Tools.h"
#include "Utils/JSON.h"
#include "Viewport/entity/nodes/SceneModel.h"

#ifdef WIN32
#undef GetObject
#endif

static Workspace::OperatorBuilder g_OperatorBuilder;
static Workspace::TransformBuilder g_TransformBuilder;

namespace NodeDeserializer
{
std::vector<Ptr<DIWNE::Node>> createFrom(const Memento& memento)
{
	std::map<Core::ID, Ptr<Workspace::CoreNode>> createdNodes;

	const auto& operators = memento["workspace"]["operators"];
	for (auto& value : operators.GetArray())
	{
		const auto maybeNode = NodeDeserializer::createOperator(value);
		if (maybeNode)
		{
			const auto node = *maybeNode;
			NodeDeserializer::assignCommon(value, node);
			createdNodes[value["id"].GetInt()] = node;
		}
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
		const auto cycle = Workspace::addNodeToNodeEditorNoSave<Workspace::Cycle>();
		const auto coreCycle = cycle->getNodebase()->as<Core::Cycle>();
		createdNodes[value["id"].GetInt()] = cycle;
		NodeDeserializer::assignCommon(value, cycle);

		if (value.HasMember("from") && value["from"].IsFloat())
		{
			const auto from = value["from"].GetFloat();
			coreCycle->setFrom(from);
		}

		if (value.HasMember("to") && value["to"].IsFloat())
		{
			const auto to = value["to"].GetFloat();
			coreCycle->setTo(to);
		}

		if (value.HasMember("manualStep") && value["manualStep"].IsFloat())
		{
			const auto step = value["manualStep"].GetFloat();
			coreCycle->setManualStep(step);
		}

		if (value.HasMember("step") && value["step"].IsFloat())
		{
			const auto multiplier = value["step"].GetFloat();
			coreCycle->setStep(multiplier);
		}

		if (value.HasMember("stepDuration") && value["stepDuration"].IsFloat())
		{
			const auto duration = value["stepDuration"].GetFloat();
			coreCycle->setStepDuration(duration);
		}

		if (value.HasMember("smooth") && value["smooth"].IsBool())
		{
			const auto smooth = value["smooth"].GetBool();
			coreCycle->setSmoothStep(smooth);
		}

		if (value.HasMember("isRunning") && value["isRunning"].IsBool())
		{
			const auto isRunning = value["isRunning"].GetBool();
			if (isRunning)
			{
				coreCycle->play();
			}
		}

		if (value.HasMember("mode") && value["mode"].IsInt())
		{
			const auto mode = value["mode"].GetInt();
			coreCycle->setMode((Core::Cycle::EMode) mode);
		}
	}

	//

	for (auto& value : memento["workspace"]["cameras"].GetArray())
	{
		const auto camera = Workspace::addNodeToNodeEditorNoSave<Workspace::Camera>();
		NodeDeserializer::assignCommon(value, camera);
		createdNodes[value["id"].GetInt()] = camera;

		const auto& viewValue = value["sequences"].GetArray()[0];
		NodeDeserializer::assignSequence(viewValue, camera->getView());

		const auto& projValue = value["sequences"].GetArray()[1];
		NodeDeserializer::assignSequence(projValue, camera->getProjection());
	}

	//

	for (auto& value : memento["workspace"]["screens"].GetArray())
	{
		const auto screen = Workspace::addNodeToNodeEditorNoSave<Workspace::Screen>();
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
		const auto model = Workspace::addNodeToNodeEditorNoSave<Workspace::Model>();
		NodeDeserializer::assignCommon(value, model);
		createdNodes[value["id"].GetInt()] = model;

		auto mesh = model->viewportModel().lock();

		if (value.HasMember("model") && value["model"].IsString())
		{
			const auto* alias = value["model"].GetString();
			mesh->setModel(alias);
		}
		else
		{
			LOG_WARN("Model node {} has no model set, using the default one.", model->getNodebase()->getId());
		}

		if (value.HasMember("visible") && value["visible"].IsBool())
		{
			mesh->m_visible = value["visible"].GetBool();
		}

		if (value.HasMember("showAxes") && value["showAxes"].IsBool())
		{
			mesh->m_showAxes = value["showAxes"].GetBool();
		}

		if (value.HasMember("opaque") && value["opaque"].IsBool())
		{
			mesh->m_opaque = value["opaque"].GetBool();
		}

		if (value.HasMember("opacity") && value["opacity"].IsFloat())
		{
			mesh->m_opacity = value["opacity"].GetFloat();
		}

		if (value.HasMember("tint"))
		{
			mesh->m_tint = JSON::getVec3(value["tint"].GetArray());
		}

		if (value.HasMember("tintStrength") && value["tintStrength"].IsFloat())
		{
			mesh->m_tintStrength = value["tintStrength"].GetFloat();
		}
	}

	if (memento["workspace"].HasMember("scriptingNodes"))
	{
		for (auto& value : memento["workspace"]["scriptingNodes"].GetArray())
		{
			const auto node = Workspace::addNodeToNodeEditorNoSave<Workspace::ScriptingNode>();
			NodeDeserializer::assignCommon(value, node);

			/// \todo Assign script after the node is connected with the other nodes.
			if (value["script"].IsString())
			{
				if (auto newNode = node->setScript(value["script"].GetString()))
				{
					// Has to be here, node id gets changed by setScript.
					createdNodes[value["id"].GetInt()] = newNode;
				}
			}
		}
	}

	//

	const auto& transforms = memento["workspace"]["transforms"];
	for (auto& value : transforms.GetArray())
	{
		const auto maybeTransform = NodeDeserializer::createTransform(value);
		if (maybeTransform)
		{
			const auto transform = maybeTransform.value();
			NodeDeserializer::assignCommon(value, transform);
			createdNodes[value["id"].GetInt()] = transform;
		}
	}

	// connect edges

	const auto& edges = memento["workspace"]["edges"];
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

	std::vector<Ptr<DIWNE::Node>> result;
	std::transform(createdNodes.begin(), createdNodes.end(), std::back_inserter(result), [](auto& pair) {
		return pair.second;
	});
	return result;
}

std::optional<Ptr<GuiOperator>> createOperator(const rapidjson::Value& value)
{
	const auto& type = value["type"].GetString();

	Ptr<GuiOperator> node;

	// Workaround for #311
	if (type == n(Core::EOperatorType::AngleAxisToQuat))
	{
		auto result = Workspace::addNodeToNodeEditorNoSave<Workspace::AngleAxisToQuatOperator>();
		if (value.HasMember("halfAngle"))
		{
			const auto halfAngle = value["halfAngle"].GetBool();
			result->m_halfAngle = halfAngle;
		}
		node = result;
	}
	else
	{
		auto maybeNode = g_OperatorBuilder(type);
		if (!maybeNode)
		{
			LOG_ERROR("Unable to create operator node with id {} of type {}.", value["id"].GetString(), type);
			return std::nullopt;
		}
		node = maybeNode.value();
	}

	const auto coreNode = node->getNodebase();

	assignCommon(value, node);

	if (value.HasMember("value") && coreNode->getOperation().isConstructor)
	{
		const auto valueType = coreNode->getOperation().inputTypes[0];
		if (auto maybeData = JSON::getData(value["value"], valueType))
		{
			const auto& data = *maybeData;
			coreNode->dataMut(0) = data;
		}
		if (value.HasMember("savedValue"))
		{
			if (auto maybeData = JSON::getData(value["savedValue"], valueType))
			{
				coreNode->dataMut(0).setSavedValue((*maybeData).data());
			}
		}
	}
	return node;
}

Ptr<GuiSequence> createSequence(const rapidjson::Value& value)
{
	auto sequence = Workspace::addNodeToNodeEditorNoSave<Workspace::Sequence>();

	assignCommon(value, sequence);
	assignSequence(value, sequence);
	return sequence;
}

std::optional<Ptr<GuiTransform>> createTransform(const rapidjson::Value& value)
{
	I3T_ASSERT(value.IsObject(), "Invalid value type");

	const auto& type = value["type"].GetString();

	const auto maybeNode = g_TransformBuilder(type);
	if (!maybeNode)
	{
		LOG_ERROR("Unable to create transform node with id {} of type {}.", value["id"].GetString(), type);
		return std::nullopt;
	}
	auto node = *maybeNode;

	const auto coreNode = node->getNodebase()->as<Core::Transform>();

	assignCommon(value, node);

	value["synergies"].GetBool() ? coreNode->enableSynergies() : coreNode->disableSynergies();

	const auto maybeLOD = EnumUtils::value<Workspace::LevelOfDetail>(std::string(value["LOD"].GetString()));
	if (maybeLOD.has_value())
	{
		node->setLevelOfDetail(maybeLOD.value());
	}

	if (value.HasMember("savedValue"))
	{
		coreNode->dataMut(0).setSavedValue(JSON::getMat(value["savedValue"]));
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

	// must be here, because of the default values
	value["locked"].GetBool() ? coreNode->lock() : coreNode->unlock();

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
		node->setRendered(value["render"].GetBool());
	}

	if (value.HasMember("numberOfDecimals"))
	{
		node->setNumberOfVisibleDecimal(value["numberOfDecimals"].GetInt());
	}

	if (value.HasMember("LOD"))
	{
		const std::string LODName = value["LOD"].GetString();
		node->setLevelOfDetail(
		    EnumUtils::value<Workspace::LevelOfDetail>(LODName).value_or(Workspace::LevelOfDetail::Full));
	}

	if (value.HasMember("selected"))
	{
		node->setSelected(value["selected"].GetBool());
	}

	const auto position = JSON::getVec2(value["position"].GetArray());
	node->setPosition(position);
}

void assignSequence(const rapidjson::Value& value, Ptr<GuiSequence> sequence)
{
	std::vector<Ptr<Workspace::TransformationBase>> transforms;
	for (const auto& transform : value["transforms"].GetArray())
	{
		auto maybeTransform = createTransform(transform);
		if (!maybeTransform)
		{
			LOG_ERROR("Unable to create transform node with id {}.", transform["id"].GetInt());
			continue;
		}
		transforms.push_back(*maybeTransform);
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
