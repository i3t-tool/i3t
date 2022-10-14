#include "SerializationVisitor.h"

#include "GUI/Elements/Nodes/WorkspaceCamera.h"
#include "GUI/Elements/Nodes/WorkspaceCycle.h"
#include "GUI/Elements/Nodes/WorkspaceElementsWithCoreData.h"
#include "GUI/Elements/Nodes/WorkspaceModel.h"
#include "GUI/Elements/Nodes/WorkspaceScreen.h"
#include "GUI/Elements/Nodes/WorkspaceSequence.h"
#include "GUI/Elements/Nodes/WorkspaceTransformation.h"

using namespace rapidjson;

void SerializationVisitor::dump(const std::vector<Ptr<GuiNode>>& nodes)
{
	m_memento.SetObject();

	rapidjson::Value workspace(rapidjson::kObjectType);

	rapidjson::Value operators(rapidjson::kArrayType);
	workspace.AddMember("operators", std::move(operators),
	                    m_memento.GetAllocator());

	rapidjson::Value sequences(rapidjson::kArrayType);
	workspace.AddMember("sequences", std::move(sequences),
	                    m_memento.GetAllocator());

	rapidjson::Value cycles(rapidjson::kArrayType);
	workspace.AddMember("cycles", std::move(cycles), m_memento.GetAllocator());

	rapidjson::Value cameras(rapidjson::kArrayType);
	workspace.AddMember("cameras", std::move(cameras), m_memento.GetAllocator());

	rapidjson::Value screens(rapidjson::kArrayType);
	workspace.AddMember("screens", std::move(screens), m_memento.GetAllocator());

	rapidjson::Value models(rapidjson::kArrayType);
	workspace.AddMember("models", std::move(models), m_memento.GetAllocator());

	rapidjson::Value transforms(rapidjson::kArrayType);
	workspace.AddMember("transforms", std::move(transforms),
	                    m_memento.GetAllocator());

	rapidjson::Value edges(rapidjson::kArrayType);
	workspace.AddMember("edges", std::move(edges), m_memento.GetAllocator());

	m_memento.AddMember("workspace", std::move(workspace),
	                    m_memento.GetAllocator());

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

	op.AddMember("type", rapidjson::Value(props->keyWord.c_str(), alloc).Move(),
	             alloc);

	if (props->isConstructor)
	{
		addData(op, "value", coreNode->getData());
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
	const auto& coreNode = node->getNodebase()->as<Core::Sequence>();
	auto& alloc = m_memento.GetAllocator();
	auto& screens = m_memento["workspace"]["screens"];
	auto& edges = m_memento["workspace"]["edges"];

	rapidjson::Value screen(rapidjson::kObjectType);
	dumpCommon(screen, node);

	/// \todo MH Save aspect

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

	/// \todo MH Save model reference!

	models.PushBack(model, alloc);

	addEdges(edges, coreNode);
}

//

void SerializationVisitor::dumpCommon(rapidjson::Value& target,
                                      const Ptr<GuiNode>& node)
{
	I3T_ASSERT(target.IsObject());

	const auto& coreNode = node->getNodebase();
	auto& alloc = m_memento.GetAllocator();

	target.AddMember("id", coreNode->getId(), alloc);
	addVector(target, "position", node->getNodePositionDiwne());
}

void SerializationVisitor::dumpSequence(rapidjson::Value& target,
                                        const Ptr<GuiSequence>& node)
{
	I3T_ASSERT(target.IsArray());

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
		              std::static_pointer_cast<WorkspaceTransformation>(transform));
	}

	sequences.PushBack(sequence, alloc);

	addEdges(edges, coreNode);
}

void SerializationVisitor::dumpTransform(rapidjson::Value& target,
                                         const Ptr<GuiTransform>& node)
{
	I3T_ASSERT(target.IsArray() && "Test your code properly!");

	const auto& coreNode = node->getNodebase()->as<Core::Transformation>();
	auto& alloc = m_memento.GetAllocator();

	const auto* props = coreNode->getOperation();

	rapidjson::Value transform(rapidjson::kObjectType);
	dumpCommon(transform, node);

	transform.AddMember(
	    "type", rapidjson::Value(props->keyWord.c_str(), alloc).Move(), alloc);

	//

	if (!coreNode->getDefaultValues().empty())
	{
		transform.AddMember("defaultValues",
		                    rapidjson::Value(rapidjson::kObjectType), alloc);
		transform.AddMember("savedDefaults",
		                    rapidjson::Value(rapidjson::kObjectType), alloc);
	}

	for (const auto& [key, value] : coreNode->getDefaultValues())
	{
		addData(transform["defaultValues"], key.c_str(), value);
	}

	//

	addMatrix(transform, "value", coreNode->getData().getMat4());

	if (coreNode->hasSavedValue())
	{
		addMatrix(transform, "savedValue", coreNode->getSavedValue());

		/*
		for (const auto& [key, value] : coreNode->getDefaultValues())
		{
		  addData(transform["savedDefaults"], key.c_str(), value);
		}
		 */
	}

	//

	addBool(transform, "synergies", coreNode->hasSynergies());
	addBool(transform, "locked", coreNode->isLocked());

	addString(transform, "LOD", EnumUtils::name(node->getLevelOfDetail()));

	target.PushBack(transform, alloc);
}

//

void SerializationVisitor::addBool(rapidjson::Value& target, const char* key,
                                   bool value)
{
	I3T_ASSERT(target.IsObject() && "Test your code properly!");

	auto& alloc = m_memento.GetAllocator();

	target.AddMember(rapidjson::Value(key, alloc).Move(), rapidjson::Value(value),
	                 alloc);
}

void SerializationVisitor::addString(rapidjson::Value& target, const char* key,
                                     const std::string& value)
{
	I3T_ASSERT(target.IsObject() && "Test your code properly!");

	auto& alloc = m_memento.GetAllocator();

	target.AddMember(rapidjson::Value(key, alloc).Move(),
	                 rapidjson::Value(value.c_str(), alloc).Move(), alloc);
}

void SerializationVisitor::addVector(rapidjson::Value& target, const char* key,
                                     const ImVec2& vec)
{
	I3T_ASSERT(target.IsObject() && "Test your code properly!");

	auto& alloc = m_memento.GetAllocator();

	rapidjson::Value vector(kArrayType);
	vector.PushBack(vec.x, alloc);
	vector.PushBack(vec.y, alloc);

	target.AddMember(rapidjson::Value(key, alloc).Move(), std::move(vector),
	                 alloc);
}

void SerializationVisitor::addVector(rapidjson::Value& target, const char* key,
                                     const glm::vec3& vec)
{
	I3T_ASSERT(target.IsObject() && "Test your code properly!");

	auto& alloc = m_memento.GetAllocator();

	rapidjson::Value vector(kArrayType);
	vector.PushBack(vec.x, alloc);
	vector.PushBack(vec.y, alloc);
	vector.PushBack(vec.z, alloc);

	target.AddMember(rapidjson::Value(key, alloc).Move(), std::move(vector),
	                 alloc);
}

void SerializationVisitor::addVector(rapidjson::Value& target, const char* key,
                                     const glm::vec4& vec)
{
	I3T_ASSERT(target.IsObject() && "Test your code properly!");

	auto& alloc = m_memento.GetAllocator();

	rapidjson::Value vector(kArrayType);
	vector.PushBack(vec.x, alloc);
	vector.PushBack(vec.y, alloc);
	vector.PushBack(vec.z, alloc);
	vector.PushBack(vec.w, alloc);

	target.AddMember(rapidjson::Value(key, alloc).Move(), std::move(vector),
	                 alloc);
}

void SerializationVisitor::addMatrix(rapidjson::Value& target, const char* key,
                                     const glm::mat4& mat)
{
	I3T_ASSERT(target.IsObject() && "Test your code properly!");

	auto& alloc = m_memento.GetAllocator();

	rapidjson::Value matrix(kArrayType);

	for (int i = 0; i < 4; ++i)
	{
		const auto& vec = mat[i];

		rapidjson::Value column(kArrayType);
		column.PushBack(vec.x, alloc);
		column.PushBack(vec.y, alloc);
		column.PushBack(vec.z, alloc);
		column.PushBack(vec.w, alloc);

		matrix.PushBack(column.Move(), alloc);
	}

	target.AddMember(rapidjson::Value(key, alloc).Move(), std::move(matrix),
	                 alloc);
}

void SerializationVisitor::addData(rapidjson::Value& target, const char* key,
                                   const DataStore& data)
{
	I3T_ASSERT(target.IsObject() && "Test your code properly!");

	auto& alloc = m_memento.GetAllocator();

	switch (data.opValueType)
	{
	case EValueType::Float:
		target.AddMember(rapidjson::Value(key, alloc).Move(),
		                 rapidjson::Value(data.getFloat()), alloc);
		break;
	case EValueType::Vec3:
		addVector(target, key, data.getVec3());
		break;
	case EValueType::Vec4:
		addVector(target, key, data.getVec4());
		break;
	case EValueType::Matrix:
		addMatrix(target, key, data.getMat4());
		break;
	case EValueType::Quat:
	{
		const auto& q = data.getQuat();
		const glm::vec4 vec = {q.x, q.y, q.z, q.w};
		addVector(target, key, vec);
		break;
	}
	case EValueType::Pulse:
	case EValueType::MatrixMul:
	case EValueType::Screen:
	case EValueType::Ptr:
		break;
	}
}

void SerializationVisitor::addEdges(rapidjson::Value& target,
                                    const Ptr<Core::Node>& node)
{
	for (const auto& input : node->getInputPins())
	{
		if (input.isPluggedIn())
		{
			const int fromId = input.getParentPin()->getOwner()->getId();
			const int fromPin = input.getParentPin()->getIndex();
			const int toId = node->getId();
			const int toPin = input.getIndex();

			rapidjson::Value e(kArrayType);

			I3T_ASSERT(target.IsArray() && "Test your code properly!");

			auto& alloc = m_memento.GetAllocator();

			e.PushBack(fromId, alloc);
			e.PushBack(fromPin, alloc);
			e.PushBack(toId, alloc);
			e.PushBack(toPin, alloc);

			target.PushBack(e.Move(), alloc);
		}
	}
}
