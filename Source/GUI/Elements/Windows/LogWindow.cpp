#include "LogWindow.h"

#include "imgui.h"

#include "Core/API.h"
#include "Core/Application.h"
#include "Core/Input/InputBindings.h"
#include "Core/Input/InputManager.h"
#include "Core/Nodes/GraphManager.h"
#include "GUI/Theme.h"
#include "GUI/UIModule.h"
#include "Logger/Logger.h"

LogWindow::LogWindow()
{
#ifdef I3T_DEBUG
	Input.bindAction("MyTestAction", EKeyState::Pressed, [] {
		LOG_INFO("MyTestAction triggered!");
	});

	Input.bindAxis("MyTestAxis", [](float val) {
		LOG_INFO("MyTestAxis triggered: {}!", val);
	});

	////
	Input.bindAction("createAndPlugConstructor", EKeyState::Released, [] {
		const auto isActionTriggered = InputManager::isActionTriggered("createAndPlugConstructor", EKeyState::Released);
		LOG_INFO("InputManager::isActionTriggered("
		         "\"createAndPlugConstructor\") = {}",
		         isActionTriggered);
	});
	////

	Input.bindAction("TestMouseCtrlAction", EKeyState::Pressed, [] {
		LOG_INFO("bind: (mouse click + left ctrl) pressed");
	});

	Input.bindAction("scrollUp", EKeyState::Pressed, []() {
		LOG_INFO("scrollUp");
	});

	Input.bindAction("scrollDown", EKeyState::Pressed, []() {
		LOG_INFO("scrollDown");
	});

	Input.bindAxis("scroll", [](float val) {
		LOG_INFO("scroll: {}", val);
	});
#endif
}

void LogWindow::render()
{
	ImGui::PushStyleColor(ImGuiCol_TabActive, App::get().getUI()->getTheme().get(EColor::DockTabActive));
	ImGui::Begin(setName("Log View").c_str(), getShowPtr());
	ImGui::PopStyleColor();

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
