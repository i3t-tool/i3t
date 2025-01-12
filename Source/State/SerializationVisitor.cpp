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
#include "SerializationVisitor.h"

#include "GUI/Workspace/Nodes/Basic/CoreNodeWithPins.h"
#include "GUI/Workspace/Nodes/Camera.h"
#include "GUI/Workspace/Nodes/Cycle.h"
#include "GUI/Workspace/Nodes/Model.h"
#include "GUI/Workspace/Nodes/Operator.h"
#include "GUI/Workspace/Nodes/Screen.h"
#include "GUI/Workspace/Nodes/Sequence.h"
#include "GUI/Workspace/Nodes/TransformationBase.h"
#include "Logger/Logger.h"
#include "Utils/JSON.h"
#include "Viewport/entity/nodes/SceneModel.h"

using namespace rapidjson;

void SerializationVisitor::dump(const std::vector<Ptr<GuiNode>>& nodes)
{
	m_memento.SetObject();

	rapidjson::Value workspace(rapidjson::kObjectType);

	rapidjson::Value operators(rapidjson::kArrayType);
	workspace.AddMember("operators", std::move(operators), m_memento.GetAllocator());

	rapidjson::Value sequences(rapidjson::kArrayType);
	workspace.AddMember("sequences", std::move(sequences), m_memento.GetAllocator());

	rapidjson::Value cycles(rapidjson::kArrayType);
	workspace.AddMember("cycles", std::move(cycles), m_memento.GetAllocator());

	rapidjson::Value cameras(rapidjson::kArrayType);
	workspace.AddMember("cameras", std::move(cameras), m_memento.GetAllocator());

	rapidjson::Value screens(rapidjson::kArrayType);
	workspace.AddMember("screens", std::move(screens), m_memento.GetAllocator());

	rapidjson::Value models(rapidjson::kArrayType);
	workspace.AddMember("models", std::move(models), m_memento.GetAllocator());

	rapidjson::Value transforms(rapidjson::kArrayType);
	workspace.AddMember("transforms", std::move(transforms), m_memento.GetAllocator());

	rapidjson::Value edges(rapidjson::kArrayType);
	workspace.AddMember("edges", std::move(edges), m_memento.GetAllocator());

	m_memento.AddMember("workspace", std::move(workspace), m_memento.GetAllocator());

	for (const auto& node : nodes)
	{
		node->accept(*this);
	}
}

void SerializationVisitor::visit(const Ptr<GuiCamera>& node)
{
	const auto& coreNode = node->getNodebase()->as<Core::Camera>();
	auto& alloc = m_memento.GetAllocator();
	auto& cameras = m_memento["workspace"]["cameras"];
	auto& edges = m_memento["workspace"]["edges"];

	rapidjson::Value camera(rapidjson::kObjectType);
	dumpCommon(camera, node);

	rapidjson::Value sequences(kArrayType);
	dumpSequence(sequences, node->getView());
	dumpSequence(sequences, node->getProjection());
	camera.AddMember("sequences", sequences.Move(), alloc);

	cameras.PushBack(camera, alloc);

	addEdges(edges, coreNode);
}

void SerializationVisitor::visit(const Ptr<GuiCycle>& node)
{
	const auto& coreNode = node->getNodebase()->as<Core::Cycle>();
	auto& alloc = m_memento.GetAllocator();
	auto& cycles = m_memento["workspace"]["cycles"];
	auto& edges = m_memento["workspace"]["edges"];

	rapidjson::Value cycle(rapidjson::kObjectType);
	dumpCommon(cycle, node);

	cycle.AddMember("from", coreNode->getFrom(), alloc);
	cycle.AddMember("to", coreNode->getTo(), alloc);
	cycle.AddMember("manualStep", coreNode->getManualStep(), alloc);
	cycle.AddMember("step", coreNode->getStep(), alloc);
	cycle.AddMember("stepDuration", coreNode->getStepDuration(), alloc);
	cycle.AddMember("smooth", coreNode->getSmoothStep(), alloc);
	cycle.AddMember("isRunning", coreNode->isRunning(), alloc);
	cycle.AddMember("mode", (int) coreNode->getMode(), alloc);

	cycles.PushBack(cycle, alloc);

	addEdges(edges, coreNode);
}

void SerializationVisitor::visit(const Ptr<GuiOperator>& node)
{
	const auto& coreNode = node->getNodebase();
	auto& alloc = m_memento.GetAllocator();
	auto& operators = m_memento["workspace"]["operators"];
	auto& edges = m_memento["workspace"]["edges"];

	const auto* props = coreNode->getOperation();

	rapidjson::Value op(rapidjson::kObjectType);
	dumpCommon(op, node);

	op.AddMember("type", rapidjson::Value(props->keyWord.c_str(), alloc).Move(), alloc);

	if (props->isConstructor)
	{
		auto data = coreNode->data(0);
		addData(op, "value", data);
		if (data.hasSavedValue())
		{
			addData(op, "savedValue", Core::Data(data.getSavedValue().value(), data.valueType));
		}
	}

	// Workaround for #311
	if (auto quatAngleAxis = std::dynamic_pointer_cast<Workspace::AngleAxisToQuatOperator>(node))
	{
		op.AddMember("halfAngle", quatAngleAxis->m_halfAngle, alloc);
	}

	operators.PushBack(op, alloc);

	addEdges(edges, coreNode);
}

void SerializationVisitor::visit(const Ptr<GuiSequence>& node)
{
	dumpSequence(m_memento["workspace"]["sequences"], node);
}

void SerializationVisitor::visit(const Ptr<GuiTransform>& node)
{
	dumpTransform(m_memento["workspace"]["transforms"].GetArray(), node);
}

void SerializationVisitor::visit(const Ptr<GuiScreen>& node)
{
	const auto& coreNode = node->getNodebase()->as<Core::Operator<Core::EOperatorType::Screen>>();
	auto& alloc = m_memento.GetAllocator();
	auto& screens = m_memento["workspace"]["screens"];
	auto& edges = m_memento["workspace"]["edges"];

	rapidjson::Value screen(rapidjson::kObjectType);
	dumpCommon(screen, node);

	// screen.AddMember("aspect", node->, alloc);
	JSON::addVector(screen, "aspect", node->getAspect(), m_memento.GetAllocator());

	screens.PushBack(screen, alloc);

	addEdges(edges, coreNode);
}

void SerializationVisitor::visit(const Ptr<GuiModel>& node)
{
	const auto& coreNode = node->getNodebase()->as<Core::Model>();
	auto& alloc = m_memento.GetAllocator();
	auto& models = m_memento["workspace"]["models"];
	auto& edges = m_memento["workspace"]["edges"];

	rapidjson::Value model(rapidjson::kObjectType);
	dumpCommon(model, node);

	const auto mesh = node->viewportModel().lock();
	const auto modelAlias = mesh->getModel();
	model.AddMember("model", rapidjson::Value(modelAlias.c_str(), alloc), alloc);

	model.AddMember("visible", mesh->m_visible, alloc);
	model.AddMember("showAxes", mesh->m_showAxes, alloc);
	model.AddMember("opaque", mesh->m_opaque, alloc);
	model.AddMember("opacity", mesh->m_opacity, alloc);
	JSON::addVector(model, "tint", mesh->m_tint, m_memento.GetAllocator());
	model.AddMember("tintStrength", mesh->m_tintStrength, alloc);

	models.PushBack(model, alloc);

	addEdges(edges, coreNode);
}

//

void SerializationVisitor::dumpCommon(rapidjson::Value& target, const Ptr<GuiNode>& node)
{
	I3T_ASSERT(target.IsObject(), "Invalid value type");

	const auto& coreNode = node->getNodebase();
	auto& alloc = m_memento.GetAllocator();

	target.AddMember("id", coreNode->getId(), alloc);
	if (node->getTopLabel() != node->getNodebase()->getLabel())
	{
		target.AddMember("label", node->getTopLabel(), alloc);
	}
	target.AddMember("render", node->isRendered(), alloc);
	target.AddMember("numberOfDecimals", node->getNumberOfVisibleDecimal(), alloc);
	target.AddMember("LOD", EnumUtils::name(node->getLevelOfDetail()), alloc);

	JSON::addVector(target, "position", node->getNodePositionDiwne(), m_memento.GetAllocator());
}

void SerializationVisitor::dumpSequence(rapidjson::Value& target, const Ptr<GuiSequence>& node)
{
	I3T_ASSERT(target.IsArray(), "Invalid value type");

	const auto& coreNode = node->getNodebase()->as<Core::Sequence>();
	auto& alloc = m_memento.GetAllocator();
	auto& sequences = target;
	auto& edges = m_memento["workspace"]["edges"];

	rapidjson::Value sequence(rapidjson::kObjectType);
	dumpCommon(sequence, node);

	sequence.AddMember("transforms", rapidjson::Value(kArrayType), alloc);

	for (const auto& transform : node->getInnerWorkspaceNodes())
	{
		dumpTransform(sequence["transforms"].GetArray(),
		              std::static_pointer_cast<Workspace::TransformationBase>(transform));
	}

	sequences.PushBack(sequence, alloc);

	addEdges(edges, coreNode);
}

void SerializationVisitor::dumpTransform(rapidjson::Value& target, const Ptr<GuiTransform>& node)
{
	I3T_ASSERT(target.IsArray(), "Invalid value type");

	const auto& coreNode = node->getNodebase()->as<Core::Transform>();
	auto& alloc = m_memento.GetAllocator();

	const auto* props = coreNode->getOperation();

	rapidjson::Value transform(rapidjson::kObjectType);
	dumpCommon(transform, node);

	transform.AddMember("type", rapidjson::Value(props->keyWord.c_str(), alloc).Move(), alloc);

	if (!coreNode->getDefaultValues().empty())
	{
		transform.AddMember("defaultValues", rapidjson::Value(rapidjson::kObjectType), alloc);
		transform.AddMember("savedDefaults", rapidjson::Value(rapidjson::kObjectType), alloc);
	}

	for (const auto& [key, value] : coreNode->getDefaultValues())
	{
		addData(transform["defaultValues"], key.c_str(), value);
	}

	//

	JSON::addMatrix(transform, "value", coreNode->data().getMat4(), m_memento.GetAllocator());

	auto maybeSavedValue = coreNode->data(0).getSavedValue();
	if (maybeSavedValue)
	{
		addData(transform, "savedValue", Core::Data(*maybeSavedValue, Core::EValueType::Matrix));

		/*
		for (const auto& [key, value] : coreNode->getDefaultValues())
		{
		  addData(transform["savedDefaults"], key.c_str(), value);
		}
		 */
	}

	//

	JSON::addBool(transform, "synergies", coreNode->hasSynergies(), m_memento.GetAllocator());
	JSON::addBool(transform, "locked", coreNode->isLocked(), m_memento.GetAllocator());

	target.PushBack(transform, alloc);
}

//

void SerializationVisitor::addData(rapidjson::Value& target, const char* key, const Core::Data& data)
{
	using namespace Core;

	I3T_ASSERT(target.IsObject(), "Invalid value type");

	auto& alloc = m_memento.GetAllocator();

	switch (data.valueType)
	{
	case EValueType::Float:
		target.AddMember(rapidjson::Value(key, alloc).Move(), rapidjson::Value(data.getFloat()), alloc);
		break;
	case EValueType::Vec3:
		JSON::addVector(target, key, data.getVec3(), m_memento.GetAllocator());
		break;
	case EValueType::Vec4:
		JSON::addVector(target, key, data.getVec4(), m_memento.GetAllocator());
		break;
	case EValueType::Matrix:
		JSON::addMatrix(target, key, data.getMat4(), m_memento.GetAllocator());
		break;
	case EValueType::Quat:
	{
		const auto& q = data.getQuat();
		const glm::vec4 vec = {q.x, q.y, q.z, q.w};
		JSON::addVector(target, key, vec, m_memento.GetAllocator());
		break;
	}
	case EValueType::Pulse:
	case EValueType::MatrixMul:
	case EValueType::Screen:
	case EValueType::Ptr:
		break;
	}
}

void SerializationVisitor::addEdges(rapidjson::Value& target, const Ptr<Core::Node>& node)
{
	I3T_ASSERT(target.IsArray(), "Invalid value type");

	for (const auto& input : node->getInputPins())
	{
		if (!input.isPluggedIn())
		{
			continue;
		}

		auto parent = input.getParentPin()->getOwner();

		const auto fromId = parent->getId();
		const auto fromPin = input.getParentPin()->Index;
		const auto toId = node->getId();
		const auto toPin = input.Index;

		rapidjson::Value e(kArrayType);

		auto& alloc = m_memento.GetAllocator();

		e.PushBack(fromId, alloc);
		e.PushBack(fromPin, alloc);
		e.PushBack(toId, alloc);
		e.PushBack(toPin, alloc);

		target.PushBack(e.Move(), alloc);
	}
}
