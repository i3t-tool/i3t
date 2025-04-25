/**
 * \file
 * \brief
 * \author Dan Rakušan <rakusan.dan@gmail.com>
 * \copyright Copyright (C) 2016-2023 I3T team, Department of Computer Graphics
 * and Interaction, FEE, Czech Technical University in Prague, Czech Republic
 *
 * This file is part of I3T - An Interactive Tool for Teaching Transformations
 * http://www.i3t-tool.org
 *
 * GNU General Public License v3.0 (see LICENSE.txt or https://www.gnu.org/licenses/gpl-3.0.txt)
 */
#include "Toolkit.h"

#include "I3TGui.h"

#include <math.h>

#include "Utils/Format.h"

namespace GUI
{
struct InputTextCallback_UserData
{
	std::string* Str;
	ImGuiInputTextCallback ChainCallback;
	void* ChainCallbackUserData;
};

static int InputTextCallback(ImGuiInputTextCallbackData* data)
{
	InputTextCallback_UserData* user_data = (InputTextCallback_UserData*) data->UserData;
	if (data->EventFlag == ImGuiInputTextFlags_CallbackResize)
	{
		// Resize string callback
		// If for some reason we refuse the new length (BufTextLen) and/or capacity (BufSize) we need to set them back
		// to what we want.
		std::string* str = user_data->Str;
		IM_ASSERT(data->Buf == str->c_str());
		str->resize(data->BufTextLen);
		data->Buf = (char*) str->c_str();
	}
	else if (user_data->ChainCallback)
	{
		// Forward to user callback, if any
		data->UserData = user_data->ChainCallbackUserData;
		return user_data->ChainCallback(data);
	}
	return 0;
}

//

bool InputText(const char* label, std::string* str, ImGuiInputTextFlags flags, ImGuiInputTextCallback callback,
               void* user_data)
{
	IM_ASSERT((flags & ImGuiInputTextFlags_CallbackResize) == 0);
	flags |= ImGuiInputTextFlags_CallbackResize;

	InputTextCallback_UserData cb_user_data;
	cb_user_data.Str = str;
	cb_user_data.ChainCallback = callback;
	cb_user_data.ChainCallbackUserData = user_data;

	const auto formattedLabel = "##" + std::string(label);

	return ImGui::InputText(formattedLabel.c_str(), (char*) str->c_str(), str->capacity() + 1, flags, InputTextCallback,
	                        &cb_user_data);
}

bool Button(const char* id, bool disabled, const ImVec2& size)
{
	if (disabled)
	{
		ImGui::PushItemFlag(ImGuiItemFlags_Disabled, true);
		ImGui::PushStyleVar(ImGuiStyleVar_Alpha, ImGui::GetStyle().Alpha * 0.5f);
	}

	const auto result = I3TGui::ButtonWithLog(id, size);

	if (disabled)
	{
		ImGui::PopItemFlag();
		ImGui::PopStyleVar();
	}

	return result;
}

bool ToggleButton(const char* label, bool& toggled, bool invert, ImVec2 size)
{
	bool pressed = false;
	int colorsPushed = 0;
	bool state = invert ? !toggled : toggled;
	if (state)
	{
		ImGui::PushStyleColor(ImGuiCol_Button, I3T::getTheme().get(EColor::CheckMark));
		colorsPushed++;
	}
	else
	{
		ImGui::PushStyleColor(ImGuiCol_Text, ImGui::GetStyleColorVec4(ImGuiCol_TextDisabled));
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, I3T::getTheme().get(EColor::TutorialButtonBg));
		colorsPushed = colorsPushed + 2;
	}
	if (I3TGui::ButtonWithLog(label, size))
	{
		pressed = true;
		toggled = !toggled;
	}
	ImGui::PopStyleColor(colorsPushed);
	return pressed;
}

bool DimButton(const char* label, const ImVec2& size)
{
	bool pressed = false;
	ImGui::PushStyleColor(ImGuiCol_Button, I3T::getTheme().get(EColor::ButtonDim));
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, I3T::getTheme().get(EColor::ButtonDimHovered));
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, I3T::getTheme().get(EColor::ButtonDimActive));
	if (I3TGui::ButtonWithLog(label, size))
	{
		pressed = true;
	}
	ImGui::PopStyleColor(3);
	return pressed;
}

bool DimButtonDark(const char* label, const ImVec2& size)
{
	bool pressed = false;
	ImGui::PushStyleColor(ImGuiCol_Button, I3T::getTheme().get(EColor::ButtonDimDark));
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, I3T::getTheme().get(EColor::ButtonDimHovered));
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, I3T::getTheme().get(EColor::ButtonDimActive));
	if (I3TGui::ButtonWithLog(label, size))
	{
		pressed = true;
	}
	ImGui::PopStyleColor(3);
	return pressed;
}

bool FloatingButton(const char* label, const ImVec2& size)
{
	ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, I3T::getTheme().getBorderSize());
	ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, I3T::getTheme().get(ESize::FloatingButtonRounding));
	bool pressed = DimButton(label, size);
	ImGui::PopStyleVar(2);
	return pressed;
}

bool FloatingButtonDark(const char* label, const ImVec2& size, float rounding)
{
	ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 0.f);
	ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, rounding);
	bool pressed = DimButtonDark(label, size);
	ImGui::PopStyleVar(2);
	return pressed;
}

bool FloatingToggleButton(const char* label, bool& toggled, bool invert, const ImVec2& size)
{
	ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, I3T::getTheme().getBorderSize());
	ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, I3T::getTheme().get(ESize::FloatingButtonRounding));
	bool pressed = false;
	int colorsPushed = 0;
	bool state = invert ? !toggled : toggled;
	if (state)
	{
		ImGui::PushStyleColor(ImGuiCol_Button, I3T::getTheme().get(EColor::ButtonDimToggled));
		colorsPushed++;
	}
	else
	{
		ImGui::PushStyleColor(ImGuiCol_Button, I3T::getTheme().get(EColor::ButtonDim));
		ImGui::PushStyleColor(ImGuiCol_Text, ImGui::GetStyleColorVec4(ImGuiCol_TextDisabled));
		colorsPushed += 2;
	}
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, I3T::getTheme().get(EColor::ButtonDimHovered));
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, I3T::getTheme().get(EColor::ButtonDimActive));
	if (I3TGui::ButtonWithLog(label, size))
	{
		pressed = true;
		toggled = !toggled;
	}
	ImGui::PopStyleColor(2);
	ImGui::PopStyleColor(colorsPushed);
	ImGui::PopStyleVar(2);
	return pressed;
}

bool ButtonWithCorners(const char* label, ImDrawFlags corners, const ImVec2& size_arg)
{
	return ButtonWithCornersEx(label, corners, size_arg, ImGuiButtonFlags_None);
}

bool ButtonWithCornersEx(const char* label, ImDrawFlags corners, const ImVec2& size_arg, ImGuiButtonFlags flags)
{
	using namespace ImGui;

	ImGuiWindow* window = ImGui::GetCurrentWindow();
	if (window->SkipItems)
		return false;

	ImGuiContext& g = *GImGui;
	const ImGuiStyle& style = g.Style;
	const ImGuiID id = window->GetID(label);
	const ImVec2 label_size = CalcTextSize(label, NULL, true);

	ImVec2 pos = window->DC.CursorPos;
	if ((flags & ImGuiButtonFlags_AlignTextBaseLine) &&
	    style.FramePadding.y <
	        window->DC.CurrLineTextBaseOffset) // Try to vertically align buttons that are smaller/have no padding so
	                                           // that text baseline matches (bit hacky, since it shouldn't be a flag)
		pos.y += window->DC.CurrLineTextBaseOffset - style.FramePadding.y;
	ImVec2 size =
	    CalcItemSize(size_arg, label_size.x + style.FramePadding.x * 2.0f, label_size.y + style.FramePadding.y * 2.0f);

	const ImRect bb(pos, pos + size);
	ItemSize(size, style.FramePadding.y);
	if (!ItemAdd(bb, id))
		return false;

	if (g.LastItemData.InFlags & ImGuiItemFlags_ButtonRepeat)
		flags |= ImGuiButtonFlags_Repeat;

	bool hovered, held;
	bool pressed = ButtonBehavior(bb, id, &hovered, &held, flags);

	// Render
	const ImU32 col = GetColorU32((held && hovered) ? ImGuiCol_ButtonActive
	                              : hovered         ? ImGuiCol_ButtonHovered
	                                                : ImGuiCol_Button);
	RenderNavHighlight(bb, id);
	RenderFrameWithCorners(bb.Min, bb.Max, col, true, style.FrameRounding, corners);

	if (g.LogEnabled)
		LogSetNextTextDecoration("[", "]");
	RenderTextClipped(bb.Min + style.FramePadding, bb.Max - style.FramePadding, label, NULL, &label_size,
	                  style.ButtonTextAlign, &bb);

	// Automatically close popups
	// if (pressed && !(flags & ImGuiButtonFlags_DontClosePopups) && (window->Flags & ImGuiWindowFlags_Popup))
	//    CloseCurrentPopup();

	IMGUI_TEST_ENGINE_ITEM_INFO(id, label, g.LastItemData.StatusFlags);
	return pressed;
}

void RenderFrameWithCorners(ImVec2 p_min, ImVec2 p_max, ImU32 fill_col, bool border, float rounding,
                            ImDrawFlags corners)
{
	using namespace ImGui;

	ImGuiContext& g = *GImGui;
	ImGuiWindow* window = g.CurrentWindow;
	window->DrawList->AddRectFilled(p_min, p_max, fill_col, rounding, corners);
	const float border_size = g.Style.FrameBorderSize;
	if (border && border_size > 0.0f)
	{
		window->DrawList->AddRect(p_min + ImVec2(1, 1), p_max + ImVec2(1, 1), GetColorU32(ImGuiCol_BorderShadow),
		                          rounding, corners, border_size);
		window->DrawList->AddRect(p_min, p_max, GetColorU32(ImGuiCol_Border), rounding, corners, border_size);
	}
}

bool SliderFloatStepped(const char* label, float* v, float step, float v_min, float v_max, const std::string& format)
{
	const int precisionInv = std::lround(1.0 / step);
	int tmpIntVal = std::lroundf(*v * precisionInv);

	if (ImGui::SliderInt(label, &tmpIntVal, std::lroundf(v_min * precisionInv), std::lroundf(v_max * precisionInv),
	                     fmt::vformat(format, fmt::make_format_args(*v)).c_str(),
	                     ImGuiSliderFlags_NoInput | ImGuiSliderFlags_NoRoundToFormat))
	{
		*v = static_cast<float>(tmpIntVal) / precisionInv;
		return true;
	}
	return false;
}

bool DrawFloat(const std::string& label, float& value, int numberOfVisibleDecimals, Core::EValueState const& valueState,
               bool& valueChanged)
{
	bool inactive = (valueState == Core::EValueState::Locked || valueState == Core::EValueState::LockedSyn);
	bool synergies = (valueState == Core::EValueState::EditableSyn || valueState == Core::EValueState::LockedSyn);
	bool inner_interaction_happen = false, valueChangedByPopup = false;

	if (synergies)
	{
		ImGui::PushStyleColor(ImGuiCol_FrameBg, I3T::getColor(EColor::Synergies_FloatBg));
		ImGui::PushStyleColor(ImGuiCol_FrameBgHovered, I3T::getColor(EColor::Synergies_FloatBgHovered));
		ImGui::PushStyleColor(ImGuiCol_FrameBgActive, I3T::getColor(EColor::Synergies_FloatBgActive));
	}
	if (inactive)
	{
		ImGui::PushItemFlag(ImGuiItemFlags_Disabled, true);
		ImGui::PushStyleVar(ImGuiStyleVar_Alpha,
		                    ImGui::GetStyle().Alpha * I3T::getSize(ESize::Float_inactive_alphaMultiplicator));
	}

	float step = I3T::getSize(ESize::Nodes_dragSpeedDefaultRatio);

	valueChanged =
	    ImGui::DragFloat(label.c_str(), &value, step, 0.0f, 0.0f, fmt::format("%.{}f", numberOfVisibleDecimals).c_str(),
	                     1.0f); /* if power >1.0f the number changes logarithmic */

	if (ImGui::IsItemActive())
		inner_interaction_happen = true;

	if (inactive)
	{
		ImGui::PopItemFlag();
		ImGui::PopStyleVar();
	}

	if (synergies)
	{
		ImGui::PopStyleColor(3);
	}

	return inner_interaction_happen || valueChanged;
}

int numberOfCharWithDecimalPoint(float value, int numberOfVisibleDecimal)
{
	int border = 10, result = 1, int_value;

	if (value < 0)
	{
		value = -value;
	}
	result++; /* always space for sign to avoid changing size of / alternatively
	             move it inside if above */

	int_value = (int) value;
	while (int_value >= border)
	{
		result++;
		border *= 10;
	}

	return result + (numberOfVisibleDecimal > 0 ? numberOfVisibleDecimal + 1 : 0); /* +1 for decimal point */
}

int maxLengthOfData4x4(const glm::mat4& data, int numberOfVisibleDecimal)
{
	int act, maximal = 0;
	for (int column = 0; column < 4; column++)
	{
		for (int row = 0; row < 4; row++)
		{
			act = numberOfCharWithDecimalPoint(data[column][row], numberOfVisibleDecimal);
			if (act > maximal)
			{
				maximal = act;
			}
		}
	}
	return maximal;
}

bool DrawMatrix(const char* label, const glm::mat4& data, int numberOfVisibleDecimals,
                const std::array<std::array<Core::EValueState, 4> const, 4>& dataState, bool& valueChanged,
                int& rowOfChange, int& columnOfChange, float& valueOfChange)
{
	float dataWidth = maxLengthOfData4x4(data, numberOfVisibleDecimals);

	bool inner_interaction_happen = false;
	bool actualValueChanged = false;
	float localData; /* user can change just one value at the moment */

	ImGui::PushItemWidth(dataWidth);
	ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, I3T::getSize(ESizeVec2::Nodes_FloatPadding));
	ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, I3T::getSize(ESizeVec2::Nodes_ItemsSpacing));
	valueChanged = false;

	/* Drawing is row-wise */
	ImGui::BeginGroup();
	for (int rows = 0; rows < 4; rows++)
	{
		// To be quite honest I don't know why this call is necessary but window text baseline offset is alwazs set here
		// to ~4 (FramePadding.y) when it really should be 0, causes any text to be too high and then it works fine
		// after the next ImGui::SameLine() call. This created a vertical gap between the first and second matrix rows.
		// JH solved this (haphazardly like me) by moving the Y cursor position by ItemSpacing which however broke down
		// at other zoom levels.
		ImGui::GetCurrentWindow()->DC.CurrLineTextBaseOffset = 0.0f;
		for (int columns = 0; columns < 4; columns++)
		{
			localData = data[columns][rows]; /* Data are column-wise */

			inner_interaction_happen |=
			    DrawFloat(fmt::format("##{}:row-{},col-{}", label, rows, columns), localData, numberOfVisibleDecimals,
			              dataState[rows][columns], actualValueChanged);

			if (actualValueChanged)
			{
				valueChanged = true;
				columnOfChange = columns;
				rowOfChange = rows;
				valueOfChange = localData;
			}

			if (columns < 3)
			{
				ImGui::SameLine();
			}
		}
	}
	ImGui::EndGroup();

	ImGui::PopStyleVar();
	ImGui::PopStyleVar();
	ImGui::PopItemWidth();

	return inner_interaction_happen;
}

void drawCross(glm::vec2 pos, ImDrawList* drawList, float thickness, float size, ImColor color)
{
	drawList->AddRectFilled(ImVec2(pos.x - floor(thickness / 2), pos.y - floor(size / 2)),
	                        ImVec2(pos.x + ceil(thickness / 2), pos.y + ceil(size / 2)), color);
	drawList->AddRectFilled(ImVec2(pos.x - floor(size / 2), pos.y - floor(thickness / 2)),
	                        ImVec2(pos.x + ceil(size / 2), pos.y + ceil(thickness / 2)), color);
}

void drawEllipse(float cx, float cy, float rx, float ry, int num_segments, ImDrawList* drawList, ImColor color,
                 float thickness)
{
	// Adapted from https://stackoverflow.com/a/34735255/3452003
	std::vector<ImVec2> points;
	points.reserve(num_segments);

	float theta = 2 * I3_PI / float(num_segments);
	float c = cosf(theta); // precalculate the sine and cosine
	float s = sinf(theta);
	float t;

	float x = 1; // we start at angle = 0
	float y = 0;

	for (int ii = 0; ii < num_segments; ii++)
	{
		// apply radius and offset
		points.push_back({x * rx + cx, y * ry + cy});

		// apply the rotation matrix
		t = x;
		x = c * x - s * y;
		y = s * t + c * y;
	}

	drawList->AddPolyline(&points[0], points.size(), color, ImDrawFlags_Closed, thickness);
}

void dockTabStylePush()
{
	ImGui::PushStyleColor(ImGuiCol_TabHovered, I3T::getUI()->getTheme().get(EColor::DockTabHovered));
	ImGui::PushStyleColor(ImGuiCol_Tab, I3T::getUI()->getTheme().get(EColor::DockTab));
	ImGui::PushStyleColor(ImGuiCol_TabActive, I3T::getUI()->getTheme().get(EColor::DockTabActive));
	ImGui::PushStyleColor(ImGuiCol_TabUnfocused, I3T::getUI()->getTheme().get(EColor::DockTabUnfocused));
	ImGui::PushStyleColor(ImGuiCol_TabUnfocusedActive, I3T::getUI()->getTheme().get(EColor::DockTabUnfocusedActive));
}

void dockTabStylePop()
{
	ImGui::PopStyleColor(5);
}

} // namespace GUI
