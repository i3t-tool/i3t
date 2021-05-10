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
}

void LogWindow::render()
{
	ImGui::Begin(getName("Log View").c_str(), getShowPtr());

	ImGui::Text("Log output");

	/// \todo MH remove test code.
  auto cycle = Core::GraphManager::getCycles()[0];
	auto str = fmt::format("Cycle from {} to {} with update step {}.", cycle->getFrom(), cycle->getTo(), cycle->getMultiplier());

	ImGui::Text(str.c_str());
	ImGui::Text("%f", cycle->getData().getFloat());

	if (ImGui::Button("Play")) { cycle->play(); } ImGui::SameLine();
	if (ImGui::Button("Stop")) { cycle->stop(); } ImGui::SameLine();
	if (ImGui::Button("Prev")) { cycle->stepBack(); } ImGui::SameLine();
	if (ImGui::Button("Next")) { cycle->stepNext(); }

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
