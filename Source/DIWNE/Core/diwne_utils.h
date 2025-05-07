#pragma once

#include "diwne_imgui.h"

#include <algorithm>
#include <cmath>
#include <string>

// This util header should NOT include DIWNE specific classes as it should be easily usable outside of DIWNE.

#define DIWNE_PIXEL_EPSILON 0.001f

#define DIWNE_TRUNC(val) IM_TRUNC(val)
#define DIWNE_TRUNC_VEC(val) ImVec2(IM_TRUNC(val.x), IM_TRUNC(val.y))
#define DIWNE_TRUNC_POS() DIWNE::DGui::truncScreenCursorPos()

// #define DIWNE_MIN_TRUNC(_VAL) ((float) (int) (_VAL < 0 ? _VAL - 1 : _VAL))
// #define DIWNE_MIN_TRUNC_VEC() ImVec2(DIWNE_MIN_TRUNC(val.x), DIWNE_MIN_TRUNC(val.y))

namespace DIWNE
{
namespace DGui
{
inline ImVec2 truncScreenCursorPos()
{
	// Snaps the screen cursor position to lower pixel boundary
	// The idea is that to properly measure size of ImGui elements (ImGui groups),
	// we need to ensure they begin at a rounded position, to make sure their new line begins at the same position.
	ImVec2 pos = ImGui::GetCursorScreenPos();
	ImGui::SetCursorScreenPos(DIWNE_TRUNC_VEC(pos));
	return pos - ImGui::GetCursorScreenPos(); // Returns the error
}
inline void BeginGroup()
{
	// We align cursor position to pixel boundaries BEFORE starting the group
	// This ensures the group measures its size correctly
	// See https://github.com/ocornut/imgui/issues/8527
	DIWNE_TRUNC_POS();
	ImGui::BeginGroup();
}
/// Ensures a new ImGui line and moves the cursor so that no vertical item spacing is applied, or just a fraction of it.
/// @param vSpacingFactor The factor of spacing applied, 0 by default (no spacing).
inline void NewLine(float vSpacingFactor = 0.f)
{
	ImGuiWindow* window = ImGui::GetCurrentWindowRead();
	if (window->SkipItems)
		return;
	if (window->DC.IsSameLine)
		ImGui::NewLine();
	float offH = ImGui::GetStyle().ItemSpacing.y * (vSpacingFactor == 0.f ? 1.f : (1.f - vSpacingFactor));
	ImGui::SetCursorPosY(ImGui::GetCursorPosY() - offH);
}
/// Limits the width of an ImGui line to not exceed a particular X coordinate.
inline void LimitLine(float newMaxX)
{
	ImGuiContext& g = *GImGui;
	ImGuiWindow* window = g.CurrentWindow;
	if (window->SkipItems)
		return;

	float diff = window->DC.CursorPosPrevLine.x - newMaxX;
	if (diff > 0)
	{
		window->DC.CursorPosPrevLine.x = newMaxX;
		window->DC.CursorMaxPos.x = newMaxX;
	}
}
inline void SameLineDummy(const ImVec2& size)
{
	ImGui::Dummy(size);
	ImGui::SameLine(0, 0);
}
/// Dummy that effectively moves the cursor by exactly the passed size in the X and Y direction.
inline void DummyXY(const ImVec2& size)
{
	ImGuiWindow* window = ImGui::GetCurrentWindowRead();
	if (window->SkipItems)
		return;
	ImGui::Dummy(size);
	ImGui::SetCursorScreenPos(ImGui::GetItemRectMax());
}
inline void DummyXYSameLine(const ImVec2& size)
{
	ImGuiWindow* window = ImGui::GetCurrentWindowRead();
	if (window->SkipItems)
		return;
	ImGui::Dummy(size);
	ImGui::SetCursorScreenPos(ImGui::GetItemRectMax());
	ImGui::SameLine(0, 0);
}
/// Dummy that begins at the max cursor position. Expands current content by the size and starts a new line.
inline void DummyMax(const ImVec2& size)
{
	ImGuiWindow* window = ImGui::GetCurrentWindowRead();
	if (window->SkipItems)
		return;
	ImGui::SetCursorScreenPos(window->DC.CursorMaxPos);
	ImGui::Dummy(size);
	NewLine();
}
/// Dummy that begins at the max cursor position. Expands current content by the size and continues the line
inline void DummyMaxSameLine(const ImVec2& size)
{
	DummyMax(size);
	ImGui::SameLine(0, 0);
}
/// Dummy that begins at the max cursor position. Expands current content by the size and starts a new line with no
/// vertical spacing, can be used to add trailing padding.
// inline void PaddingMax(const ImVec2& size)
// {
// 	DummyMax(size);
// 	NewLine();
// }
inline void BeginVerticalAlign(float yOffset)
{
	ImGui::BeginGroup();
	if (yOffset > 0.0f)
		DummyXY(ImVec2(0.0f, yOffset));
}
inline void EndVerticalAlign(float yOffset = 0.0f)
{
	NewLine();
	if (yOffset > 0.0f)
		DummyXY(ImVec2(0.0f, yOffset));
	ImGui::EndGroup();
}
/// Variant of ImGui::AlignTextToFramePadding for custom frame padding
inline void AlignTextToPadding(ImVec2 padding)
{
	ImGuiWindow* window = ImGui::GetCurrentWindow();
	if (window->SkipItems)
		return;

	ImGuiContext& g = *GImGui;
	window->DC.CurrLineSize.y = ImMax(window->DC.CurrLineSize.y, g.FontSize + padding.y * 2);
	window->DC.CurrLineTextBaseOffset = ImMax(window->DC.CurrLineTextBaseOffset, padding.y);
}
/// Variant of ImGui::GetFrameHeight() for custom frame padding
inline float GetFrameHeight(ImVec2 padding)
{
	ImGuiContext& g = *GImGui;
	return g.FontSize + padding.y * 2.0f;
}
/// Invisible button that can optionally be disabled to allow drag operations with no active id
inline bool InvisibleButton(const std::string& id, const ImVec2& size, bool disabled, bool& hovered, bool& active)
{
	ImGuiContext& g = *ImGui::GetCurrentContext();
	bool wasDisabled = (g.CurrentItemFlags & ImGuiItemFlags_Disabled) != 0;
	bool startDisabled = !wasDisabled && disabled;
	if (startDisabled)
		ImGui::BeginDisabled(true);
	bool result = ImGui::InvisibleButton(id.c_str(), size);
	if (startDisabled)
		ImGui::EndDisabled();
	hovered = ImGui::IsItemHovered(startDisabled ? ImGuiHoveredFlags_AllowWhenDisabled : 0);
	active = ImGui::IsItemActive();
	return result;
}

/// Invisible button that can optionally be disabled to allow drag operations with no active id
inline bool InvisibleButton(const std::string id, const ImVec2& size, bool disabled)
{
	if (disabled)
		ImGui::BeginDisabled(true);
	bool result = ImGui::InvisibleButton(id.c_str(), size);
	if (disabled)
		ImGui::EndDisabled();
	return result;
}

/// Helper data structure for BeginRect()/EndRect().
struct RectData
{
	ImVec2 BackupCursorPos;
	ImVec2 BackupCursorMaxPos;
};

/// Begin measurement of an ImGui item rect. Much like BeginGroup(), but just the item rect size measuring part.
/// Returns temporary RectData that need to be passed to the corresponding EndRect() call.
/// Explicit passing of data is necessary as we don't have a global stack for it like Dear ImGui does.
inline RectData BeginRect()
{
	RectData data;
	ImGuiContext& g = *GImGui;
	ImGuiWindow* window = g.CurrentWindow;
	data.BackupCursorPos = window->DC.CursorPos;
	data.BackupCursorMaxPos = window->DC.CursorMaxPos;
	return data;
}

/// End measurement of a previous BeginRect() call. Finalized the passed rect data.
inline ImRect EndRect(RectData& data)
{
	ImGuiContext& g = *GImGui;
	ImGuiWindow* window = g.CurrentWindow;
	return ImRect(data.BackupCursorPos, ImMax(window->DC.CursorMaxPos, data.BackupCursorPos));
}

inline void Tooltip(const char* header, const char* description, const ImColor& color, float wrapWidth = 35.0f)
{
	if (ImGui::BeginTooltip())
	{
		ImGui::PushTextWrapPos(ImGui::GetFontSize() * wrapWidth);
		ImGui::TextColored(color, header);

		if (description != nullptr && strlen(description) != 0)
		{
			ImGui::TextDisabled(description);
		}

		ImGui::PopTextWrapPos();
		ImGui::EndTooltip();
	}
}

} // namespace DGui
namespace DMath
{
template <typename T>
T clamp(T value, T low, T high)
{
	return std::min(std::max(value, low), high);
}
template <typename T>
float map(T value, T min1, T max1, T min2, T max2)
{
	return min2 + (value - min1) * (max2 - min2) / (max1 - min1);
}
inline float len(const ImVec2& v)
{
	return sqrt(v.x * v.x + v.y * v.y);
}
inline ImVec2 normalize(const ImVec2& v)
{
	ImVec2 ret(v.x, v.y);
	float len = sqrt(v.x * v.x + v.y * v.y);
	ret.x /= len;
	ret.y /= len;
	return ret;
}
inline float smoothstep(float min, float max, float t)
{
	t = clamp((t - min) / (max - min), 0.f, 1.f);
	return t * t * (3.f - 2.f * t);
}
/// Modulo operation supporting negative numbers
template <typename T>
float mod(T x, T m)
{
	return x - m * floor(x / m);
}
/// Modulo operation supporting negative numbers
inline ImVec2 mod(const ImVec2& v, const ImVec2& m)
{
	return ImVec2(mod(v.x, m.x), mod(v.y, m.y));
}
/// Modulo operation supporting negative numbers
inline ImVec2 mod(const ImVec2& v, float m)
{
	return ImVec2(mod(v.x, m), mod(v.y, m));
}
/// Exact equality
inline bool equals(const ImVec2& a, const ImVec2& b)
{
	return a.x == b.x && a.y == b.y;
}
/// Exact equality
inline bool equals(const ImRect& a, const ImRect& b)
{
	return equals(a.Min, b.Min) && equals(a.Max, b.Max);
}
/// Absolute epsilon equality
inline bool equals(float val1, float val2, float abs_error)
{
	return fabsf(val1 - val2) <= abs_error;
}
/// Absolute epsilon equality
inline bool equals(const ImVec2& a, const ImVec2& b, float abs_error)
{
	return equals(a.x, b.x, abs_error) && equals(a.y, b.y, abs_error);
}
inline float distanceToRect(const ImVec2& point, const ImRect& rect)
{
	float dx = std::max(std::max(rect.Min.x - point.x, 0.f), point.x - rect.Max.x);
	float dy = std::max(std::max(rect.Min.y - point.y, 0.f), point.y - rect.Max.y);
	sqrtf(dx * dx + dy * dy);
}
} // namespace DMath

namespace DDraw
{
void drawSpringDebug(const ImVec2& pos, ImVec2 springSize, bool horizontal);

inline void AddLineRaw(ImDrawList* idl, const ImVec2& p1, const ImVec2& p2, ImU32 col, float thickness)
{
	if ((col & IM_COL32_A_MASK) == 0)
		return;
	idl->PathLineTo(p1); // Removed ImVec2(0.5f, 0.5f)
	idl->PathLineTo(p2); // Removed ImVec2(0.5f, 0.5f)
	idl->PathStroke(col, 0, thickness);
}

/// Draws a rounded border inside the given min/max bounds. Needed to properly aling rounded corners.
inline void AddInnerRoundedRectScreen(const ImVec2& min, const ImVec2& max, const ImVec4& col, float rounding,
                                      ImDrawFlags flags, float thickness, float offset)
{
	ImDrawList* idl = ImGui::GetWindowDrawList();
	ImVec2 hThickness(offset * 0.5f, offset * 0.5f);
	ImVec2 borderMin = min + hThickness;
	ImVec2 borderMax = max - hThickness;
	float adjRounding = ImMax(0.0f, rounding - thickness * 0.5f);
	idl->AddRect(borderMin, borderMax, ImGui::ColorConvertFloat4ToU32(col), adjRounding, flags, thickness);
}

inline void AddRectFilledOffsetScreen(const ImVec2& min, const ImVec2& max, const ImVec4& col, float rounding,
                                      ImDrawFlags flags, float offset, bool adjustRounding = true)
{
	ImDrawList* idl = ImGui::GetWindowDrawList();
	ImVec2 hThickness(offset, offset);
	ImVec2 borderMin = min + hThickness;
	ImVec2 borderMax = max - hThickness;
	float adjRounding = adjustRounding ? ImMax(0.0f, rounding - offset) : rounding;
	idl->AddRectFilled(borderMin, borderMax, ImGui::ColorConvertFloat4ToU32(col), adjRounding, flags);
}

inline void AddRectFilledBorderedScreen(const ImVec2& min, const ImVec2& max, const ImVec4& col,
                                        const ImVec4& borderCol, float rounding, ImDrawFlags flags, float thickness)
{
	ImDrawList* idl = ImGui::GetWindowDrawList();
	if (thickness > 0.0f)
	{
		AddRectFilledOffsetScreen(min, max, col, rounding, flags, thickness);
		idl->AddRectFilled(min, max, ImGui::ColorConvertFloat4ToU32(borderCol), rounding, flags);
	}
	else
	{
		idl->AddRectFilled(min, max, ImGui::ColorConvertFloat4ToU32(col), rounding, flags);
	}
}

inline void AddRectFilledDoubleBorderedScreen(const ImVec2& min, const ImVec2& max, const ImVec4& col,
                                              const ImVec4& borderCol1, const ImVec4& borderCol2, float rounding,
                                              ImDrawFlags flags, float thickness1, float thickness2)
{
	constexpr ImVec2 pxo = ImVec2(0.5f, 0.5f);
	ImDrawList* idl = ImGui::GetWindowDrawList();
	if (thickness1 > 0.0f && thickness2 > 0.0f)
	{
		idl->AddRectFilled(min + pxo, max - pxo, ImGui::ColorConvertFloat4ToU32(borderCol2), rounding, flags);
		AddRectFilledOffsetScreen(min + pxo, max - pxo, borderCol1, rounding, flags, thickness1);
		AddRectFilledOffsetScreen(min + pxo, max - pxo, col, rounding, flags, thickness1 + thickness2);
	}
	else if (thickness1 > 0.0f)
	{
		idl->AddRectFilled(min + pxo, max - pxo, ImGui::ColorConvertFloat4ToU32(borderCol1), rounding, flags);
		AddRectFilledOffsetScreen(min + pxo, max - pxo, col, rounding, flags, thickness1);
	}
	else
	{
		idl->AddRectFilled(min + pxo, max - pxo, ImGui::ColorConvertFloat4ToU32(col), rounding, flags);
	}
}
} // namespace DDraw
} // namespace DIWNE