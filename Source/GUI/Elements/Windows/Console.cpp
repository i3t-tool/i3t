#include "Console.h"

#include "imgui.h"

#include "Commands/ApplicationCommands.h"
#include "Core/Input/InputManager.h"

using namespace UI;

#define MAX_COMMAND_SIZE 1024
char command[MAX_COMMAND_SIZE];
int lineend = 0;
int linebegin = 0;

Console::Console(bool show) : IWindow(show)
{
	Input.addKeyDownFn(Keys::Code::down, [this] { onDownKey(); });
	Input.addKeyDownFn(Keys::Code::up, [this] { onUpKey(); });
}

Console::~Console()
{
}

void Console::render()
{
	ImGui::Begin("Console", getShowPtr());

	// Reserve enough left-over height for 1 separator + 1 input text
	const float footerHeightToReserve = ImGui::GetStyle().ItemSpacing.y + ImGui::GetFrameHeightWithSpacing();
	ImGui::BeginChild("Output", ImVec2{0, -footerHeightToReserve});

	auto& m_buffer = m_stdoutCapture.GetBuffer();

	ImGui::TextUnformatted(m_buffer.str().c_str());

	// Set scroll to bottom of the child window.
	if (ImGui::GetScrollY() >= ImGui::GetScrollMaxY())
		ImGui::SetScrollHereY(1.0f);

	ImGui::EndChild();

	ImGui::Separator();

	// Reclaim focus after pressing enter key.
	bool reclaimFocus = false;
	const ImGuiInputTextFlags inputTextFlags = ImGuiInputTextFlags_EnterReturnsTrue;

	if (InputManager::isKeyJustPressed(Keys::up))
	{
		/*ImGuiIO io = ImGui::GetIO();
		float up = io.KeysDownDuration[ImGuiKey_UpArrow];
		bool up2 = io.KeysDown[ImGuiKey_UpArrow];
		printf("%f %d\n", up, up2);*/

		std::string str = m_buffer.str();

		if (str.size() > 0)
		{
			bool text = false;
			int i = 0;
			while (linebegin >= 0)
			{
				if (i > 0 && str[linebegin] != '\n' && !text)
				{
					text = true;
					lineend = linebegin + 1;
				}
				if (str[linebegin] == '\n' && text)
				{
					linebegin++;
					break;
				}
				i++;
				linebegin--;
			}
			linebegin *= linebegin > 0;
			while (str[linebegin] == '\n' && linebegin < lineend)
			{
				linebegin++;
			}

			memcpy(command, str.c_str() + linebegin, lineend - linebegin);
			command[lineend - linebegin] = '\0';
		}
	}
	if (InputManager::isKeyJustPressed(Keys::down))
	{
		std::string str = m_buffer.str();

		if (str.size() > 0)
		{
			bool text = false;
			int i = 0;
			linebegin = lineend;
			while (lineend < str.size() - 1)
			{
				if (i > 0 && str[lineend] != '\n' && !text)
				{
					text = true;
				}
				if (str[lineend] == '\n' && text)
				{
					break;
				}
				i++;
				lineend++;
			}
			while (str[lineend] == '\n' && linebegin < lineend)
			{
				lineend--;
			}
			lineend++;
			while (str[linebegin] == '\n' && linebegin < lineend)
			{
				linebegin++;
			}

			memcpy(command, str.c_str() + linebegin, lineend - linebegin);
			command[lineend - linebegin] = '\0';
		}
	}
	if (ImGui::InputText("Input", command, 1024, inputTextFlags))
	{
		m_buffer << command << "\n";

		ConsoleCommand::dispatch(command);

		strcpy(command, "");
		// strcpy_s(command, ""); /* 3 arguments?*/
		lineend = (int)m_buffer.str().size() - 1;   // skip \0
		linebegin = (int)m_buffer.str().size() - 1; // skip \0

		reclaimFocus = true;
	}

	// Auto-focus on window apparition
	ImGui::SetItemDefaultFocus();
	if (reclaimFocus)
		ImGui::SetKeyboardFocusHere(-1); // Auto focus previous widget

	ImGui::End();
}

void Console::onUpKey()
{
	std::cout << "UP" << std::endl;
}

void Console::onDownKey()
{
	std::cout << "DOWN" << std::endl;
}
