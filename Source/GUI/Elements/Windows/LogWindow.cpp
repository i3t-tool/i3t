#include "LogWindow.h"

#include "imgui.h"

#include "Core/Nodes/GraphManager.h"
#include "Logger/Logger.h"

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

	ImGui::BeginChild(ID);
	ImGui::TextUnformatted(Logger::getInstance().getBuffer().str().c_str());

	// Set scroll to bottom of the child window.
	if (ImGui::GetScrollY() >= ImGui::GetScrollMaxY())
		ImGui::SetScrollHereY(1.0f);

	ImGui::EndChild();

	ImGui::End();
}
