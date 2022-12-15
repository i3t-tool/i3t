#include "Console.h"

#include "imgui.h"

#include "Commands/ApplicationCommands.h"
#include "Core/API.h"
#include "Core/Input/InputManager.h"
#include "GUI/Elements/IWindow.h"

using namespace UI;

#define MAX_COMMAND_SIZE 1024
char command[MAX_COMMAND_SIZE];
// char command2[MAX_COMMAND_SIZE];
std::vector<glm::ivec2> commands;
int selected = 0;

Console::Console(bool show) : IWindow(show)
{
	/*InputManager::setInputAction("console_up", Keys::Code::up);
	InputManager::setInputAction("console_down", Keys::Code::down);

	Input.bindAction("console_up", EKeyState::Released, [this] { onUpKey(); });
	Input.bindAction("console_down", EKeyState::Pressed, [this] { onDownKey(); });
	*/
}

int history(ImGuiInputTextCallbackData* d)
{
	if (d->EventKey == ImGuiKey_UpArrow)
	{
		Ptr<Console> console = I3T::getWindowPtr<Console>();
		console->onUpKey();
	}
	else if (d->EventKey == ImGuiKey_DownArrow)
	{
		Ptr<Console> console = I3T::getWindowPtr<Console>();
		console->onDownKey();
	}
	d->DeleteChars(0, d->BufTextLen);
	d->InsertChars(0, command);
	// printf("command %s\n", command2);
	return 1;
}
void Console::render()
{
	ImGui::PushStyleColor(ImGuiCol_TabActive, App::get().getUI()->getTheme().get(EColor::DockTabActive));
	ImGui::Begin(setName("Console").c_str(), getShowPtr());
	ImGui::PopStyleColor();

	// Reserve enough left-over height for 1 separator + 1 input text
	const float footerHeightToReserve = ImGui::GetStyle().ItemSpacing.y + ImGui::GetFrameHeightWithSpacing();
	ImGui::BeginChild("Output", ImVec2{0, -footerHeightToReserve});

	std::stringstream& m_buffer = m_stdoutCapture.GetBuffer();

	ImGui::TextUnformatted(m_buffer.str().c_str());

	// Set scroll to bottom of the child window.
	if (ImGui::GetScrollY() >= ImGui::GetScrollMaxY())
		ImGui::SetScrollHereY(1.0f);

	ImGui::EndChild();

	ImGui::Separator();

	const ImGuiInputTextFlags inputTextFlags = ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_CallbackHistory;
	// ImGuiInputTextCallback in=[this]{return this->h();};

	if (ImGui::InputText("Input", command, 1024, inputTextFlags, &history))
	{
		bool nonwhite = false;
		int len = (int)strlen(command);
		for (int i = 0; i < len; i++)
		{
			if (command[i] != '\t' && command[i] != ' ')
			{
				nonwhite = true;
				break;
			}
		}
		if (nonwhite)
		{
			commands.push_back(glm::ivec2(m_buffer.str().size(), len));
		}
		selected = (int)commands.size();

		m_buffer << command << "\n";

		ConsoleCommand::dispatch(command);

		strcpy(command, "");
		ImGui::SetKeyboardFocusHere(-1);
	}

	ImGui::End();
}

void Console::onUpKey()
{
	if (commands.size() > 0)
	{
		std::string str = m_stdoutCapture.GetBuffer().str();
		selected--;
		if (selected < 0)
		{
			selected = 0;
		}
		else if (selected >= commands.size())
		{
			selected = (int)commands.size() - 1;
		}
		std::string ss = str.substr(commands[selected][0], commands[selected][1]);

		memcpy(command, ss.c_str(), ss.size());
		command[ss.size()] = '\0';
		// strcpy(command2, command);
		// printf("up   %d/%lld,,<%s>\n", selected, commands.size()-1, command2);
	}
}

void Console::onDownKey()
{
	if (commands.size() > 0)
	{
		std::string str = m_stdoutCapture.GetBuffer().str();
		selected++;
		if (selected < 0)
		{
			selected = 0;
		}
		else if (selected >= commands.size())
		{
			command[0] = '\0';
		}
		else
		{
			std::string ss = str.substr(commands[selected][0], commands[selected][1]);
			memcpy(command, ss.c_str(), ss.size());
			command[ss.size()] = '\0';
		}
		// strcpy(command2, command);
		// printf("down %d/%lld,,<%s>\n", selected, commands.size() - 1, command2);
	}
}
