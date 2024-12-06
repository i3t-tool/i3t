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

#include "Scripting/Environment.h"
#include "Scripting/ScriptingModule.h"

struct ScriptInterface
{
	sol::function onInit;
	sol::function onUpdate;
};

constexpr const char* SCRIPTING_NODE_TEMPLATE = R"(
I3T.workspace.__scripts[{}] = {{}}
do
    local self = I3T.workspace.__scripts[{}]
    {}
end)";

static Result<ScriptInterface, Error> createScript(Core::ID id, const std::string& script)
{
	auto& scripting = Application::getModule<ScriptingModule>();

	auto toExecute = fmt::format(SCRIPTING_NODE_TEMPLATE, id, id, script);
	const bool result = scripting.runScript(toExecute.c_str());

	if (!result)
	{
		return Err(Error("Failed to create node from script"));
	}

	auto node = scripting.environment()["I3T"]["workspace"]["__scripts"][id];

	return ScriptInterface{node["on_init"].get<sol::function>(), node["on_update_values"].get<sol::function>()};
}

static void removeScript(Core::ID id)
{
	auto& scripting = Application::getModule<ScriptingModule>();
	auto scripts = scripting.environment()["I3T"]["workspace"]["__scripts"];
	scripts[id] = sol::nil;
}

//------------------------------------------------------------------------------------------------//

static Core::Operation operation = {"Script", "script"};

class ScriptingNode : public Core::Node
{
public:
	ScriptingNode() : Node(&operation) {}

	explicit ScriptingNode(const std::string& script) : Node(&operation), m_script(script)
	{
		m_interface = createScript(getId(), script).value();
	}

	~ScriptingNode() override
	{
		removeScript(getId());
	}

	void onInit() override
	{
		if (m_interface.onInit)
		{
			m_interface.onInit();
		}
	}

	void updateValues(int inputIndex) override
	{
		if (m_interface.onUpdate)
		{
			m_interface.onUpdate();
		}
	}

	std::string m_script;

private:
	ScriptInterface m_interface;
};

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
		m_editedScript = std::vector(script.begin(), script.end());
		m_editedScript.push_back('\0');
	}

	void onImGui() override
	{
		ImGui::InputTextMultiline("##script", m_editedScript.data(), m_editedScript.size(), ImVec2(400, 400),
		                          ImGuiInputTextFlags_CallbackResize, textEditCallback, (void*) &m_editedScript);

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
		m_editedScript.pop_back();
		std::string script(m_editedScript.begin(), m_editedScript.end());

		// Try to execute the script.
		const auto nextId = Core::IdGenerator::next();
		auto result = createScript(nextId, script);
		if (!result)
		{
			/// \todo Show error message from the script.
			m_message = "Script has errors, see console or log output for details.";
		}
		else
		{
			m_script = script;
			removeScript(nextId);
			close();
			m_onSave();
		}
	}

	static int textEditCallback(ImGuiInputTextCallbackData* data)
	{
		if (data->EventFlag == ImGuiInputTextFlags_CallbackResize)
		{
			auto* editedScript = (std::vector<char>*) (data->UserData);
			editedScript->resize(data->BufSize);
			data->Buf = editedScript->begin().operator->();
		}
		return 0;
	}

	std::string m_message;
	std::string& m_script;
	std::vector<char> m_editedScript;
	std::function<void(void)> m_onSave;
};

//------------------------------------------------------------------------------------------------//

namespace Workspace
{
ScriptingNode::ScriptingNode(DIWNE::Diwne& diwne)
    : CoreNodeWithPins(diwne, Core::GraphManager::createCustomNode<::ScriptingNode>())
{}

ScriptingNode::ScriptingNode(DIWNE::Diwne& diwne, const std::string& script)
    : CoreNodeWithPins(diwne, Core::GraphManager::createCustomNode<::ScriptingNode>(script))
{}

void ScriptingNode::popupContent()
{
	CoreNode::drawMenuSetEditable();

	ImGui::Separator();

	CoreNode::drawMenuDuplicate();

	ImGui::Separator();

	if (ImGui::MenuItem("Edit script"))
	{
		auto& script = std::static_pointer_cast<::ScriptingNode>(m_nodebase)->m_script;
		auto modal = std::make_unique<ScriptEditorModal>(script, [this] {
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

void ScriptingNode::reloadScript()
{
	auto self = std::static_pointer_cast<ScriptingNode>(shared_from_this());
	auto coreNode = std::static_pointer_cast<::ScriptingNode>(m_nodebase);
	auto newNode = std::make_shared<ScriptingNode>(diwne, coreNode->m_script);
	self.swap(newNode);
}
} // namespace Workspace
