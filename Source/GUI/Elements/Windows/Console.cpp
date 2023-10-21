/**
 * \file
 * \brief
 * \author Martin Herich
 * \copyright Copyright (C) 2016-2023 I3T team, Department of Computer Graphics
 * and Interaction, FEE, Czech Technical University in Prague, Czech Republic
 *
 * This file is part of I3T - An Interactive Tool for Teaching Transformations
 * http://www.i3t-tool.org
 *
 * GNU General Public License v3.0 (see LICENSE.txt or https://www.gnu.org/licenses/gpl-3.0.txt)
 */
#include "Console.h"

#include "imgui.h"

#include "API.h"
#include "Commands/ApplicationCommands.h"
#include "Core/Input/InputManager.h"
#include "GUI/Elements/IWindow.h"
#include "GUI/Toolkit.h"
#include "Scripting/ScriptingModule.h"

using namespace UI;

static int textEditCallbackStub(ImGuiInputTextCallbackData* data)
{
	auto* console = (Console*) data->UserData;
	return console->textEditCallback(data);
}

Console::Console(bool show) : IWindow(show), m_oss(App::get().getModule<ScriptingModule>().outputStream())
{
	m_oss << "I3T console\n";
}

void Console::render()
{
	const auto& style = ImGui::GetStyle();
	m_InputHeight = ImGui::GetFontSize() + 2 * style.FramePadding.y + style.ItemSpacing.y;

	drawOutput();
	drawInput();
}

int Console::textEditCallback(ImGuiInputTextCallbackData* data)
{
	switch (data->EventFlag)
	{
	case ImGuiInputTextFlags_CallbackHistory:
	{
		const int prev_history_pos = m_HistoryPos;
		if (data->EventKey == ImGuiKey_UpArrow)
		{
			if (m_HistoryPos == -1)
				m_HistoryPos = m_History.size() - 1;
			else if (m_HistoryPos > 0)
				m_HistoryPos--;
		}
		else if (data->EventKey == ImGuiKey_DownArrow)
		{
			if (m_HistoryPos != -1)
				if (++m_HistoryPos >= m_History.size())
					m_HistoryPos = -1;
		}

		// A better implementation would preserve the data on the current input line along with cursor position.
		if (prev_history_pos != m_HistoryPos)
		{
			const auto history_str = (m_HistoryPos >= 0) ? m_History[m_HistoryPos] : "";
			data->DeleteChars(0, data->BufTextLen);
			data->InsertChars(0, history_str.c_str());
		}
	}
	default:
		break;
	}

	return 0;
}

void Console::drawOutput()
{
	const auto flags = ImGuiInputTextFlags_ReadOnly;

	const auto buffer = m_oss.view();

	auto maxSize = ImGui::GetContentRegionAvail();
	maxSize.y -= m_InputHeight;

	m_oss << "X";
	m_oss.seekp(0, std::ios::end);
	int bufferSize = m_oss.tellp();

	const auto str = (char*) buffer.data();
	str[bufferSize - 2] = '\0';
	str[bufferSize - 1] = '\0';

	ImGui::InputTextMultiline("##ConsoleOutput", (char*) buffer.data(), bufferSize - 2, maxSize, flags);

	m_oss.seekp(-2, m_oss.cur);

	if (m_ShouldSetFocus)
	{
		// Scroll to the bottom.
		// https://github.com/ocornut/imgui/issues/1523#issuecomment-354003592
		ImGui::BeginChild("##ConsoleOutput");
		ImGui::SetScrollHereY(1.0f);
		ImGui::EndChild();
	}
}

void Console::drawInput()
{
	if (m_ShouldSetFocus)
	{
		ImGui::SetKeyboardFocusHere();
		m_ShouldSetFocus = false;
	}

	const auto flags = ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_CallbackCompletion |
	                   ImGuiInputTextFlags_CallbackHistory;

	ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x);
	if (GUI::InputText("#Command", &m_Command, flags, &textEditCallbackStub, (void*) this))
	{
		execute();
	}
}

void Console::execute()
{
	m_ShouldSetFocus = true;

	const bool isValid = !m_Command.empty();

	if (!isValid)
	{
		return;
	}

	m_oss << "\n> " << m_Command << "\n";

	ConsoleCommand::dispatch(m_Command);

	m_HistoryPos = -1;
	for (int i = m_History.size() - 1; i >= 0; i--)
	{
		if (m_History[i] == m_Command)
		{
			m_History.erase(m_History.begin() + i);
			break;
		}
	}
	m_History.push_back(m_Command);

	m_Command = "";
}
