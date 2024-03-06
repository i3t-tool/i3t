/**
 * \file
 * \brief
 * \author Martin Herich <martin.herich@phire.cz>
 * \copyright Copyright (C) 2016-2023 I3T team, Department of Computer Graphics
 * and Interaction, FEE, Czech Technical University in Prague, Czech Republic
 *
 * This file is part of I3T - An Interactive Tool for Teaching Transformations
 * http://www.i3t-tool.org
 *
 * GNU General Public License v3.0 (see LICENSE.txt or https://www.gnu.org/licenses/gpl-3.0.txt)
 */
#pragma once

#include <deque>

#define IMGUI_DEFINE_MATH_OPERATORS
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
