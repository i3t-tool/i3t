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
#include "Scripting/Utils.h"

static Workspace::OperatorBuilder g_OperatorBuilder;
static Workspace::TransformBuilder g_TransformBuilder;

static Workspace::WorkspaceDiwne& getNodeEditor()
{
	const auto workspace = I3T::getUI()->getWindowManager().getWindowPtr<WorkspaceWindow>();
	return workspace->getNodeEditor();
}

//----------------------------------------------------------------------------//

template <typename T>
static std::optional<T> getValue(Ptr<GuiNode> guiNode, int index = 0)
{
	const auto node = guiNode->getNodebase();

	if (index >= node->getOutputPins().size())
	{
		print("no such index");
		return std::nullopt;
	}

	const auto maybeValue = node->data(index).getValue<T>();
	if (!maybeValue.has_value())
	{
		print("no such type of value");
		return std::nullopt;
	}

	return maybeValue;
}

template <typename T>
static bool setValue(Ptr<GuiNode> guiNode, const T& value)
{
	const auto node = guiNode->getNodebase();

	if (!node->getOperation()->isConstructor)
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

//----------------------------------------------------------------------------//

LUA_REGISTRATION
{
	// clang-format off
	L.new_usertype<GuiNode>(
	    "Node",
	    "type", [](Ptr<GuiNode> self) {
		    return self->getNodebase()->getOperation()->keyWord;
	    },
	    "get_position", &GuiNode::getNodePositionDiwne,
	    "set_position", &GuiNode::setNodePositionDiwne
	);

	L.new_usertype<GuiOperator>(
	    "Operator",
	    sol::base_classes, sol::bases<GuiNode>(),
	    // getters
	    "get_float", &getValue<float>,
	    "get_vec3", &getValue<glm::vec3>,
	    "get_vec4", &getValue<glm::vec4>,
	    "get_mat4", &getValue<glm::mat4>,
	    // setters
	    "set_float", &setValue<float>,
	    "set_vec3", &setValue<glm::vec3>,
	    "set_vec4", &setValue<glm::vec4>,
	    // inputs/outputs
	    "plug", [](Ptr<GuiOperator> self, int from, Ptr<GuiOperator> other, int to) -> bool {
		    return connectNodesNoSave(self, other, from, to);
	    },
	    "unplug_input", [&](Ptr<GuiOperator> self, int inputIndex) -> bool {
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
	    },
	    "unplug_output", [&](Ptr<GuiOperator> self, int outputIndex) {
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
	    },
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
	    sol::base_classes, sol::bases<GuiNode>());

	L.new_usertype<GuiTransform>(
	    "Transform",
	    sol::base_classes, sol::bases<GuiNode>(),
	    "get_value", [](Ptr<GuiTransform> self) {
		    return getValue<glm::mat4>(self);
	    },
	    "set_value",
	    [](Ptr<GuiTransform> self, float value, const ImVec2& coords) {
		    const auto glmCoords = glm::vec2(coords.x, coords.y);
		    const auto result = self->getNodebase()->setValue(value, glmCoords);
		    if (result.status != Core::SetValueResult::Status::Ok)
		    {
			    print(result.message);
		    }
	    },
	    // get default value
	    "get_float", &getDefaultValue<float>,
	    "get_vec3", &getDefaultValue<glm::vec3>,
	    "get_vec3", &getDefaultValue<glm::vec4>,
	    // set default value
	    "set_float", &setDefaultValue<float>,
	    "set_vec3", &setDefaultValue<glm::vec3>,
	    "set_vec4", &setDefaultValue<glm::vec4>,
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
	    sol::base_classes, sol::bases<GuiNode>(),
	    "push", [](GuiSequence& self, Ptr<GuiTransform> transform) {
		    self.moveNodeToSequence(transform);
	    },
	    "push", [](GuiSequence& self, Ptr<GuiTransform> transform, int index) {
		    self.moveNodeToSequence(transform, index);
	    },
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

	// I3T functions

	auto api = L["I3T"];

	api["get_all_nodes"] = getWorkspaceNodes;

	api["get_node"] = [](Core::ID id) -> Ptr<GuiNode> {
		const auto result = getNodeEditor().getNode<GuiNode>(id);

		if (!result)
		{
			print(result.error().str());

			return nullptr;
		}

		return result.value();
	};

	api["get_transform"] = [](Core::ID id) -> Ptr<GuiTransform> {
		const auto result = getNodeEditor().getNode<GuiTransform>(id);

		if (!result)
		{
			print(result.error().str());

			return nullptr;
		}

		return result.value();
	};

	api["delete_node"] = [](GuiNode& node) {
		node.deleteActionDiwne();
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
