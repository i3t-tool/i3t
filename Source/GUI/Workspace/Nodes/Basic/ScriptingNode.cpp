/**
 * \file
 * \brief
 * \authors Martin Herich
 * \copyright Copyright (C) 2016-2024 I3T team, Department of Computer Graphics
 * and Interaction, FEE, Czech Technical University in Prague, Czech Republic
 *
 * This file is part of I3T - An Interactive Tool for Teaching Transformations
 * http://www.i3t-tool.org
 *
 * GNU General Public License v3.0 (see LICENSE.txt or https://www.gnu.org/licenses/gpl-3.0.txt)
 */

#include "GUI/Workspace/Nodes/ScriptingNode.h"

#include "GUI/Elements/Windows/WorkspaceWindow.h"
#include "Scripting/ScriptingModule.h"

constexpr const char* SCRIPTING_NODE_TEMPLATE = R"(
I3T.workspace.__scripts[{}] = {{}}
do
    local self = I3T.workspace.__scripts[{}]
    {}
end)";

//------------------------------------------------------------------------------------------------//

/// \todo This function is not working properly, it is not possible to get the node from the environment.
static auto getNode(ScriptingModule& scripting, Core::ID id)
{
	return scripting.environment()["I3T"]["workspace"]["__scripts"][id];
}

class ScriptingNode : public Core::Node
{
public:
	ScriptingNode() : Node(DEFAULT_SCRIPTING_NODE_OPERATION) {}

	explicit ScriptingNode(std::unique_ptr<Workspace::ScriptInterface> interface,
	                       Workspace::ScriptingNode* workspaceNode)
	    : Node(interface->operation), m_workspaceNode(workspaceNode), m_interface(std::move(interface))
	{}

	~ScriptingNode() override
	{
		if (m_interface)
		{
			Workspace::removeScript(m_interface->id);
		}
	}

	/// Cannot be used Node::onInit function because it is called before the node is added to the workspace.
	void performInit()
	{
		if (m_workspaceNode != nullptr && m_interface != nullptr)
		{
			auto& lua = Application::getModule<ScriptingModule>().environment();
			auto script = lua["I3T"]["workspace"]["__scripts"][m_interface->id];

			script["node"] = std::dynamic_pointer_cast<Workspace::CoreNode>(m_workspaceNode->shared_from_this());

			if (m_interface->onInit)
			{
				m_interface->onInit();
			}
			m_isInitialized = true;
		}
	}

	void updateValues(int inputIndex) override
	{
		if (m_isInitialized && m_interface->onUpdate)
		{
			m_interface->onUpdate();
		}
	}

	Workspace::ScriptInterface* getInterface() const
	{
		return m_interface.get();
	}

private:
	Workspace::ScriptingNode* m_workspaceNode = nullptr;
	std::unique_ptr<Workspace::ScriptInterface> m_interface = nullptr;

	bool m_isInitialized = false;
};

//------------------------------------------------------------------------------------------------//

namespace Workspace
{
Result<std::unique_ptr<Workspace::ScriptInterface>, Error> createScript(Core::ID id, const std::string& script)
{
	auto& scripting = Application::getModule<ScriptingModule>();

	auto toExecute = fmt::format(SCRIPTING_NODE_TEMPLATE, id, id, script);
	const bool result = scripting.runScript(toExecute.c_str());

	if (!result)
	{
		return Err(Error("Failed to create node from script"));
	}

	// auto node = getNode(scripting, id);
	auto& lua = scripting.environment();
	auto node = lua["I3T"]["workspace"]["__scripts"][id];

	if (node["operation"] != sol::nil && !node["operation"].is<Core::Operation>())
	{
		return Err(Error("Node Operation is invalid."));
	}

	Core::Operation operation = node["operation"].get_or<Core::Operation>(DEFAULT_SCRIPTING_NODE_OPERATION);

	auto onInit = node["on_init"].get_or<sol::function>(sol::nil);
	if (onInit)
	{
		onInit.set_error_handler(lua["print"]);
	}

	auto onUpdate = node["on_update_values"].get_or<sol::function>(sol::nil);
	if (onUpdate)
	{
		onUpdate.set_error_handler(lua["print"]);
	}

	return std::make_unique<Workspace::ScriptInterface>(id, operation, std::move(onInit), std::move(onUpdate));
}

void removeScript(Core::ID id)
{
	auto& scripting = Application::getModule<ScriptingModule>();
	auto scripts = scripting.environment()["I3T"]["workspace"]["__scripts"];
	if (scripts != sol::nil)
	{
		scripts[id]["node"] = sol::nil;
		scripts[id] = sol::nil;
	}
}

//------------------------------------------------------------------------------------------------//

class ScriptEditorModal : public ModalWindow
{
public:
	explicit ScriptEditorModal(
	    std::string& script, std::function<void(void)> onSave =
	                             [] {
	                             })
	    : ModalWindow("Edit script"), m_script(script), m_onSave(std::move(onSave))
	{
		if (!m_script.empty())
		{
			std::copy(m_script.begin(), m_script.end(), m_editedScript.begin());
		}
	}

	void onImGui() override
	{
		ImGui::InputTextMultiline("##script", m_editedScript.data(), m_editedScript.size(), ImVec2(400, 400));

		if (!m_message.empty())
		{
			ImGui::TextColored(ImVec4(0.8f, 0.0f, 0.0f, 1.0f), m_message.c_str());
		}

		if (ImGui::Button("Cancel"))
		{
			close();
		}

		ImGui::SameLine();
		if (ImGui::Button("Save"))
		{
			onSave();
		}
	}

private:
	void onSave()
	{
		// Trim ending null character.
		std::string script(m_editedScript.begin(), m_editedScript.end());
		script.erase(std::find(script.begin(), script.end(), '\0'), script.end());

		// Try to execute the script.
		const auto nextId = Core::IdGenerator::next();
		auto result = createScript(nextId, script);
		if (!result)
		{
			using namespace std::string_literals;
			m_message = "Script has errors, see console or log output for details.\n"s + result.error().str();
		}
		else
		{
			m_script = script;
			removeScript(nextId);
			close();
			m_onSave();
		}
	}

	std::string m_message;
	std::string& m_script;
	std::array<char, 1024 * 10> m_editedScript = {0};
	std::function<void(void)> m_onSave;
};

//------------------------------------------------------------------------------------------------//

ScriptingNode::ScriptingNode(DIWNE::Diwne& diwne)
    : CoreNodeWithPins(diwne, Core::GraphManager::createCustomNode<::ScriptingNode>())
{}

ScriptingNode::ScriptingNode(DIWNE::Diwne& diwne, const std::string& script, std::unique_ptr<ScriptInterface> interface)
    : CoreNodeWithPins(diwne, Core::GraphManager::createCustomNode<::ScriptingNode>(std::move(interface), this)),
      m_script(script)
{
	m_interface = getNodebase()->as<::ScriptingNode>()->getInterface();
}

void ScriptingNode::popupContent()
{
	const auto workspace = I3T::getUI()->getWindowManager().getWindowPtr<WorkspaceWindow>();
	auto& nodeEditor = workspace->getNodeEditor();
	auto& nodes = nodeEditor.m_workspaceCoreNodes;

	CoreNode::drawMenuSetEditable();

	ImGui::Separator();

	CoreNode::drawMenuDuplicate();

	ImGui::Separator();

	if (ImGui::MenuItem("Edit script"))
	{
		auto modal = std::make_unique<ScriptEditorModal>(m_script, [this] {
			reloadScript();
		});
		I3T::getUI()->getWindowManager().openModal(std::move(modal));
	}

	if (ImGui::MenuItem("Reload script"))
	{
		reloadScript();
	}
}

int ScriptingNode::maxLengthOfData()
{
	return 0;
}

void ScriptingNode::drawMenuLevelOfDetail()
{
	drawMenuLevelOfDetail_builder(std::dynamic_pointer_cast<CoreNode>(shared_from_this()),
	                              {LevelOfDetail::Full, LevelOfDetail::Label});
}

Ptr<ScriptingNode> ScriptingNode::setScript(const std::string& script)
{
	m_script = script;

	return reloadScript();
}

Ptr<ScriptingNode> ScriptingNode::reloadScript()
{
	auto self = std::static_pointer_cast<ScriptingNode>(shared_from_this());
	auto coreNode = std::static_pointer_cast<::ScriptingNode>(m_nodebase);
	const auto id = Core::IdGenerator::next();
	auto interface = createScript(id, m_script);
	if (!interface)
	{
		LOG_ERROR("Failed to reload script");
		return nullptr;
	}

	auto newNode = std::make_shared<ScriptingNode>(diwne, m_script, std::move(interface.value()));

	const auto workspace = I3T::getUI()->getWindowManager().getWindowPtr<WorkspaceWindow>();
	auto& nodeEditor = workspace->getNodeEditor();
	nodeEditor.replaceNode(self, newNode);
	self->m_interface = nullptr;

	newNode->getNodebase()->as<::ScriptingNode>()->performInit();

	// All connections are lost, so we need to reconnect the node.
	auto connections = collectNeighbours(newNode->getId());
	for (const auto& [fromId, fromIndex, toId, toIndex] : connections)
	{
		if (!Tools::plug(fromId, fromIndex, toId, toIndex))
		{
			LOG_ERROR("Failed to reconnect node: Node#{}[{}] -> Node#{}[{}]", fromId, fromIndex, toId, toIndex);
		}
	}
	newNode->setNodePositionDiwne(getNodePositionDiwne());

	return newNode;
}

std::vector<std::tuple<Core::ID, int, Core::ID, int>> ScriptingNode::collectNeighbours(Core::ID overrideId) const
{
	std::vector<std::tuple<Core::ID, int, Core::ID, int>> neighbours;
	const auto node = getNodebase();
	for (auto i = 0L; i < node->getInputPins().size(); i++)
	{
		auto& pin = node->getInputPins()[i];
		if (pin.isPluggedIn())
		{
			auto* parentPin = pin.getParentPin();
			auto fromId = parentPin->Owner.getId();
			auto fromIndex = parentPin->Index;
			auto toId = overrideId;
			auto toIndex = i;
			neighbours.emplace_back(fromId, fromIndex, toId, toIndex);
		}
	}

	for (auto i = 0L; i < node->getOutputPins().size(); i++)
	{
		auto& pin = node->getOutputPins()[i];
		for (auto* outputPin : pin.getOutComponents())
		{
			auto fromId = overrideId;
			auto fromIndex = i;
			auto toId = outputPin->Owner.getId();
			auto toIndex = outputPin->Index;
			neighbours.emplace_back(fromId, fromIndex, toId, toIndex);
		}
	}

	return neighbours;
}
} // namespace Workspace
