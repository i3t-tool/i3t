/**
 * \file
 * \brief
 * \author Martin Herich <martin.herich@phire.cz>
 * \copyright Copyright (C) 2016-2024 I3T team, Department of Computer Graphics
 * and Interaction, FEE, Czech Technical University in Prague, Czech Republic
 *
 * This file is part of I3T - An Interactive Tool for Teaching Transformations
 * http://www.i3t-tool.org
 *
 * GNU General Public License v3.0 (see LICENSE.txt or https://www.gnu.org/licenses/gpl-3.0.txt)
 */
#include "Scripting/Environment.h"

#include "GUI/Elements/Windows/WorkspaceWindow.h"
#include "GUI/Workspace/Builder.h"
#include "GUI/Workspace/Nodes/Basic/CoreNode.h"
#include "GUI/Workspace/Nodes/ScriptingNode.h"
#include "Scripting/Utils.h"
#include "Viewport/entity/nodes/SceneModel.h"

static Workspace::OperatorBuilder g_OperatorBuilder;
static Workspace::TransformBuilder g_TransformBuilder;

static Workspace::WorkspaceDiwne& getNodeEditor()
{
	const auto workspace = I3T::getUI()->getWindowManager().getWindowPtr<WorkspaceWindow>();
	return workspace->getNodeEditor();
}

//----------------------------------------------------------------------------//

template <typename R, typename T>
static std::optional<T> getValue(Ptr<R> guiNode, int luaIndex)
{
	const auto node = guiNode->getNodebase();

	const auto index = luaIndex - 1;

	constexpr auto isTransform = std::is_same_v<R, GuiTransform>;

	if (index >= node->getOutputPins().size() && index == 0 && !isTransform)
	{
		print("no such index");
		return std::nullopt;
	}

	const auto maybeValue = node->data(index).template getValue<T>();
	if (!maybeValue.has_value())
	{
		print("no such type of value");
		return std::nullopt;
	}

	return maybeValue;
}

template <typename R, typename T>
static bool setValue(Ptr<R> guiNode, const T& value)
{
	const auto node = guiNode->getNodebase();

	constexpr auto isTransform = std::is_same_v<R, GuiTransform>;

	if (!node->getOperation()->isConstructor && !isTransform)
	{
		print("cannot set value for this node");
		return false;
	}

	if (node->data().index() != variant_index<Core::Data::Storage, T>())
	{
		print("invalid value type for this node");
		return false;
	}

	const auto result = node->setValue(value);
	if (result.status != Core::SetValueResult::Status::Ok)
	{
		print(result.message);
		return false;
	}

	return true;
}

//----------------------------------------------------------------------------//

template <typename T>
static std::optional<T> getDefaultValue(Ptr<GuiTransform> guiNode, const std::string& name)
{
	const auto transform = guiNode->getNodebase()->as<Core::Transform>();
	const auto maybeValue = transform->getDefaultValue(name).getValue<T>();
	if (!maybeValue.has_value())
	{
		print("transform does not have value of given type");
	}

	return maybeValue;
}

template <typename T>
static bool setDefaultValue(Ptr<GuiTransform> guiNode, const std::string& name, const T& value)
{
	const auto node = guiNode->getNodebase()->as<Core::Transform>();
	const auto result = node->setDefaultValue(name, value);
	if (result.status != Core::SetValueResult::Status::Ok)
	{
		print(result.message);

		return false;
	}

	return true;
}

//----------------------------------------------------------------------------//

static auto getWorkspaceNodes()
{
	return getNodeEditor().m_workspaceCoreNodes;
}

static auto findNode(Core::ID id) -> Ptr<GuiNode>
{
	constexpr const bool searchInner = true;
	const auto result = getNodeEditor().getNode<GuiNode>(id, searchInner);

	if (!result)
	{
		print(result.error().str());

		return nullptr;
	}

	return result.value();
};

//----------------------------------------------------------------------------//

static bool plug(Ptr<GuiNode> self, int from, Ptr<GuiNode> other, int to)
{
	return connectNodesNoSave(self, other, from, to);
}

static bool unplugInput(Ptr<GuiNode> self, int inputIndex)
{
	if (auto node = std::dynamic_pointer_cast<Workspace::CoreNodeWithPins>(self))
	{
		if (node->getInputs().size() <= inputIndex)
		{
			print(fmt::format("No such input index {}", inputIndex));
			return false;
		}

		if (node->getInputs()[inputIndex]->isConnected())
		{
			node->getInputs()[inputIndex]->unplug();
		}

		return true;
	}

	return false;
}

static bool unplugOutput(Ptr<GuiNode> self, int outputIndex)
{
	if (auto node = std::dynamic_pointer_cast<Workspace::CoreNodeWithPins>(self))
	{
		if (node->getOutputs().size() <= outputIndex)
		{
			print(fmt::format("No such output index {}", outputIndex));
			return false;
		}

		const auto& nodes = getNodeEditor().m_workspaceCoreNodes;
		std::vector<std::pair<Ptr<Workspace::CoreNodeWithPins>, int>> toUnplug;
		for (const auto& outputPin : self->getNodebase()->getOutputPins())
		{
			for (const auto inputPin : outputPin.getOutComponents())
			{
				if (auto maybeNode = Workspace::Tools::findNodeById(nodes, inputPin->Owner.getId()))
				{
					auto nodeWithPin = std::dynamic_pointer_cast<Workspace::CoreNodeWithPins>(*maybeNode);
					toUnplug.push_back({nodeWithPin, inputPin->Index});
				}
			}
		}

		for (const auto& [childNode, inputIndex] : toUnplug)
		{
			childNode->getInputs()[inputIndex]->unplug();
		}

		return true;
	}

	return false;
}

//-- Workspace Management ----------------------------------------------------//


//----------------------------------------------------------------------------//

/*
SOL_BASE_CLASSES(GuiNode, GuiSequence);
SOL_DERIVED_CLASSES(GuiSequence, GuiNode);
 */

namespace LuaSerializer
{
std::string toConstructor(const ImVec2& vec)
{
	return fmt::format("Vec2.new({}, {})", vec.x, vec.y);
}

std::string toConstructor(const glm::vec3& vec)
{
	return fmt::format("Vec3.new({}, {}, {})", vec.x, vec.y, vec.z);
}

std::string toConstructor(const glm::vec4& vec)
{
	return fmt::format("Vec4.new({}, {}, {}, {})", vec.x, vec.y, vec.z, vec.w);
}

std::string toConstructor(const glm::quat& vec)
{
	return fmt::format("Vec4.new({}, {}, {}, {})", vec.x, vec.y, vec.z, vec.w);
}

std::string toConstructor(const glm::mat4 mat)
{
	std::string result = "Mat4.new(";
	for (int i = 0; i < 4; ++i)
	{
		result += toConstructor(mat[i]);
		if (i != 3)
		{
			result += ", ";
		}
	}
	result += ")";

	return result;
}

Result<std::string, Error> toConstructor(const Core::Data& data)
{
	switch (data.valueType)
	{
	case Core::EValueType::Pulse:
		return Err("Not implemented!");
	case Core::EValueType::Float:
		return std::to_string(data.getFloat());
	case Core::EValueType::Vec3:
		return toConstructor(data.getVec3());
	case Core::EValueType::Vec4:
		return toConstructor(data.getVec4());
	case Core::EValueType::Matrix:
		return toConstructor(data.getMat4());
	case Core::EValueType::Quat:
		return toConstructor(data.getQuat());
	case Core::EValueType::MatrixMul:
	case Core::EValueType::Screen:
	case Core::EValueType::Ptr:
		return Err("Not implemented!");
	}

	return Err("Invalid data type!");
}
}; // namespace LuaSerializer

class ToScriptVisitor : public NodeVisitor
{
public:
	void append(const std::string& str)
	{
		m_stream << str;
	}

	void appendConnections(const Ptr<Workspace::CoreNode>& node)
	{
		for (const auto& input : node->getNodebase()->getInputPins())
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

			m_connections.push_back({fromId, fromPin, toId, toPin});
		}
	}

	std::string getScript() const
	{
		auto script = m_stream.str();

		std::stringstream connectionsStream;

		connectionsStream << "\n";
		for (const auto& connection : m_connections)
		{
			connectionsStream << fmt::format("I3T.plug(node_{}:get_id(), {}, node_{}:get_id(), {})\n", connection.x,
			                                 connection.y, connection.z, connection.w);
		}

		return script + connectionsStream.str();
	}

private:
	void dumpTransforms(const Ptr<Workspace::Sequence>& sequence)
	{
		auto transforms = sequence->getInnerWorkspaceNodes();
		std::reverse(transforms.begin(), transforms.end());
		for (const auto& transform : transforms)
		{
			transform->accept(*this);
			m_stream << fmt::format("node_{}:push(node_{})\n", sequence->getId(), transform->getId());
		}
	}

	void dumpCommon(const Ptr<Workspace::CoreNode>& node)
	{
		appendConnections(node);

		if (node->getTopLabel() != node->getNodebase()->getLabel())
		{
			m_stream << fmt::format("node_{}:set_label(\"{}\")\n", node->getId(), node->getTopLabel());
		}
		m_stream << fmt::format("node_{}:set_render({})\n", node->getId(), node->getRender());
		m_stream << fmt::format("node_{}:set_number_of_decimals({})\n", node->getId(),
		                        node->getNumberOfVisibleDecimal());
		m_stream << fmt::format("node_{}:set_lod({})\n", node->getId(), (int) node->getLevelOfDetail());
		m_stream << fmt::format("node_{}:set_position({})\n", node->getId(),
		                        LuaSerializer::toConstructor(node->getNodePositionDiwne()));
	}

	void visit(const Ptr<GuiCamera>& node) override
	{
		auto coreNode = node->getNodebase()->as<Core::Camera>();

		m_stream << fmt::format("local node_{} = Camera.new()\n", node->getId());
		dumpCommon(node);

		auto projection = node->getProjection();
		m_stream << fmt::format("local node_{} = node_{}:get_projection()\n", projection->getId(), node->getId());
		dumpTransforms(node->getProjection());
		appendConnections(projection);

		auto view = node->getView();
		m_stream << fmt::format("local node_{} = node_{}:get_view()\n", view->getId(), node->getId());
		dumpTransforms(node->getView());
		appendConnections(view);
	}

	void visit(const Ptr<GuiCycle>& node) override
	{
		auto coreNode = node->getNodebase()->as<Core::Cycle>();

		m_stream << fmt::format("local node_{} = Cycle.new()\n", node->getId());
		dumpCommon(node);

		m_stream << fmt::format("node_{}:set_from({})\n", node->getId(), coreNode->getFrom());
		m_stream << fmt::format("node_{}:set_to({})\n", node->getId(), coreNode->getTo());
		m_stream << fmt::format("node_{}:set_manual_step({})\n", node->getId(), coreNode->getManualStep());
		m_stream << fmt::format("node_{}:set_step({})\n", node->getId(), coreNode->getStep());
		m_stream << fmt::format("node_{}:set_step_duration({})\n", node->getId(), coreNode->getStepDuration());
		m_stream << fmt::format("node_{}:set_smooth({})\n", node->getId(), coreNode->getSmoothStep());
		m_stream << fmt::format("node_{}:set_mode({})\n", node->getId(), (int) coreNode->getMode());

		if (coreNode->isRunning())
		{
			m_stream << fmt::format("node_{}:play()\n", node->getId());
		}
	}

	void visit(const Ptr<GuiOperator>& node) override
	{
		auto coreNode = node->getNodebase();
		auto props = coreNode->getOperation();

		m_stream << fmt::format("local node_{} = Operator.new(\"{}\")\n", node->getId(), props->keyWord);
		dumpCommon(node);

		if (props->isConstructor)
		{
			auto data = coreNode->data(0);
			if (auto dataConstructor = LuaSerializer::toConstructor(data))
			{
				m_stream << fmt::format("node_{}:set_value({})\n", node->getId(), dataConstructor.value());
			}
			else
			{
				LOG_ERROR("Cannot serialize data to constructor on node {}", coreNode->getSignature());
			}
		}
	}

	void visit(const Ptr<GuiSequence>& node) override
	{
		auto coreNode = node->getNodebase()->as<Core::Sequence>();

		m_stream << fmt::format("local node_{} = Sequence.new()\n", node->getId());
		dumpCommon(node);
		dumpTransforms(node);
	}

	void visit(const Ptr<GuiTransform>& node) override
	{
		auto coreNode = node->getNodebase()->as<Core::Transform>();

		m_stream << fmt::format("local node_{} = Transform.new(\"{}\")\n", node->getId(),
		                        coreNode->getOperation()->keyWord);
		dumpCommon(node);

		m_stream << fmt::format("node_{}:set_value({})\n", node->getId(),
		                        LuaSerializer::toConstructor(coreNode->data().getMat4()));

		if (coreNode->hasSynergies())
		{
			m_stream << fmt::format("node_{}:enable_synergies()\n", node->getId());
		}
		else
		{
			m_stream << fmt::format("node_{}:disable_synergies()\n", node->getId());
		}

		if (coreNode->isLocked())
		{
			m_stream << fmt::format("node_{}:lock()\n", node->getId());
		}
		else
		{
			m_stream << fmt::format("node_{}:unlock()\n", node->getId());
		}

		for (const auto& [key, data] : coreNode->getDefaultValues())
		{
			if (auto dataConstructor = LuaSerializer::toConstructor(data))
			{
				m_stream << fmt::format("node_{}:set_default_value(\"{}\", {})\n", node->getId(), key,
				                        dataConstructor.value());
			}
			else
			{
				LOG_ERROR("Cannot serialize data to constructor on node {}", coreNode->getSignature());
			}
		}
	}

	void visit(const Ptr<GuiScreen>& node) override
	{
		m_stream << fmt::format("local node_{} = Screen.new()\n", node->getId());
		dumpCommon(node);

		m_stream << fmt::format("node_{}:set_aspect({})\n", node->getId(),
		                        LuaSerializer::toConstructor(node->getAspect()));
	}

	void visit(const Ptr<GuiModel>& node) override
	{
		auto coreNode = node->getNodebase();

		m_stream << fmt::format("local node_{} = Model.new()\n", node->getId());
		dumpCommon(node);

		auto model = node->viewportModel().lock();

		m_stream << fmt::format("node_{}:set_model(\"{}\")\n", node->getId(), model->getModel());
		m_stream << fmt::format("node_{}:set_visible({})\n", node->getId(), model->m_visible);
		m_stream << fmt::format("node_{}:show_axes({})\n", node->getId(), model->m_showAxes);
		m_stream << fmt::format("node_{}:set_opaque({})\n", node->getId(), model->m_opaque);
		m_stream << fmt::format("node_{}:set_opacity({})\n", node->getId(), model->m_opacity);
		m_stream << fmt::format("node_{}:set_tint({})\n", node->getId(), LuaSerializer::toConstructor(model->m_tint));
		m_stream << fmt::format("node_{}:set_tint_strength({})\n", node->getId(), model->m_tintStrength);
	}

	std::vector<glm::ivec4> m_connections;
	std::stringstream m_stream;
};

std::string to_script(const std::vector<Ptr<Workspace::CoreNode>>& nodes)
{
	ToScriptVisitor visitor;

	for (const auto& node : nodes)
	{
		node->accept(visitor);
	}

	return visitor.getScript();
}

template <typename T>
static Ptr<T> cast(Ptr<Workspace::CoreNode> element)
{
	return std::dynamic_pointer_cast<T>(element);
}

LUA_REGISTRATION
{
	// clang-format off
	L.new_usertype<GuiNode>(
	    "Node",
		sol::no_constructor,
		"get_id", &GuiNode::getId,
		"get_position", &GuiNode::getNodePositionDiwne,
		"set_position", &GuiNode::setNodePositionDiwne,
		"get_label", &GuiNode::getTopLabel,
		"set_label", &GuiNode::setTopLabel,
		"get_render", &GuiNode::getRender,
		"set_render", &GuiNode::setRender,
		"get_number_of_decimals", &GuiNode::getNumberOfVisibleDecimal,
		"set_number_of_decimals", &GuiNode::setNumberOfVisibleDecimal,
		"get_lod", &GuiNode::getLevelOfDetail,
		"set_lod", &GuiNode::setLevelOfDetail,
		"as_operator", &cast<GuiOperator>,
		"as_transform", &cast<GuiTransform>,
		"as_sequence", &cast<GuiSequence>,
		"as_camera", &cast<GuiCamera>,
		"as_cycle", &cast<GuiCycle>,
		"as_screen", &cast<GuiScreen>,
		"as_model", &cast<GuiModel>
	);

	L.new_usertype<GuiOperator>(
	    "Operator",
		sol::base_classes, sol::bases<GuiNode>(),
	    sol::meta_function::construct, [](const std::string& type) -> Ptr<GuiOperator> {
		    const auto maybeType = EnumUtils::value<Core::EOperatorType>(type);
		    if (!maybeType.has_value())
		    {
			    print(fmt::format("Unknown operator \"{}\".", type));

			    return nullptr;
		    }

		    const auto maybeOp = g_OperatorBuilder(type.c_str());
		    if (!maybeOp)
		    {
			    return nullptr;
		    }
		    const auto op = *maybeOp;
		    const auto ID = op->getNodebase()->getId();

		    print(fmt::format("ID: {}", ID));

		    return std::dynamic_pointer_cast<GuiOperator>(op);
	    },
		// getters
		"get_float", &getValue<GuiOperator, float>,
		"get_vec3", &getValue<GuiOperator, glm::vec3>,
		"get_vec4", &getValue<GuiOperator, glm::vec4>,
		"get_quat", &getValue<GuiOperator, glm::quat>,
		"get_mat4", &getValue<GuiOperator, glm::mat4>,
		// setters
		"set_value", sol::overload(
			&setValue<GuiOperator, float>,
			&setValue<GuiOperator, glm::vec3>,
			&setValue<GuiOperator, glm::vec4>
		)
	);

	L.new_usertype<GuiTransform>(
	    "Transform",
	    sol::base_classes, sol::bases<GuiNode>(),
	    "get_mat4", [](Ptr<GuiTransform> self) {
		    return getValue<GuiTransform, glm::mat4>(self, 1);
	    },
		// setters
		"set_value", sol::overload(
			&setValue<GuiTransform, glm::mat4>,
			[](Ptr<GuiTransform> self, float value, const ImVec2& coords) {
				const auto glmCoords = glm::vec2(coords.x, coords.y);
				const auto result = self->getNodebase()->setValue(value, glmCoords);
				if (result.status != Core::SetValueResult::Status::Ok)
				{
					print(result.message);
				}
			}
		),
		// get default value
		"get_default_float", &getDefaultValue<float>,
		"get_default_vec3", &getDefaultValue<glm::vec3>,
		"get_default_vec4", &getDefaultValue<glm::vec4>,
		"set_default_value", sol::overload(
			&setDefaultValue<float>,
			&setDefaultValue<glm::vec3>,
			&setDefaultValue<glm::vec4>
		),
	    // synergies, ...
	    "is_valid",
	    [](Ptr<GuiTransform> self) {
		    return self->getNodebase()->as<Core::Transform>()->isValid();
	    },
	    "is_locked",
	    [](Ptr<GuiTransform> self) {
		    return self->getNodebase()->as<Core::Transform>()->isLocked();
	    },
	    "is_in_sequence",
	    [](Ptr<GuiTransform> self) {
		    return self->getNodebase()->as<Core::Transform>()->isInSequence();
	    },
	    "lock",
	    [](Ptr<GuiTransform> self) {
		    self->getNodebase()->as<Core::Transform>()->lock();
	    },
	    "unlock",
	    [](Ptr<GuiTransform> self) {
		    self->getNodebase()->as<Core::Transform>()->unlock();
	    },
	    "has_synergies",
	    [](Ptr<GuiTransform> self) {
		    return self->getNodebase()->as<Core::Transform>()->hasSynergies();
	    },
	    "enable_synergies",
	    [](Ptr<GuiTransform> self) {
		    self->getNodebase()->as<Core::Transform>()->enableSynergies();
	    },
	    "disable_synergies",
	    [](Ptr<GuiTransform> self) {
		    self->getNodebase()->as<Core::Transform>()->disableSynergies();
	    },
	    sol::meta_function::construct,
	    [](const std::string& type) -> Ptr<GuiTransform> {
		    const auto maybeType = EnumUtils::value<Core::ETransformType>(type);
		    if (!maybeType.has_value())
		    {
			    print(fmt::format("Unknown transform \"{}\".", type));

			    return nullptr;
		    }

		    const auto maybeTransform = g_TransformBuilder(type.c_str());
		    if (!maybeTransform)
		    {
			    return nullptr;
		    }
		    const auto transform = *maybeTransform;
		    const auto ID = transform->getNodebase()->getId();
		    print(fmt::format("ID: {}", ID));

		    return std::dynamic_pointer_cast<GuiTransform>(transform);
	    });

	L.new_usertype<GuiSequence>(
	    "Sequence",
	    sol::base_classes, sol::bases<GuiNode, Workspace::CoreNodeWithPins>(),
		"get_mat4", &getValue<Workspace::Sequence, glm::mat4>,
	    "push", sol::overload(
			[](GuiSequence& self, Ptr<GuiTransform> transform) {
				self.moveNodeToSequence(transform);
			},
	    	[](GuiSequence& self, Ptr<GuiTransform> transform, int index) {
				self.moveNodeToSequence(transform, index);
			}
		),
	    "pop", [](GuiSequence& self, int index) -> Ptr<GuiTransform> {
		    auto maybeTransform = self.getTransform(index);
		    if (!maybeTransform.has_value())
		    {
			    print(fmt::format("{} is out of bounds", index));

			    return nullptr;
		    }

		    auto transform = std::dynamic_pointer_cast<GuiTransform>(maybeTransform.value());
		    self.moveNodeToWorkspace(transform);

		    return transform;
	    },
	    sol::meta_function::construct, []() -> Ptr<GuiSequence> {
		    auto sequence = Workspace::addNodeToNodeEditor<GuiSequence>();

		    print(fmt::format("ID: {}", sequence->getNodebase()->getId()));

		    return sequence;
	    });

	L.new_usertype<GuiModel>(
		"Model",
		sol::base_classes, sol::bases<GuiNode, Workspace::CoreNodeWithPins>(),
		sol::meta_function::construct, []() -> Ptr<GuiModel> {
			auto model = Workspace::addNodeToNodeEditor<GuiModel>();

			print(fmt::format("ID: {}", model->getNodebase()->getId()));

			return model;
		},
		"set_model", [](GuiModel& self, const std::string& value) {
			self.m_viewportModel.lock()->setModel(value);
		},
		"set_visible", [](GuiModel& self, bool value) {
			self.m_viewportModel.lock()->m_visible = value;
		},
		"show_axes", [](GuiModel& self, bool value) {
			self.m_viewportModel.lock()->m_showAxes = value;
		},
		"set_opaque", [](GuiModel& self, bool value) {
			self.m_viewportModel.lock()->m_opaque = value;
		},
		"set_opacity", [](GuiModel& self, float value) {
			self.m_viewportModel.lock()->m_opacity = value;
		},
		"set_tint", [](GuiModel& self, const glm::vec3& value) {
			self.m_viewportModel.lock()->m_tint = value;
		},
		"set_tint_strength", [](GuiModel& self, float value) {
			self.m_viewportModel.lock()->m_tintStrength = value;
		}
	);

	L.new_usertype<Workspace::Cycle>(
		"Cycle",
		sol::base_classes, sol::bases<GuiNode>(),
		sol::meta_function::construct, []() -> Ptr<Workspace::Cycle> {
			auto cycle = Workspace::addNodeToNodeEditor<Workspace::Cycle>();

			print(fmt::format("ID: {}", cycle->getNodebase()->getId()));

			return cycle;
		},
		"set_from", [](Workspace::Cycle& self, float value) {
			self.getNodebase()->as<Core::Cycle>()->setFrom(value);
		},
		"set_to", [](Workspace::Cycle& self, float value) {
			self.getNodebase()->as<Core::Cycle>()->setTo(value);
		},
		"set_manual_step", [](Workspace::Cycle& self, float value) {
			self.getNodebase()->as<Core::Cycle>()->setStep(value);
		},
		"set_step", [](Workspace::Cycle& self, float value) {
			self.getNodebase()->as<Core::Cycle>()->setStep(value);
		},
		"set_step_duration", [](Workspace::Cycle& self, float value) {
			self.getNodebase()->as<Core::Cycle>()->setStepDuration(value);
		},
		"set_smooth", [](Workspace::Cycle& self, bool value) {
			self.getNodebase()->as<Core::Cycle>()->setSmoothStep(value);
		},
		"set_mode", [](Workspace::Cycle& self, int value) {
			auto mode = Core::Cycle::EMode(value);
			self.getNodebase()->as<Core::Cycle>()->setMode(mode);
		},
		"play", [](Workspace::Cycle& self) {
			self.getNodebase()->as<Core::Cycle>()->play();
		}
	);

	L.new_usertype<Workspace::Camera>(
		"Camera",
		sol::base_classes, sol::bases<GuiNode>(),
		sol::meta_function::construct, []() -> Ptr<Workspace::Camera> {
			auto camera = Workspace::addNodeToNodeEditor<Workspace::Camera>();

			print(fmt::format("ID: {}", camera->getNodebase()->getId()));

			return camera;
		},
		"get_projection", [](Workspace::Camera& self) {
			return self.getProjection();
		},
		"get_view", [](Workspace::Camera& self) {
			return self.getView();
		}
	);

	L.new_usertype<Workspace::Screen>(
		"Screen",
		sol::base_classes, sol::bases<GuiNode>(),
		sol::meta_function::construct, []() -> Ptr<Workspace::Screen> {
			auto screen = Workspace::addNodeToNodeEditor<Workspace::Screen>();

			print(fmt::format("ID: {}", screen->getNodebase()->getId()));

			return screen;
		},
		"set_aspect", [](Workspace::Screen& self, const ImVec2& value) {
			self.setAspect(value);
		}
	);

	// I3T functions

	auto api = L["I3T"];

	api["workspace"] = L.create_table();
	auto workspace = api["workspace"];

	workspace["__scripts"] = L.create_table();

	workspace["set_zoom"] = [](float value) {
		getNodeEditor().diwne.setWorkAreaZoom(value);
	};
	workspace["set_work_area"] = [](const ImVec2& min, const ImVec2& max) {
		ImRect area(min, max);
		getNodeEditor().diwne.setWorkAreaDiwne(area);
	};

	workspace["clear"] = []() {
		for (const auto& node : getWorkspaceNodes())
		{
			node->deleteActionDiwne();
		}
	};

	api["to_script"] = []() -> std::string {
		return to_script(getWorkspaceNodes());
	};

	api["get_all_nodes"] = getWorkspaceNodes;

	api["get_node"] = findNode;

	api["delete_node"] = [](GuiNode& node) {
		node.deleteActionDiwne();
	};

	//

	api["plug"] = [](Core::ID from, int fromIdx, Core::ID to, int toIdx) {
		auto fromNode = findNode(from);
		auto toNode = findNode(to);

		if (!fromNode || !toNode)
		{
			return false;
		}

		return plug(fromNode, fromIdx, toNode, toIdx);
	};

	api["unplug_input"] = [](Core::ID id, int inputIndex) {
		auto node = findNode(id);

		if (!node)
		{
			return false;
		}

		return unplugInput(node, inputIndex);
	};

	api["unplug_output"] = [](Core::ID id, int outputIndex) {
		auto node = findNode(id);

		if (!node)
		{
			return false;
		}

		return unplugOutput(node, outputIndex);
	};

	//

	api["print_operator_types"] = []() {
		const auto types = magic_enum::enum_names<Core::EOperatorType>();
		for (const auto& t : types)
		{
			print(std::string(t));
		}
	};

	api["print_transform_types"] = []() {
		const auto types = magic_enum::enum_names<Core::ETransformType>();
		for (const auto& t : types)
		{
			print(std::string(t));
		}
	};

	api["print_workspace"] = []() {
		const auto& nodes = getNodeEditor().m_workspaceCoreNodes;
		for (const auto& node : nodes)
		{
			const auto& coreNode = node->getNodebase();
			const auto* op = coreNode->getOperation();

			print(fmt::format("node {}, type {}", coreNode->getId(), op->keyWord));
		}
	};

	// clang-format on
};
