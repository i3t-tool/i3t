#include "LogWindow.h"

#include "imgui.h"

#include "Core/Nodes/GraphManager.h"
#include "Logger/Logger.h"

LogWindow::LogWindow()
{
#ifdef I3T_DEBUG
	Input.bindAction("MyTestAction", EKeyState::Pressed, [] { Log::info("MyTestAction triggered!"); });

	Input.bindAxis("MyTestAxis", [](float val) { Log::info("MyTestAxis triggered: {}!", val); });

	////
	Input.bindAction("createAndPlugConstructor", EKeyState::Released,
	                 []
	                 {
		                 const auto isActionTriggered =
		                     InputManager::isActionTriggered("createAndPlugConstructor", EKeyState::Released);
		                 Log::info("InputManager::isActionTriggered("
		                           "\"createAndPlugConstructor\") = {}",
		                           isActionTriggered);
	                 });
	////

	Input.bindAction("TestMouseCtrlAction", EKeyState::Pressed,
	                 [] { Log::info("bind: (mouse click + left ctrl) pressed"); });

	Input.bindAction("scrollUp", EKeyState::Pressed, []() { Log::info("scrollUp"); });

	Input.bindAction("scrollDown", EKeyState::Pressed, []() { Log::info("scrollDown"); });

	Input.bindAxis("scroll", [](float val) { Log::info("scroll: {}", val); });
#endif
}

void LogWindow::render()
{
	ImGui::Begin(getName("Log View").c_str(), getShowPtr());

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
		Log::info("query: (mouse click + left ctrl) pressed");
	}

	ImGui::EndChild();

	ImGui::End();
}
