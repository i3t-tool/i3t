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
#include "GUI/Elements/Modals/ScriptEditor.h"

#define IMGUI_DEFINE_MATH_OPERATORS
#include "imgui.h"

#include "Core/Nodes/Id.h"
#include "GUI/Workspace/Nodes/ScriptingNode.h"

static const std::string SCRIPT_TEMPLATE = R"(self.operation = Operation.new(
	-- inputs { name = type }
	{ x = ValueType.Float, y = ValueType.Float },
	-- outputs { name = type }
	{ result = ValueType.Vec3 }
)
self.on_init = function()
	-- custom initialization code
end
self.on_update_values = function()
	-- custom on update values code, access the node using
	-- self.node reference.
end
)";

std::unique_ptr<TextEditor> createEditor()
{
	auto editor = std::make_unique<TextEditor>();
	editor->SetLanguageDefinition(TextEditor::LanguageDefinition::Lua());

	auto palette = I3T::getTheme().isDark() ? TextEditor::GetDarkPalette() : TextEditor::GetLightPalette();
	editor->SetPalette(palette);

	return editor;
}

ScriptEditorModal::ScriptEditorModal(Workspace::ScriptingNode* node) : ModalWindow("Edit script"), m_node(node)
{
	m_editor = createEditor();
	auto script = node->getScript();
	if (script.empty())
	{
		script = SCRIPT_TEMPLATE;
	}
	m_editedScript = script;
	m_editor->SetText(m_editedScript);
}

void ScriptEditorModal::onImGui()
{
	ImGui::PushFont(I3T::getFont(EFont::Mono));

	m_editor->Render("Script Editor", ImVec2(600, 800), false);

	ImGui::PopFont();

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

	ImGui::SameLine();
	if (ImGui::Button("Save and close"))
	{
		onSave();
		close();
	}
}

void ScriptEditorModal::onSave()
{
	m_editor->SetErrorMarkers({});

	std::string script = m_editor->GetText();

	// Try to execute the script.
	const auto nextId = Core::IdGenerator::next();
	auto result = Workspace::createScript(nextId, script);
	if (!result)
	{
		auto err = result.error();
		if (err.line)
		{
			auto line = std::min(err.line.value(), m_editor->GetTotalLines());
			m_editor->SetErrorMarkers({{line, err.message}});
		}
		else
		{
			m_message = err.message;
		}
	}
	else
	{
		Workspace::removeScript(nextId);

		auto newNode = m_node->setScript(script);
		m_node = newNode.get();
	}
}
