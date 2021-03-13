#include "ConsoleWindow.h"

#include "imgui.h"

#include "Commands/ApplicationCommands.h"

char command[1024];

void ConsoleWindow::render()
{
  ImGui::Begin("Console", getShowPtr());

  // Reserve enough left-over height for 1 separator + 1 input text
  const float footerHeightToReserve = ImGui::GetStyle().ItemSpacing.y + ImGui::GetFrameHeightWithSpacing();
  ImGui::BeginChild("Output", ImVec2{0, -footerHeightToReserve});

  ImGui::TextUnformatted(m_buffer.str().c_str());

  // Set scroll to bottom of the child window.
  if (ImGui::GetScrollY() >= ImGui::GetScrollMaxY())
    ImGui::SetScrollHereY(1.0f);

  ImGui::EndChild();

  ImGui::Separator();

  // Reclaim focus after pressing enter key.
  bool reclaimFocus = false;
  const ImGuiInputTextFlags inputTextFlags = ImGuiInputTextFlags_EnterReturnsTrue;
  if (ImGui::InputText("Input", command, 1024, inputTextFlags))
  {
    // CE_LOG_INFO("Command: {}", command);
    m_buffer << command << "\n";

		/// MESSAGE FOR DANIEL!
		/// To catch fired command, use the following anywhere in your code:
		///
		/// void functionWhichProcessConsoleCommands(std::string rawCommand) { ... }
		///
		/// # Bind function to the command.
		/// ConsoleCommand::addListener(&functionWhich...);
		///
		/// # if you need to bind non static member function, use lambda expr.:
		/// ConsoleCommand::addListener([this](std::string c) { ... });
		ConsoleCommand::dispatch(command);

    strcpy_s(command, "");

    reclaimFocus = true;
  }

  // Auto-focus on window apparition
  ImGui::SetItemDefaultFocus();
  if (reclaimFocus)
    ImGui::SetKeyboardFocusHere(-1); // Auto focus previous widget

  ImGui::End();
}
