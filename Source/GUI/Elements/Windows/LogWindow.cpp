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
#include "LogWindow.h"
#include "GUI/IconFonts/Icons.h"
#include "GUI/Toolkit.h"

#define IMGUI_DEFINE_MATH_OPERATORS
#include "imgui.h"

#include "Core/Input/InputBindings.h"
#include "Core/Input/InputManager.h"
#include "GUI/Theme/Theme.h"
#include "I3T.h"
#include "Logger/Logger.h"

LogWindow::LogWindow() : IWindow(ICON_I3T_LOG " Log View")
{
#ifdef I3T_DEBUG
	m_input->bindAction("MyTestAction", EKeyState::Pressed, [] {
		LOG_INFO("MyTestAction triggered!");
	});

	m_input->bindAxis("MyTestAxis", [](float val) {
		LOG_INFO("MyTestAxis triggered: {}!", val);
	});

	////
	m_input->bindAction("createAndPlugConstructor", EKeyState::Released, [] {
		const auto isActionTriggered = InputManager::isActionTriggered("createAndPlugConstructor", EKeyState::Released);
		LOG_INFO("InputManager::isActionTriggered("
		         "\"createAndPlugConstructor\") = {}",
		         isActionTriggered);
	});
	////

	m_input->bindAction("TestMouseCtrlAction", EKeyState::Pressed, [] {
		LOG_INFO("bind: (mouse click + left ctrl) pressed");
	});

	m_input->bindAction("scrollUp", EKeyState::Pressed, []() {
		LOG_INFO("scrollUp");
	});

	m_input->bindAction("scrollDown", EKeyState::Pressed, []() {
		LOG_INFO("scrollDown");
	});

	m_input->bindAxis("scroll", [](float val) {
		LOG_INFO("scroll: {}", val);
	});
#endif
}

void LogWindow::render()
{
	GUI::dockTabStylePush();
	ImGui::Begin(getName(), getShowPtr());
	GUI::dockTabStylePop();
	this->updateWindowInfo();

	ImGui::Text("Log output");

	/// \todo MH test code, remove
	static bool val = true;

	ImGui::Text("Switch fire action key");
	ImGui::SameLine();
	std::string s;
	for (auto action : InputBindings::getActionMapping("fire"))
	{
		s += std::to_string(action.code);
	}
	ImGui::Text("Current keys: %s", s.c_str());
	if (ImGui::Button("Switch"))
	{
#ifdef I3T_DEBUG
		val = !val;
		if (val)
		{
			InputBindings::setActionKey("fire", Keys::Code::b);
			InputBindings::removeActionKey("fire", Keys::Code::v);
		}
		else
		{
			InputBindings::setActionKey("fire", Keys::Code::v);
			InputBindings::removeActionKey("fire", Keys::Code::b);
		}
#endif
	}

	ImGui::BeginChild(ID);
	ImGui::TextUnformatted(Logger::getInstance().getBuffer().str().c_str());

	// Set scroll to bottom of the child window.
	if (ImGui::GetScrollY() >= ImGui::GetScrollMaxY())
		ImGui::SetScrollHereY(1.0f);

	if (InputManager::isActionTriggered("createAndPlugConstructor", EKeyState::Released))
	{
		LOG_INFO("query: (mouse click + left ctrl) pressed");
	}

	ImGui::EndChild();

	ImGui::End();
}
