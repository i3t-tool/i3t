/**
 * \author Martin Herich <martin.herich@phire.cz>
 */
#pragma once

#include <deque>

#include "imgui.h"

#include "GUI/Elements/IWindow.h"
#include "Utils/Other.h"

namespace UI
{
/**
 * The stdout is redirected to the console.
 */
class Console : public IWindow
{
public:
	I3T_WINDOW(Console)

	/// \pre Must be instantiated after the ScriptingModule!
	Console(bool show);
	~Console() override = default;
	void render() override;

	int textEditCallback(ImGuiInputTextCallbackData* data);

private:
	void drawOutput();
	void drawInput();
	void execute();

	std::ostringstream& m_oss;

	/// Modified by DrawInput on command submit.
	bool m_ShouldSetFocus = false;

	float m_InputHeight = 0.0f;

	//

	std::string m_Command;
	int m_HistoryPos;
	std::vector<std::string> m_History;
};
} // namespace UI
