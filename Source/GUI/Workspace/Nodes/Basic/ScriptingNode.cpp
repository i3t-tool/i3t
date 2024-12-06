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

static Core::Operation operation = {"Script", "script"};

class ScriptingNode : public Core::Node
{
public:
	ScriptingNode() : Node(&operation) {}

	explicit ScriptingNode(const std::string& script) : Node(&operation), m_script(script)
	{
		/// \todo Delete old script node!
		auto& scripting = Application::getModule<ScriptingModule>();

		auto toExecute = fmt::format(R"(
I3T.workspace.__scripts[{}] = {{}}
do
    local self = I3T.workspace.__scripts[{}]
    {}
end)",
		                             getId(), getId(), script);

		if (scripting.runScript(toExecute.c_str()))
		{
			auto node = scripting.environment()["I3T"]["workspace"]["__scripts"][getId()];
			m_onInit = node["on_init"].get<sol::function>();
			m_onUpdate = node["on_update_values"].get<sol::function>();
		}
	}

	~ScriptingNode() override
	{
		auto& scripting = Application::getModule<ScriptingModule>();
		auto scripts = scripting.environment()["I3T"]["workspace"]["__scripts"];
		scripts[getId()] = sol::nil;
	}

	void onInit() override
	{
		if (m_onInit)
		{
			m_onInit();
		}
	}

	void updateValues(int inputIndex) override
	{
		if (m_onUpdate)
		{
			m_onUpdate();
		}
	}

	std::string m_script;

private:
	sol::function m_onInit;
	sol::function m_onUpdate;
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
		auto callback = [](ImGuiInputTextCallbackData* data) -> int {
			if (data->EventFlag == ImGuiInputTextFlags_CallbackResize)
			{
				auto* editedScript = (std::vector<char>*) (data->UserData);
				editedScript->resize(data->BufSize);
				data->Buf = editedScript->begin().operator->();
			}
			return 0;
		};
		ImGui::InputTextMultiline("##script", m_editedScript.data(), m_editedScript.size(), ImVec2(400, 400),
		                          ImGuiInputTextFlags_CallbackResize, callback, (void*) &m_editedScript);

		if (ImGui::Button("Cancel"))
		{
			close();
		}

		ImGui::SameLine();
		if (ImGui::Button("Save"))
		{
			// trim ending null character
			m_editedScript.pop_back();
			m_script = std::string(m_editedScript.begin(), m_editedScript.end());

			close();
			m_onSave();
		}
	}

private:
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
