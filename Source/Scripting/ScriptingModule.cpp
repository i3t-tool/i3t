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
#include "Scripting/ScriptingModule.h"

#include <functional>
#include <string_view>

#include "Commands/ApplicationCommands.h"
#include "Core/Defs.h"
#include "Core/Nodes/NodeData.h"
#include "Core/Nodes/Operations.h"
#include "GUI/Elements/Nodes/Builder.h"
#include "Scripting/Environment.h"
#include "Utils/Format.h"
#include "Utils/Variant.h"

static OperatorBuilder g_OperatorBuilder;
static TransformBuilder g_TransformBuilder;

std::function<void(const std::string& str)> g_printRef;

static WorkspaceDiwne& getNodeEditor()
{
	const auto workspace = I3T::getUI()->getWindowManager().getWindowPtr<WorkspaceWindow>();
	return workspace->getNodeEditor();
}

//----------------------------------------------------------------------------//

template <typename T> static std::optional<T> getValue(Ptr<GuiNode> guiNode, int index = 0)
{
	const auto node = guiNode->getNodebase();

	if (index >= node->getOutputPins().size())
	{
		g_printRef("no such index");
		return std::nullopt;
	}

	const auto maybeValue = node->data(index).getValue<T>();
	if (!maybeValue.has_value())
	{
		g_printRef("no such type of value");
		return std::nullopt;
	}

	return maybeValue;
}

template <typename T> static bool setValue(Ptr<GuiNode> guiNode, const T& value)
{
	const auto node = guiNode->getNodebase();

	if (!node->getOperation()->isConstructor)
	{
		g_printRef("cannot set value for this node");
		return false;
	}
	if (node->data().index() != variant_index<Core::Data::Storage, T>())
	{
		g_printRef("invalid value type for this node");
		return false;
	}

	const auto result = node->setValue(value);
	if (result.status != Core::SetValueResult::Status::Ok)
	{
		g_printRef(result.message);
		return false;
	}

	return true;
}

//----------------------------------------------------------------------------//

template <typename T> static std::optional<T> getDefaultValue(Ptr<GuiTransform> guiNode, const std::string& name)
{
	const auto transform = guiNode->getNodebase()->as<Core::Transform>();
	const auto maybeValue = transform->getDefaultValue(name).getValue<T>();
	if (!maybeValue.has_value())
	{
		g_printRef("transform does not have value of given type");
	}

	return maybeValue;
}

template <typename T> static bool setDefaultValue(Ptr<GuiTransform> guiNode, const std::string& name, const T& value)
{
	const auto node = guiNode->getNodebase()->as<Core::Transform>();
	const auto result = node->setDefaultValue(name, value);
	if (result.status != Core::SetValueResult::Status::Ok)
	{
		g_printRef(result.message);

		return false;
	}

	return true;
}

//----------------------------------------------------------------------------//

void ScriptingModule::onInit()
{
	// Setup I3T commands
	ConsoleCommand::addListener([this](std::string c) {
		runScript(c.c_str());
	});

	// Setup Lua

	g_printRef = [this](const std::string& str) {
		print(str);
	};

	//

	m_Lua.open_libraries(sol::lib::base, sol::lib::package);

	//

	// clang-format off
	m_Lua.new_usertype<ImVec2>(
	    "Vec2", sol::constructors<ImVec2(), ImVec2(float, float)>(),
	    "x", &ImVec2::x,
	    "y", &ImVec2::y,
	    sol::meta_function::to_string, [](const ImVec2& vec) {
		    return std::to_string(vec.x) + "\t" + std::to_string(vec.y);
	    });

	m_Lua.new_usertype<glm::vec3>(
	    "Vec3", sol::constructors<glm::vec3(), glm::vec3(float, float, float)>(),
	    "x", &glm::vec3::x,
	    "y", &glm::vec3::y,
	    "z", &glm::vec3::z,
	    sol::meta_function::to_string, [](const glm::vec3 self) {
		    return Utils::toString(self);
		});

	m_Lua.new_usertype<glm::vec4>(
	    "Vec4", sol::constructors<glm::vec4(), glm::vec4(float, float, float, float)>(),
	    "x", &glm::vec4::x,
	    "y", &glm::vec4::y,
	    "z", &glm::vec4::z,
	    "w", &glm::vec4::w,
	    sol::meta_function::to_string, [](const glm::vec4 self) {
			return Utils::toString(self);
		});

	m_Lua.new_usertype<glm::mat4>(
	    "Mat4", sol::constructors<glm::mat4, glm::mat4(float), glm::mat4(glm::vec4, glm::vec4, glm::vec4, glm::vec4)>(),
	    "x", [](const glm::mat4 self) {
		    return self[0];
	    },
	    "y", [](const glm::mat4 self) {
		    return self[1];
	    },
	    "z", [](const glm::mat4 self) {
		    return self[2];
	    },
	    "w", [](const glm::mat4 self) {
		    return self[3];
	    },
	    sol::meta_function::to_string, [](const glm::mat4 self) {
		    return Utils::toString(self, true);
	    });

	//

	m_Lua.new_usertype<GuiNode>(
		"Node",
		"type", [](Ptr<GuiNode> self) {
			return self->getNodebase()->getOperation()->keyWord;
		},
		"get_position", &GuiNode::getNodePositionDiwne,
		"set_position", &GuiNode::setNodePositionDiwne
	);

	m_Lua.new_usertype<GuiOperator>(
	    "Operator",
		sol::base_classes, sol::bases<GuiNode>(),
	    "get_float", &getValue<float>,
		"get_vec3", &getValue<glm::vec3>,
		"get_vec4", &getValue<glm::vec4>,
		"get_mat4", &getValue<glm::mat4>,
		"set_float", &setValue<float>,
		"set_vec3", &setValue<glm::vec3>,
		"set_vec4", &setValue<glm::vec4>,
		"plug", [](Ptr<GuiOperator> self, int from, Ptr<GuiOperator> other, int to) {
		    return connectNodesNoSave(self, other, from, to);
	    },
	    "unplug_input", [&](Ptr<GuiOperator> self, int inputIndex) {
		    if (auto node = std::dynamic_pointer_cast<WorkspaceNodeWithCoreDataWithPins>(self))
		    {
			    if (node->getInputs().size() <= inputIndex)
			    {
				    print(fmt::format("No such input index {}", inputIndex));
				    return;
			    }

			    if (node->getInputs()[inputIndex]->isConnected())
			    {
				    node->getInputs()[inputIndex]->unplug();
			    }
		    }
	    },
	    "unplug_output", [&](Ptr<GuiOperator> self, int outputIndex) {
		    if (auto node = std::dynamic_pointer_cast<WorkspaceNodeWithCoreDataWithPins>(self))
		    {
			    if (node->getOutputs().size() <= outputIndex)
			    {
				    print(fmt::format("No such output index {}", outputIndex));
				    return;
			    }

			    const auto& nodes = getNodeEditor().m_workspaceCoreNodes;
			    std::vector<std::pair<Ptr<WorkspaceNodeWithCoreDataWithPins>, int>> toUnplug;
			    for (const auto& outputPin : self->getNodebase()->getOutputPins())
			    {
				    for (const auto inputPin : outputPin.getOutComponents())
				    {
					    if (auto maybeNode = findNodeById(nodes, inputPin->Owner.getId()))
					    {
						    auto nodeWithPin = std::dynamic_pointer_cast<WorkspaceNodeWithCoreDataWithPins>(*maybeNode);
						    toUnplug.push_back({nodeWithPin, inputPin->Index});
					    }
				    }
			    }

			    for (const auto& [childNode, inputIndex] : toUnplug)
			    {
				    childNode->getInputs()[inputIndex]->unplug();
			    }
		    }
	    },
	    sol::meta_function::construct, [this](const std::string& type) -> Ptr<GuiOperator> {
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

	m_Lua.new_usertype<GuiTransform>(
	    "Transform",
		sol::base_classes, sol::bases<GuiNode>(),
	    "get_value", [](Ptr<GuiTransform> self) {
		    return getValue<glm::mat4>(self);
	    },
	    "set_value",
	    [this](Ptr<GuiTransform> self, float value, const ImVec2& coords) {
		    const auto glmCoords = glm::vec2(coords.x, coords.y);
		    const auto result = self->getNodebase()->setValue(value, glmCoords);
		    if (result.status != Core::SetValueResult::Status::Ok)
		    {
			    print(result.message);
		    }
	    },
	    // get default value
	    "get_float", &getDefaultValue<float>, "get_vec3", &getDefaultValue<glm::vec3>, "get_vec3",
	    &getDefaultValue<glm::vec4>,
	    // set default value
	    "set_float", &setDefaultValue<float>, "set_vec3", &setDefaultValue<glm::vec3>, "set_vec4",
	    &setDefaultValue<glm::vec4>,
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
	    [this](const std::string& type) -> Ptr<GuiTransform> {
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

	m_Lua.new_usertype<GuiSequence>(
	    "Sequence",
	    sol::base_classes, sol::bases<GuiNode>(),
	    "push", [this](GuiSequence& self, Ptr<GuiTransform> transform) {
		    self.moveNodeToSequence(transform);
	    },
	    "push", [this](GuiSequence& self, Ptr<GuiTransform> transform, int index) {
		    self.moveNodeToSequence(transform, index);
	    },
	    "pop", [this](GuiSequence& self, int index) -> Ptr<GuiTransform> {
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
	    sol::meta_function::construct, [this]() -> Ptr<GuiSequence> {
		    auto sequence = addNodeToNodeEditor<GuiSequence>();

		    print(fmt::format("ID: {}", sequence->getNodebase()->getId()));

		    return sequence;
	    });

	//

	m_Lua.set_function("get_all_nodes", []() -> std::vector<Ptr<GuiNode>> {
	  	return getNodeEditor().m_workspaceCoreNodes;
	});

	m_Lua.set_function("get_node", [this](Core::ID id) -> Ptr<GuiNode> {
		const auto result = getNodeEditor().getNode<GuiNode>(id);

		if (!result)
		{
			print(result.error().str());

			return nullptr;
		}

		return result.value();
	});

	m_Lua.set_function("get_transform", [this](Core::ID id) -> Ptr<GuiTransform> {
		const auto result = getNodeEditor().getNode<GuiTransform>(id);

		if (!result)
		{
			print(result.error().str());

			return nullptr;
		}

		return result.value();
	});

	m_Lua.set_function("delete", [](GuiNode& node) {
		node.deleteActionDiwne();
	});

	//

	m_Lua.set_function("eval", [this](const std::string& scriptSource) {
		runScript(scriptSource.c_str());
	});

	m_Lua.set_function("eval_from_file", [this](const std::string& path) {
		if (!fs::exists(fs::path(path)))
		{
			print("file does not exist");
			return false;
		}
		return true;
	});

	m_Lua.set_function("save_script", [](const std::string& path) {
		/// \todo
	});

	//

	m_Lua.set_function("help", [this]() {
		print("This is an I3T scripting module running on Lua "
		      "(sol " SOL_VERSION_STRING ").");
	});

	//

	m_Lua.set_function("_AppendToBuffer", [this](const std::string& str) {
		print(str);
	});

	m_Lua.script(R"(
		local _Print = print
		print = function(...)
			local args = {...}
			if #args == 0 then
				_AppendToBuffer("nil")
				return
			end

			local printResult = ""
			for i,v in ipairs(args) do
				printResult = printResult .. tostring(v) .. "\t"
			end
			_AppendToBuffer(printResult)
		end
)");

	//

	m_Lua.set_function("print_operator_types", [this]() {
		const auto types = magic_enum::enum_names<Core::EOperatorType>();
		for (const auto& t : types)
		{
			print(std::string(t));
		}
	});

	m_Lua.set_function("print_transform_types", [this]() {
		const auto types = magic_enum::enum_names<Core::ETransformType>();
		for (const auto& t : types)
		{
			print(std::string(t));
		}
	});

	m_Lua.set_function("print_workspace", [this]() {
		const auto& nodes = getNodeEditor().m_workspaceCoreNodes;
		for (const auto& node : nodes)
		{
			const auto& coreNode = node->getNodebase();
			const auto* op = coreNode->getOperation();

			print(fmt::format("node {}, type {}", coreNode->getId(), op->keyWord));
		}
	});

	//

	m_Lua.set_function("load_scene", [](const std::string& path) {
		/// \todo
	});

	m_Lua.set_function("save_scene", [](const std::string& path) {
		/// \todo
	});
	// clang-format on

	//-- Timers --------------------------------------------------------------------------------------------------------

	m_Lua.set_function("set_timer", [this](uint64_t intervalMs, sol::protected_function callback) {
		callback.set_error_handler(m_Lua["print"]);

		return m_chronos.setTimer(intervalMs, callback);
	});

	m_Lua.set_function("clear_timer", [this](Ptr<Timer> timer) {
		m_chronos.clearTimer(timer);
	});

	//------------------------------------------------------------------------------------------------------------------

	Registry::registerAll(m_Lua);
}

void ScriptingModule::onUpdate(double deltaSeconds)
{
	m_chronos.update(deltaSeconds);
}

void ScriptingModule::onClose()
{
	// Destroy timers before the workspace, because of weird DINWE nodes ownership issues.
	m_chronos = {};
}

bool ScriptingModule::runScript(const char* luaSource)
{
	try
	{
		m_Lua.safe_script(luaSource);
	}
	catch (const std::exception& e)
	{
		print(e.what());
		return false;
	}

	return true;
}

void ScriptingModule::print(const std::string& str)
{
	LOG_INFO(str);
	m_oss << str << "\n";
}
