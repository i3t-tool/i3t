#include "LogWindow.h"

#include "imgui.h"

#include "Logger/Logger.h"

void LogWindow::render()
{
  ImGui::Begin("Log View", getShowPtr());

  ImGui::Text("Log output");

  ImGui::BeginChild(ID);
  ImGui::TextUnformatted(Logger::getInstance().getBuffer().str().c_str());

  // Set scroll to bottom of the child window.
  if (ImGui::GetScrollY() >= ImGui::GetScrollMaxY())
    ImGui::SetScrollHereY(1.0f);

  ImGui::EndChild();

  ImGui::End();
}
