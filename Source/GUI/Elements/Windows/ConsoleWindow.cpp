#include "ConsoleWindow.h"

#include "imgui.h"

void ConsoleWindow::render()
{
	ImGui::Begin("Console", getShowPtr());
	ImGui::Text("My console!");
	ImGui::End();
}
