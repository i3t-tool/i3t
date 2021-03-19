#include "Console.h"

#include "imgui.h"

#include "Commands/ApplicationCommands.h"
#include "Core/Input/InputManager.h"

using namespace UI;

#define MAX_COMMAND_SIZE 1024
char command[MAX_COMMAND_SIZE];
std::vector<glm::ivec2> commands;
int selected=0;

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

	if (ImGui::InputText("Input", command, 1024, inputTextFlags))
	{
		commands.push_back(glm::ivec2(m_buffer.str().size(), strlen(command)));
		selected=commands.size();
		m_buffer << command << "\n";

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

void Console::onUpKey()
{
	//std::cout << "UP" << std::endl;
	if(commands.size()>0){
		std::string str = m_stdoutCapture.GetBuffer().str();
		selected--;
		if(selected<0){selected=0;}
		else if(selected>=commands.size()){selected=commands.size()-1; }
		std::string ss=str.substr(commands[selected][0], commands[selected][1]);
		memcpy(command, ss.c_str(), ss.size());
		command[ss.size()] = '\0';

		printf("up   %d,%d,,<%s>\n", commands[selected][0], commands[selected][1], command);
	}
	
	render();
}

void Console::onDownKey()
{

	if (commands.size() > 0) {
		std::string str = m_stdoutCapture.GetBuffer().str();
		selected++;
		if (selected < 0) { selected = 0; }
		else if (selected >= commands.size()) { 
			selected = commands.size() - 1;
			command[0] = '\0';
		}
		else{
			std::string ss = str.substr(commands[selected][0], commands[selected][1]);
			memcpy(command, ss.c_str(), ss.size());
			command[ss.size()] = '\0';
		}

		printf("down %d,%d,,<%s>\n", commands[selected][0], commands[selected][1], command);
	}

	render();
}
