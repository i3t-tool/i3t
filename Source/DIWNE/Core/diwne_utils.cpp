/**
 * \file
 * \brief
 * \author Dan Rakušan <rakusan.dan@gmail.com>
 * \copyright Copyright (C) 2016-2025 I3T team, Department of Computer Graphics
 * and Interaction, FEE, Czech Technical University in Prague, Czech Republic
 *
 * This file is part of I3T - An Interactive Tool for Teaching Transformations
 * http://www.i3t-tool.org
 *
 * GNU General Public License v3.0 (see LICENSE.txt or https://www.gnu.org/licenses/gpl-3.0.txt)
 */
#include "diwne_utils.h"

#include <string>

namespace DIWNE
{
namespace DDraw
{
void drawSpringDebug(const ImVec2& pos, ImVec2 springSize, bool horizontal)
{
	int handleSize = 14;
	int barSize = 2;
	if (horizontal)
	{
		ImVec2 start = pos + ImVec2(0, 4);
		ImVec2 end = ImVec2(start.x + springSize.x, start.y);
		ImGui::GetForegroundDrawList()->AddLine(start, ImVec2(start.x, start.y + handleSize), IM_COL32(255, 0, 0, 255));
		ImGui::GetForegroundDrawList()->AddLine(end, ImVec2(end.x, end.y + handleSize), IM_COL32(255, 0, 0, 255));
		int bOff = handleSize / 2 - barSize;
		ImGui::GetForegroundDrawList()->AddLine(ImVec2(start.x, start.y + bOff), ImVec2(end.x, end.y + bOff),
		                                        IM_COL32(255, 0, 0, 255));
		bOff = handleSize / 2 + barSize;
		ImGui::GetForegroundDrawList()->AddLine(ImVec2(start.x, start.y + bOff), ImVec2(end.x, end.y + bOff),
		                                        IM_COL32(255, 0, 0, 255));
		ImGui::GetForegroundDrawList()->AddLine(ImVec2(start.x, start.y + bOff), ImVec2(end.x, end.y + bOff),
		                                        IM_COL32(255, 0, 0, 255));
		ImGui::GetForegroundDrawList()->AddText(start + ImVec2(springSize.x / 2, handleSize + 2),
		                                        IM_COL32(217, 50, 50, 255), std::to_string(springSize.x).c_str());
	}
	else
	{
		ImVec2 start = pos + ImVec2(4, 0);
		ImVec2 end = ImVec2(start.x, start.y + springSize.y);
		ImGui::GetForegroundDrawList()->AddLine(start, ImVec2(start.x + handleSize, start.y),
		                                        IM_COL32(255, 102, 0, 255));
		ImGui::GetForegroundDrawList()->AddLine(end, ImVec2(end.x + handleSize, end.y), IM_COL32(255, 102, 0, 255));
		int bOff = handleSize / 2 - barSize;
		ImGui::GetForegroundDrawList()->AddLine(ImVec2(start.x + bOff, start.y), ImVec2(end.x + bOff, end.y),
		                                        IM_COL32(255, 102, 0, 255));
		bOff = handleSize / 2 + barSize;
		ImGui::GetForegroundDrawList()->AddLine(ImVec2(start.x + bOff, start.y), ImVec2(end.x + bOff, end.y),
		                                        IM_COL32(255, 102, 0, 255));
		ImGui::GetForegroundDrawList()->AddText(start + ImVec2(handleSize + 2, springSize.y / 2),
		                                        IM_COL32(227, 122, 52, 255), std::to_string(springSize.y).c_str());
	}
}
} // namespace DDraw
} // namespace DIWNE