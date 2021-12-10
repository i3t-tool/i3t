#include "LogWindow.h"

#include "imgui.h"

#include "Core/Nodes/GraphManager.h"
#include "Logger/Logger.h"

LogWindow::LogWindow()
{
	Input.bindAction("MyTestAction", EKeyState::Pressed, [] {
    Log::info("MyTestAction triggered!");
	});

	Input.bindAxis("MyTestAxis", [](float val) {
	  Log::info("MyTestAxis triggered: {}!", val);
  });

	Input.bindAction("scrollUp", EKeyState::Pressed, []() {
		Log::info("scrollUp");
	});

	Input.bindAction("scrollDown", EKeyState::Pressed, []() {
		Log::info("scrollDown");
	});

	Input.bindAxis("scroll", [](float val) {
		Log::info("scroll: {}", val);
	});
}

void LogWindow::render()
{
	ImGui::Begin(getName("Log View").c_str(), getShowPtr());

	ImGui::Text("Log output");

	/// \todo MH test code, remove
  static bool val = true;

  ImGui::Text("Switch fire action key"); ImGui::SameLine();
  std::string s;
  for (auto action : InputBindings::getActionMapping("fire"))
  {
		s += std::to_string(action.code);
	}
	ImGui::Text("Current keys: %s", s.c_str());
	if (ImGui::Button("Switch"))
  {
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
	}

	ImGui::BeginChild(ID);
	ImGui::TextUnformatted(Logger::getInstance().getBuffer().str().c_str());

	// Set scroll to bottom of the child window.
	if (ImGui::GetScrollY() >= ImGui::GetScrollMaxY())
		ImGui::SetScrollHereY(1.0f);

	ImGui::EndChild();

	ImGui::End();
}
