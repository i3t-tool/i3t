#include "Canvas.h"

#include "NodeEditor.h"

namespace DIWNE
{
Canvas::Canvas(NodeEditor& editor)
    : editor(editor),
      m_viewRectDiwne(editor.mp_settingsDiwne->workAreaDiwne.Min, editor.mp_settingsDiwne->workAreaDiwne.Max),
      m_zoom(editor.mp_settingsDiwne->workAreaInitialZoom)
{}

void Canvas::setZoom(float val)
{
	m_prevZoom = m_zoom;
	if (val < editor.mp_settingsDiwne->minWorkAreaZoom)
	{
		m_zoom = editor.mp_settingsDiwne->minWorkAreaZoom;
	}
	else if (val > editor.mp_settingsDiwne->maxWorkAreaZoom)
	{
		m_zoom = editor.mp_settingsDiwne->maxWorkAreaZoom;
	}
	else
		m_zoom = val;
	if (m_prevZoom != m_zoom)
		editor.onZoom();
}

bool Canvas::applyZoomScaling()
{
	if (m_zoomScalingApplied)
		return true;

	// Fringe scale can stay at 1 (default), it's a parameter that specifies how "blurry" anti aliased lines/shapes
	// are ImGui::GetCurrentWindow()->DrawList->_FringeScale = 1 / m_zoom;
	// ImGui::SetWindowFontScale(m_zoom);

	auto* font = ImGui::GetFont();
	if (!font)
	{
		return false;
	}

	m_zoomOriginalFontScale = applyZoomScalingToFont(font, 1.0f);
	ImGui::PushFont(font);

	// Scale the whole ImGui style, will be restored later
	m_zoomOriginalStyle = ImGui::GetStyle();

	//	ImGui::GetStyle().ScaleAllSizes(m_zoom);
	//	ImGui::GetStyle().ScaleAllSizes(d > dMax ? m_zoom : 1.0f);
	// ScaleAllSizes(ImGui::GetStyle(), d > dMax ? m_zoom : 1.0f);
	ScaleAllSizes(ImGui::GetStyle(), m_zoom);

	// TODO: We do not round styles, ImGui does round them, this introduces an issue with the cursor position not being
	//  rounded when starting a new imgui item, once the cursor is moved to the next item ImGui automatically rounds it.
	//  This creates an inconsistency where first elements can be offset a little and not others.

	//	d %= dMax * 2;

	m_zoomScalingApplied = true;
	return false;
}

bool Canvas::stopZoomScaling()
{
	if (!m_zoomScalingApplied)
		return false;

	ImGui::GetCurrentContext()->Style = m_zoomOriginalStyle;

	// Need to reset default font BEFORE popping font
	stopZoomScalingToFont(ImGui::GetFont(), m_zoomOriginalFontScale);
	ImGui::PopFont();

	m_zoomScalingApplied = false;
	return true;
}

bool Canvas::ensureZoomScaling(bool active)
{
	bool activeBefore = m_zoomScalingApplied;
	if (activeBefore != active)
	{
		if (active)
		{
			applyZoomScaling();
		}
		else
		{
			stopZoomScaling();
		}
	}
	return activeBefore;
}

float Canvas::applyZoomScalingToFont(ImFont* font, float scaleMultiplier)
{
	if (!font)
	{
		return 1.0f;
	}

	ImFont* f = font;
	// if (largeFont != nullptr)
	// {
	// 	f = largeFont;
	// }
	// TODO: (DR) Original scale could be stored in view specific temp storage
	//  that way we could calculate scaled font size without actually scaling the font
	float originalScale = f->Scale;
	f->Scale = m_zoom * scaleMultiplier;
	ImGui::PushFont(f);

	return originalScale;

	// TODO: Test dynamic font switching based on zoom level
}

void Canvas::stopZoomScalingToFont(ImFont* font, float originalScale)
{
	font->Scale = originalScale;
	ImGui::PopFont();
}

void Canvas::ScaleAllSizes(ImGuiStyle& style, float scale_factor)
{
	style.WindowPadding = style.WindowPadding * scale_factor;
	style.WindowRounding = style.WindowRounding * scale_factor;
	style.WindowMinSize = style.WindowMinSize * scale_factor;
	style.ChildRounding = style.ChildRounding * scale_factor;
	style.PopupRounding = style.PopupRounding * scale_factor;
	style.FramePadding = style.FramePadding * scale_factor;
	style.FrameRounding = style.FrameRounding * scale_factor;
	style.ItemSpacing = style.ItemSpacing * scale_factor;
	style.ItemInnerSpacing = style.ItemInnerSpacing * scale_factor;
	style.CellPadding = style.CellPadding * scale_factor;
	style.TouchExtraPadding = style.TouchExtraPadding * scale_factor;
	style.IndentSpacing = style.IndentSpacing * scale_factor;
	style.ColumnsMinSpacing = style.ColumnsMinSpacing * scale_factor;
	style.ScrollbarSize = style.ScrollbarSize * scale_factor;
	style.ScrollbarRounding = style.ScrollbarRounding * scale_factor;
	style.GrabMinSize = style.GrabMinSize * scale_factor;
	style.GrabRounding = style.GrabRounding * scale_factor;
	style.LogSliderDeadzone = style.LogSliderDeadzone * scale_factor;
	style.TabRounding = style.TabRounding * scale_factor;
	style.TabMinWidthForCloseButton =
	    (style.TabMinWidthForCloseButton != FLT_MAX) ? style.TabMinWidthForCloseButton * scale_factor : FLT_MAX;
	style.SeparatorTextPadding = style.SeparatorTextPadding * scale_factor;
	style.DockingSeparatorSize = style.DockingSeparatorSize * scale_factor;
	style.DisplayWindowPadding = style.DisplayWindowPadding * scale_factor;
	style.DisplaySafeAreaPadding = style.DisplaySafeAreaPadding * scale_factor;
	style.MouseCursorScale = style.MouseCursorScale * scale_factor;
}

void Canvas::updateViewportRects()
{
	ImVec2 windowPos = ImGui::GetWindowPos();
	ImVec2 windowSize = ImGui::GetWindowSize();

	m_viewRectScreen.Min = windowPos;
	m_viewRectScreen.Max = windowPos + windowSize;

	m_viewRectDiwne.Max = m_viewRectDiwne.Min + windowSize / m_zoom;

	//	DIWNE_INFO("waScreen: {}:{}, {}:{}, waDiwne: {}:{}, {}:{}", m_viewRectScreen.Min.x, m_viewRectScreen.Min.y,
	//	           m_viewRectScreen.Max.x, m_viewRectScreen.Max.y, m_viewRectDiwne.Min.x, m_viewRectDiwne.Min.y,
	//	           m_viewRectDiwne.Max.x, m_viewRectDiwne.Max.y)
}

// ImVec2 Canvas::transformFromImGuiToDiwne(const ImVec2& point) const
// {
// 	return workArea2screen(screen2workArea(point) / m_zoom); /* basically just zoom */
// }
//
// ImVec2 Canvas::transformFromDiwneToImGui(const ImVec2& point) const
// {
// 	return workArea2screen(screen2workArea(point) * m_zoom); /* basically just zoom */
// }
//
// ImVec4 Canvas::transformFromImGuiToDiwne(const ImVec4& rect) const
// {
// 	ImVec2 const topleft = transformFromImGuiToDiwne(ImVec2(rect.x, rect.y));
// 	ImVec2 const bottomright = transformFromImGuiToDiwne(ImVec2(rect.z, rect.w));
// 	return ImVec4(topleft.x, topleft.y, bottomright.x, bottomright.y);
// }
//
// ImVec4 Canvas::transformFromDiwneToImGui(const ImVec4& rect) const
// {
// 	ImVec2 const topleft = transformFromDiwneToImGui(ImVec2(rect.x, rect.y));
// 	ImVec2 const bottomright = transformFromDiwneToImGui(ImVec2(rect.z, rect.w));
// 	return ImVec4(topleft.x, topleft.y, bottomright.x, bottomright.y);
// }

void Canvas::moveViewportZoomed(const ImVec2& distance)
{
	moveViewport(screen2diwneSize(distance));
}

void Canvas::moveViewport(const ImVec2& distance)
{
	m_viewRectDiwne.Translate(distance);
}

ImVec2 Canvas::screen2workArea(const ImVec2& point) const
{
	return point - m_viewRectScreen.Min;
}

ImVec2 Canvas::workArea2screen(const ImVec2& point) const
{
	return point + m_viewRectScreen.Min;
}

ImVec2 Canvas::diwne2workArea(const ImVec2& point) const
{
	float dpiScale = editor.mp_settingsDiwne->dpiScale;
	return (point - m_viewRectDiwne.Min) * m_zoom * dpiScale;
}

ImVec2 Canvas::workArea2diwne(const ImVec2& point) const
{
	float dpiScale = editor.mp_settingsDiwne->dpiScale;
	return point / (m_zoom * dpiScale) + m_viewRectDiwne.Min;
}

ImVec2 Canvas::screen2diwne(const ImVec2& point) const
{
	return workArea2diwne(screen2workArea(point));
}

ImVec2 Canvas::diwne2screen(const ImVec2& point) const
{
	return workArea2screen(diwne2workArea(point));
}
ImVec2 Canvas::diwne2screenTrunc(const ImVec2& point) const
{
	ImVec2 pos = diwne2screen(point);
	return ImVec2(IM_TRUNC(pos.x + DIWNE_PIXEL_EPSILON), IM_TRUNC(pos.y + DIWNE_PIXEL_EPSILON));
}
ImRect Canvas::diwne2screen(const ImRect& rect) const
{
	return ImRect(diwne2screen(rect.Min), diwne2screen(rect.Max));
}
ImRect Canvas::screen2diwne(const ImRect& rect) const
{
	return ImRect(screen2diwne(rect.Min), screen2diwne(rect.Max));
}
ImRect Canvas::diwne2screenTrunc(const ImRect& rect) const
{
	return ImRect(diwne2screenTrunc(rect.Min), diwne2screenTrunc(rect.Max));
}
float Canvas::diwne2screenSize(float size) const
{
	return size * editor.getZoom() * editor.getDpiScale();
}
float Canvas::screen2diwneSize(float size) const
{
	return size / (editor.getZoom() * editor.getDpiScale());
}
float Canvas::diwne2screenSizeTrunc(float size) const
{
	return IM_TRUNC(diwne2screenSize(size));
}
ImVec2 Canvas::diwne2screenSize(const ImVec2& point) const
{
	return ImVec2(diwne2screenSize(point.x), diwne2screenSize(point.y));
}
ImVec2 Canvas::screen2diwneSize(const ImVec2& point) const
{
	return ImVec2(screen2diwneSize(point.x), screen2diwneSize(point.y));
}

float Canvas::getZoom() const
{
	return m_zoom;
};
ImRect Canvas::getViewportRectDiwne() const
{
	return m_viewRectDiwne;
};
void Canvas::setViewportRectDiwne(ImRect rect)
{
	m_viewRectDiwne = rect;
};
ImRect Canvas::getViewportRectScreen() const
{
	return m_viewRectScreen;
};

void Canvas::AddRectFilledDiwne(const ImVec2& p_min, const ImVec2& p_max, ImVec4 col, float rounding,
                                ImDrawFlags rounding_corners, bool ignoreZoom) const
{
	ImDrawList* idl = ImGui::GetWindowDrawList(); /* \todo maybe use other channel with correct
	                                                 Clip rect for drawing of manual shapes, but
	                                                 be careful with order of drew elements */
	idl->AddRectFilled(diwne2screenTrunc(p_min), diwne2screenTrunc(p_max), ImGui::ColorConvertFloat4ToU32(col),
	                   rounding * (ignoreZoom ? 1.0f : m_zoom), rounding_corners);
}

void Canvas::AddLine(const ImVec2& p1, const ImVec2& p2, ImVec4 col, float thickness, bool ignoreZoom) const
{
	ImGui::GetWindowDrawList()->AddLine(diwne2screen(p1), diwne2screen(p2), ImGui::ColorConvertFloat4ToU32(col),
	                                    thickness * (ignoreZoom ? 1.0f : m_zoom));
}

void Canvas::AddRectDiwne(const ImVec2& p_min, const ImVec2& p_max, ImVec4 col, float rounding,
                          ImDrawFlags rounding_corners, float thickness, bool ignoreZoom) const
{
	ImDrawList* idl = ImGui::GetWindowDrawList();
	idl->AddRect(diwne2screenTrunc(p_min), diwne2screenTrunc(p_max), ImGui::ColorConvertFloat4ToU32(col),
	             rounding * (ignoreZoom ? 1.0f : m_zoom), rounding_corners, thickness * (ignoreZoom ? 1.0f : m_zoom));
}

void Canvas::AddRectForegroundDiwne(const ImVec2& p_min, const ImVec2& p_max, ImVec4 col, float rounding,
                                    ImDrawFlags rounding_corners, float thickness, bool ignoreZoom) const
{
	ImDrawList* idl = ImGui::GetForegroundDrawList();
	idl->AddRect(diwne2screenTrunc(p_min), diwne2screenTrunc(p_max), ImGui::ColorConvertFloat4ToU32(col),
	             rounding * (ignoreZoom ? 1.0f : m_zoom), rounding_corners, thickness * (ignoreZoom ? 1.0f : m_zoom));
}

void Canvas::AddBezierCurveDiwne(ImDrawList* idl, const ImVec2& p1, const ImVec2& p2, const ImVec2& p3,
                                 const ImVec2& p4, ImVec4 col, float thickness, int num_segments /*=0*/) const
{
	idl->AddBezierCubic(diwne2screen(p1), diwne2screen(p2), diwne2screen(p3), diwne2screen(p4),
	                    ImGui::ColorConvertFloat4ToU32(col), thickness * m_zoom, num_segments);
}
void Canvas::drawGrid(bool dots, float size, ImVec4 color, float fadeStart, float fadeEnd, bool ignoreZoom) const
{
	assert(fadeStart > fadeEnd);
	if (m_zoom < fadeEnd)
		return;

	ImDrawList* idl = ImGui::GetWindowDrawList();
	const ImRect diwneRect = getViewportRectDiwne();
	const ImRect screenRect = getViewportRectScreen();
	const ImVec2 gridOffset = ImVec2(size, size) - DMath::mod(diwneRect.Min, size);
	const ImVec2 gridStartS = screenRect.Min + diwne2screenSize(gridOffset);
	const float step = diwne2screenSize(size);

	float alphaFactor = DMath::smoothstep(0.0f, 1.0f, DMath::map(m_zoom, fadeStart, fadeEnd, 1.f, 0.f));
	color.w *= alphaFactor;

	ImU32 col = ImGui::ColorConvertFloat4ToU32(color);
	if (!dots)
	{
		for (float x = gridStartS.x; x < screenRect.Max.x; x += step)
			idl->AddLine({x, screenRect.Min.y}, {x, screenRect.Max.y}, col, ignoreZoom ? 1.0f : m_zoom);
		for (float y = gridStartS.y; y < screenRect.Max.y; y += step)
			idl->AddLine({screenRect.Min.x, y}, {screenRect.Max.x, y}, col, ignoreZoom ? 1.0f : m_zoom);
	}
	else
	{
		ImDrawListFlags backup_flags = idl->Flags;
		idl->Flags &= ~ImDrawListFlags_AntiAliasedFill;
		for (float x = gridStartS.x; x < screenRect.Max.x; x += step)
		{
			for (float y = gridStartS.y; y < screenRect.Max.y; y += step)
			{
				ImVec2 p = {x, y};
				float o = 1.5f * editor.getDpiScale();
				ImVec2 points[] = {
				    ImVec2(p.x - o, p.y),
				    ImVec2(p.x, p.y - o),
				    ImVec2(p.x + o, p.y),
				    ImVec2(p.x, p.y + o),
				};
				idl->AddConvexPolyFilled(points, 4, col);
				// idl->AddCircleFilled({x, y}, 2.0f * editor.getDpiScale(), col, 4);
			}
		}
		idl->Flags = backup_flags;
	}
}

void Canvas::DrawIconCircle(ImDrawList* idl, ImColor shapeColor, ImColor innerColor, ImVec2 topLeft, ImVec2 bottomRight,
                            bool filled, float thickness /*=1*/) const
{
	ImVec2 center = ImVec2((topLeft.x + bottomRight.x) / 2, (topLeft.y + bottomRight.y) / 2);
	float radius = std::min(bottomRight.x - topLeft.x, bottomRight.y - topLeft.y) / 2;

	idl->AddCircleFilled(center, radius, shapeColor);
	if (!filled)
	{
		idl->AddCircleFilled(center, radius - thickness, innerColor);
	}
}

void Canvas::DrawIconRectangle(ImDrawList* idl, ImColor shapeColor, ImColor innerColor, ImVec2 topLeft,
                               ImVec2 bottomRight, bool filled, ImVec2 thickness /*=ImVec2(1, 1)*/,
                               float rounding /*= 0*/) const
{
	idl->AddRectFilled(topLeft, bottomRight, shapeColor, rounding);
	if (!filled)
	{
		idl->AddRectFilled(topLeft + thickness, bottomRight - thickness, innerColor, rounding);
	}
}
void Canvas::DrawIconPause(ImDrawList* idl, ImColor shapeColor, ImColor innerColor, ImVec2 topLeft, ImVec2 bottomRight,
                           bool filled, ImVec2 thickness, float rounding) const
{
	const float width = bottomRight.x - topLeft.x;
	// const float columnWidth = width / 3.0f;
	const float columnWidth = width * 6.0f / 15.0f;
	ImVec2 bottomRight1;
	bottomRight1.x = topLeft.x + columnWidth;
	bottomRight1.y = bottomRight.y;

	ImVec2 topLeft2;
	topLeft2.x = bottomRight.x - columnWidth;
	topLeft2.y = topLeft.y;

	idl->AddRectFilled(topLeft, bottomRight1, shapeColor, rounding);
	idl->AddRectFilled(topLeft2, bottomRight, shapeColor, rounding);
	if (!filled)
	{
		idl->AddRectFilled(topLeft + thickness, bottomRight1 - thickness, innerColor, rounding);
		idl->AddRectFilled(topLeft2 + thickness, bottomRight - thickness, innerColor, rounding);
	}
}

void Canvas::DrawIconTriangleLeft(ImDrawList* idl, ImColor shapeColor, ImColor innerColor, ImVec2 topLeft,
                                  ImVec2 bottomRight, bool filled, float thickness /*= 1*/) const
{
	idl->AddTriangleFilled(ImVec2(bottomRight.x, topLeft.y), ImVec2(topLeft.x, (topLeft.y + bottomRight.y) / 2),
	                       bottomRight, shapeColor);
	if (!filled)
	{
		idl->AddTriangleFilled(ImVec2(bottomRight.x - thickness, topLeft.y + thickness),
		                       ImVec2(topLeft.x + thickness, (topLeft.y + bottomRight.y) / 2),
		                       bottomRight - ImVec2(thickness, thickness), innerColor);
	}
}

// | I < | --- 1 1 5   vertical bar followed by the arrow
void Canvas::DrawIconSkipBack(ImDrawList* idl, ImColor shapeColor, ImColor innerColor, ImVec2 topLeft,
                              ImVec2 bottomRight, bool filled, float thickness /*= 1*/) const
{
	const float columnWidth = (bottomRight.x - topLeft.x) / 7.0f; // width of vertical line & padding
	ImVec2 bottomRight1;
	bottomRight1.x = topLeft.x + columnWidth; // vertical bar
	bottomRight1.y = bottomRight.y;

	// float Left2x = bottomRight.x - 5.0f * columnWidth; // triangle
	ImVec2 middleLeft2;
	middleLeft2.x = bottomRight.x - 5.0f * columnWidth; // triangle
	middleLeft2.y = (topLeft.y + bottomRight.y) / 2;

	idl->AddRectFilled(topLeft, bottomRight1, shapeColor);
	idl->AddTriangleFilled(ImVec2(bottomRight.x, topLeft.y), middleLeft2, bottomRight, shapeColor);
	if (!filled)
	{
		idl->AddRectFilled(topLeft + ImVec2(thickness, thickness), bottomRight1 - ImVec2(thickness, thickness),
		                   innerColor);
		idl->AddTriangleFilled(ImVec2(bottomRight.x, topLeft.y) + ImVec2(-thickness, 2 * thickness),
		                       middleLeft2 + ImVec2(2.0f * thickness, 0.0f),
		                       bottomRight - ImVec2(thickness, 2 * thickness), innerColor);
	}
}

// | < I | --- 5 1 1    arrow followed by the vertical bar
void Canvas::DrawIconSkipBack2(ImDrawList* idl, ImColor shapeColor, ImColor innerColor, ImVec2 topLeft,
                               ImVec2 bottomRight, bool filled, float thickness /*= 1*/) const
{
	const float columnWidth = (bottomRight.x - topLeft.x) / 7.0f; // width of vertical line & padding

	ImVec2 middleLeft1 = ImVec2(topLeft.x, (topLeft.y + bottomRight.y) / 2); // triangle
	ImVec2 topRight1 = ImVec2(topLeft.x + 5.0f * columnWidth, topLeft.y);
	ImVec2 bottomRight1 = ImVec2(topRight1.x, bottomRight.y);

	ImVec2 topLeft2 = ImVec2(bottomRight.x - 1.0f * columnWidth, topLeft.y); // rectangle

	idl->AddTriangleFilled(middleLeft1, topRight1, bottomRight1, shapeColor);
	idl->AddRectFilled(topLeft2, bottomRight, shapeColor);
	if (!filled)
	{
		idl->AddTriangleFilled(middleLeft1 + ImVec2(2.0f * thickness, 0.0f),
		                       topRight1 + ImVec2(-thickness, 2 * thickness),
		                       bottomRight1 + ImVec2(-thickness, -2 * thickness), innerColor);
		idl->AddRectFilled(topLeft2 + ImVec2(thickness, thickness), bottomRight - ImVec2(thickness, thickness),
		                   innerColor);
	}
}
void Canvas::DrawIconRewind(ImDrawList* idl, ImColor shapeColor, ImColor innerColor, ImVec2 topLeft, ImVec2 bottomRight,
                            bool filled, float thickness) const
{
	const ImVec2 middleLeft = ImVec2(topLeft.x, (topLeft.y + bottomRight.y) / 2.0f);

	const ImVec2 middlePoint = (topLeft + bottomRight) / 2.0f;
	const ImVec2 topMiddle = ImVec2(middlePoint.x, topLeft.y);
	const ImVec2 bottomMiddle = ImVec2(middlePoint.x, bottomRight.y);

	const ImVec2 topRight = ImVec2(bottomRight.x, topLeft.y);

	idl->AddTriangleFilled(topMiddle, middleLeft, bottomMiddle, shapeColor);
	idl->AddTriangleFilled(topRight, middlePoint, bottomRight, shapeColor);

	if (!filled)
	{
		idl->AddTriangleFilled(topMiddle + ImVec2(-thickness, 2 * thickness), middleLeft + ImVec2(2 * thickness, 0.0f),
		                       bottomMiddle + ImVec2(-thickness, -2 * thickness), innerColor);
		idl->AddTriangleFilled(topRight + ImVec2(-thickness, 2 * thickness),
		                       middlePoint + ImVec2(2 * thickness, thickness),
		                       bottomRight + ImVec2(-thickness, -2.0f * thickness), innerColor);
	}
}

// | > I | --- 5 1 1   right arrow followed by the vertical bar
void Canvas::DrawIconSkipForward(ImDrawList* idl, ImColor shapeColor, ImColor innerColor, ImVec2 topLeft,
                                 ImVec2 bottomRight, bool filled, float thickness /*= 1*/) const
{
	const float columnWidth = (bottomRight.x - topLeft.x) / 7.0f; // width of vertical line & padding

	ImVec2 middleRight1;
	middleRight1.x = topLeft.x + 5.0f * columnWidth; // triangle
	middleRight1.y = (topLeft.y + bottomRight.y) / 2;

	ImVec2 topLeft2;
	topLeft2.x = bottomRight.x - columnWidth; // vertical bar
	topLeft2.y = topLeft.y;

	idl->AddTriangleFilled(topLeft, middleRight1, ImVec2(topLeft.x, bottomRight.y), shapeColor);
	idl->AddRectFilled(topLeft2, bottomRight, shapeColor);

	if (!filled)
	{
		idl->AddTriangleFilled(topLeft + ImVec2(thickness, 2 * thickness),
		                       middleRight1 - ImVec2(2.0f * thickness, 0.0f),
		                       ImVec2(topLeft.x, bottomRight.y) + ImVec2(thickness, -2 * thickness), innerColor);
		idl->AddRectFilled(topLeft2 + ImVec2(thickness, thickness), bottomRight - ImVec2(thickness, thickness),
		                   innerColor);
	}
}

// | I > | --- 1 1 5     vertical bar followed by the right arrow
void Canvas::DrawIconSkipForward2(ImDrawList* idl, ImColor shapeColor, ImColor innerColor, ImVec2 topLeft,
                                  ImVec2 bottomRight, bool filled, float thickness /*= 1*/) const
{
	const float columnWidth = (bottomRight.x - topLeft.x) / 7.0f; // width of vertical line & padding

	ImVec2 bottomRight1;
	bottomRight1.x = topLeft.x + columnWidth; // vertical bar
	bottomRight1.y = bottomRight.y;

	ImVec2 topLeft2 = ImVec2(bottomRight.x - 5.0f * columnWidth, topLeft.y); // triangle
	ImVec2 bottomLeft2 = ImVec2(topLeft2.x, bottomRight.y);

	ImVec2 middleRight2 = ImVec2(bottomRight.x, (topLeft.y + bottomRight.y) / 2.0f);

	idl->AddRectFilled(topLeft, bottomRight1, shapeColor);
	idl->AddTriangleFilled(topLeft2, bottomLeft2, middleRight2, shapeColor);

	if (!filled)
	{
		idl->AddRectFilled(topLeft + ImVec2(thickness, thickness), bottomRight1 - ImVec2(thickness, thickness),
		                   innerColor);

		idl->AddTriangleFilled(topLeft2 + ImVec2(thickness, 2 * thickness),
		                       bottomLeft2 + ImVec2(thickness, -2 * thickness),
		                       middleRight2 + ImVec2(-2.0f * thickness, 0.0f), innerColor);
	}
}
void Canvas::DrawIconFastForward(ImDrawList* idl, ImColor shapeColor, ImColor innerColor, ImVec2 topLeft,
                                 ImVec2 bottomRight, bool filled, float thickness) const
{
	const ImVec2 bottomLeft = ImVec2(topLeft.x, bottomRight.y);

	const ImVec2 middlePoint = (topLeft + bottomRight) / 2.0f;
	const ImVec2 topMiddle = ImVec2(middlePoint.x, topLeft.y);
	const ImVec2 bottomMiddle = ImVec2(middlePoint.x, bottomRight.y);

	const ImVec2 middleRight = ImVec2(bottomRight.x, (topLeft.y + bottomRight.y) / 2.0f);

	idl->AddTriangleFilled(topLeft, middlePoint, bottomLeft, shapeColor);
	idl->AddTriangleFilled(topMiddle, middleRight, bottomMiddle, shapeColor);

	if (!filled)
	{
		idl->AddTriangleFilled(topLeft + ImVec2(thickness, 2 * thickness), middlePoint + ImVec2(-2 * thickness, 0.0f),
		                       bottomLeft + ImVec2(thickness, -2 * thickness), innerColor);
		idl->AddTriangleFilled(topMiddle + ImVec2(thickness, 2 * thickness), middleRight + ImVec2(-2 * thickness, 0.0f),
		                       bottomMiddle + ImVec2(thickness, -2 * thickness), innerColor);
	}
}

void Canvas::DrawIconTriangleRight(ImDrawList* idl, ImColor shapeColor, ImColor innerColor, ImVec2 topLeft,
                                   ImVec2 bottomRight, bool filled, float thickness /*= 1*/) const
{
	idl->AddTriangleFilled(topLeft, ImVec2(bottomRight.x, (topLeft.y + bottomRight.y) / 2),
	                       ImVec2(topLeft.x, bottomRight.y), shapeColor);
	if (!filled)
	{
		idl->AddTriangleFilled(topLeft + ImVec2(thickness, 2 * thickness), // (PF 2x for better border)
		                       ImVec2(bottomRight.x - 2 * thickness, (topLeft.y + bottomRight.y) / 2),
		                       ImVec2(topLeft.x + thickness, bottomRight.y - 2 * thickness), innerColor);
	}
}

void Canvas::DrawIconTriangleDownRight(ImDrawList* idl, ImColor shapeColor, ImColor innerColor, ImVec2 topLeft,
                                       ImVec2 bottomRight, bool filled, float thickness /*= 1*/) const
{
	ImVec2 p1 = ImVec2(topLeft.x + 0.5f, bottomRight.y - 0.5f);
	ImVec2 p2 = bottomRight - ImVec2(0.5f, 0.5f);
	ImVec2 p3 = ImVec2(bottomRight.x - 0.5f, topLeft.y + 0.5f);

	idl->AddTriangleFilled(p1, p2, p3, shapeColor);
	if (!filled)
	{
		idl->AddTriangleFilled(p1 + ImVec2(2 * thickness, -thickness), p2 - ImVec2(thickness, thickness),
		                       p3 + ImVec2(-thickness, 2 * thickness), innerColor);
	}
}

void Canvas::DrawIconTriangleDownLeft(ImDrawList* idl, ImColor shapeColor, ImColor innerColor, ImVec2 topLeft,
                                      ImVec2 bottomRight, bool filled, float thickness /*= 1*/) const
{
	ImVec2 p1 = topLeft + ImVec2(0.5f, 0.5f);
	ImVec2 p2 = ImVec2(topLeft.x + 0.5f, bottomRight.y - 0.5f);
	ImVec2 p3 = bottomRight - ImVec2(0.5f, 0.5f);

	idl->AddTriangleFilled(p1, p2, p3, shapeColor);
	if (!filled)
	{
		idl->AddTriangleFilled(p1 + ImVec2(thickness, 2 * thickness), p2 + ImVec2(thickness, -thickness),
		                       p3 - ImVec2(2 * thickness, thickness), innerColor);
	}
}

void Canvas::DrawIconGrabDownLeft(ImDrawList* idl, ImColor shapeColor, ImColor innerColor, ImVec2 topLeft,
                                  ImVec2 bottomRight, bool filled, float thickness /*= 1*/) const
{
	ImVec2 margin = diwne2screenSize(ImVec2(0.5f, 0.5f));
	topLeft = topLeft + margin;
	bottomRight = bottomRight - margin;

	int lineCount = 3;
	float padding = diwne2screenSize(1.5f);
	float squaredPadding = sqrt(2) * padding;
	float pointOffsetLong = 2 * squaredPadding;
	float pointOffsetShort = padding;
	float size = abs(bottomRight.y - topLeft.y) - pointOffsetLong - pointOffsetShort;
	float step = size / lineCount;

	for (int i = 0; i < lineCount; i++)
	{
		idl->AddLine(ImVec2(bottomRight.x - 1.2f * pointOffsetLong - (i * step), bottomRight.y - pointOffsetShort),
		             ImVec2(topLeft.x + pointOffsetShort, topLeft.y + 1.2f * pointOffsetLong + (i * step)), shapeColor,
		             thickness * m_zoom);
	}
}

void Canvas::DrawIconGrabDownRight(ImDrawList* idl, ImColor shapeColor, ImColor innerColor, ImVec2 topLeft,
                                   ImVec2 bottomRight, bool filled, float thickness /*= 1*/) const
{
	ImVec2 margin = diwne2screenSize(ImVec2(0.5f, 0.5f));
	topLeft = topLeft + margin;
	bottomRight = bottomRight - margin;

	int lineCount = 3;
	float padding = diwne2screenSize(1.5f);
	float squaredPadding = sqrt(2) * padding;
	float pointOffsetLong = 2 * squaredPadding;
	float pointOffsetShort = padding;
	float size = abs(bottomRight.y - topLeft.y) - pointOffsetLong - pointOffsetShort;
	float step = size / lineCount;

	for (int i = 0; i < lineCount; i++)
	{
		idl->AddLine(ImVec2(topLeft.x + 1.2f * pointOffsetLong + (i * step), bottomRight.y - pointOffsetShort),
		             ImVec2(bottomRight.x - pointOffsetShort, topLeft.y + 1.2f * pointOffsetLong + (i * step)),
		             shapeColor, thickness * m_zoom);
	}
}
void Canvas::DrawBurgerMenu(ImDrawList* idl, const ImColor& color, const ImRect& rect, const ImVec2& padding,
                            float thickness) const
{
	thickness = diwne2screenSize(thickness);
	ImVec2 pad = diwne2screenSize(padding);

	ImVec2 topLeft = rect.Min + pad;
	ImVec2 bottomRight = rect.Max - pad;
	topLeft.y += thickness / 2.0f;
	ImVec2 p2 = ImVec2(bottomRight.x, topLeft.y);
	float stepY = ((bottomRight.y - topLeft.y) - thickness) / 2.0f;

	DDraw::AddLineRaw(idl, topLeft, p2, color, thickness);
	topLeft.y += stepY;
	p2.y += stepY;
	DDraw::AddLineRaw(idl, topLeft, p2, color, thickness);
	topLeft.y += stepY;
	p2.y += stepY;
	DDraw::AddLineRaw(idl, topLeft, p2, color, thickness);
}

void Canvas::DrawIconCross(ImDrawList* idl, ImColor shapeColor, ImColor innerColor, ImVec2 topLeft, ImVec2 bottomRight,
                           bool filled, float shapeThickness, float innerThickness) const
{
	bottomRight = bottomRight - ImVec2(1.0f, 1.0f);

	innerThickness = diwne2screenSize(shapeThickness);
	shapeThickness = diwne2screenSize(shapeThickness);

	ImVec2 pTL = topLeft;
	ImVec2 pBR = bottomRight;
	ImVec2 pTR = ImVec2(bottomRight.x, topLeft.y);
	ImVec2 pBL = ImVec2(topLeft.x, bottomRight.y);

	// (DR): This is junky, but it seems to help making the cross look more symmetrical
	//   The true reason for the asymmetry probably lies in the zoom level value, on my machine it steps in 0.125
	//   intervals, every other zoom level is fine, the ones between can cause asymmetry, might be a good idea to
	//   investigate this further and maybe restrict what values the zoom can have
	// (PF) this should be set by Nodes_Transformation_ValidIcon_padding for isValid Icon.
	//   for cross was (184, 196, 185, 196)
	//   for hyphen is
	pTR = pTR + ImVec2(0.01f, -0.01f);
	pBR = pBR + ImVec2(0.01f, 0.01f);

	//	LOG_INFO("zoom: {}", m_zoom);
	//	LOG_INFO("pTL: {:10.3f},{:10.3f}  pBR: {:10.3f},{:10.3f}", pTL.x, pTL.y, pBR.x, pBR.y);
	idl->AddLine(pTL, pBR, shapeColor, shapeThickness);
	idl->AddLine(pBL, pTR, shapeColor, shapeThickness);

	// TODO: (DR) Commenting this out for the time being as it isn't used anyway, needs to be rewritten to support
	//   zooming like the code above
	//	float thicknessDiff = (shapeThickness - thicknesInner) / 2;
	//	ImVec2 thicknessDiffVec = ImVec2(thicknesDiff, thicknesDiff);
	//	if (!filled)
	//	{
	//				idl->AddLine(topLeft + thicknessDiffVec, bottomRight - thicknessDiffVec, InnerColor,
	// thicknessInner); 		idl->AddLine(ImVec2(topLeft.x, bottomRight.y) + thicknessDiffVec,
	// ImVec2(bottomRight.x, topLeft.y) - thicknessDiffVec, InnerColor, thicknessInner);
	//	}
}

void Canvas::DrawIconHyphen(ImDrawList* idl, ImColor shapeColor, ImColor innerColor, ImVec2 topLeft, ImVec2 bottomRight,
                            bool filled, float thickness) const
{
	bottomRight = bottomRight - ImVec2(1.0f, 1.0f);

	float middleY = (topLeft.y + bottomRight.y) / 2;
	ImVec2 start = ImVec2(topLeft.x, middleY);
	ImVec2 end = ImVec2(bottomRight.x, middleY);

	idl->AddLine(start, end, shapeColor, diwne2screenSize(thickness));
}

bool Canvas::IconButton(IconType bgIconType, ImColor bgShapeColor, ImColor bgInnerColor, ImVec2 size, ImVec4 padding,
                        bool filled, std::string id) const
{
	return IconButton(id, false, bgIconType, bgShapeColor, bgInnerColor, IconType::NoIcon, IM_COL32_BLACK,
	                  IM_COL32_BLACK, size, padding, filled);
}

bool Canvas::IconButton(const std::string id, bool disabled, IconType bgIconType, ImColor bgShapeColor,
                        ImColor bgInnerColor, IconType fgIconType, ImColor fgShapeColor, ImColor fgInnerColor,
                        ImVec2 size, ImVec4 padding, bool filled) const
{
	DrawIcon(bgIconType, bgShapeColor, bgInnerColor, fgIconType, fgShapeColor, fgInnerColor, size, padding, filled);

	//	ImGui::Dummy(size); // Better to use InvisibleButton
	// We're making the InvisibleButton disabled so that when its pressed / dragged it does not set an ActiveID in ImGui
	// Setting ActiveID is the same thing what a DragFloat does when it drags, it disables interaction with other items
	// until the drag/press operation stops. This is not desirable for a pin as we want other things to hover still.
	if (disabled)
		ImGui::BeginDisabled(true);
	bool result = ImGui::InvisibleButton(id.c_str(), size);
	if (disabled)
		ImGui::EndDisabled();
	return result;
}

bool Canvas::IconButton2(const std::string& id, ImVec2 size, bool disabled, IconType bgIconType, IconType fgIconType,
                         const IconStyle& style, const IconStyle& hoveredStyle, const IconStyle& activeStyle) const
{
	ImVec2 initPos = ImGui::GetCursorScreenPos();
	bool hovered, active;
	bool result = DIWNE::DGui::ButtonDummy(id, size, disabled, hovered, active);
	ImVec2 afterPos = ImGui::GetCursorScreenPos();

	// Decide which icon style to used based on button state
	const IconStyle* sp = nullptr;
	if (active)
		sp = &activeStyle;
	else if (hovered)
		sp = &hoveredStyle;
	else
		sp = &style;
	const IconStyle& s = *sp;

	ImGui::SetCursorScreenPos(initPos);
	DrawIcon(bgIconType, s.bgShapeColor, s.bgInnerColor, fgIconType, s.fgShapeColor, s.fgInnerColor, size, s.padding,
	         s.filled);
	ImGui::SetCursorScreenPos(afterPos);

	return result;
}

void Canvas::EmptyButton(ImVec2 size, ImColor color, float rounding /*= 0*/)
{
	ImDrawList* idl = ImGui::GetWindowDrawList();

	const ImVec2 icon_min = ImGui::GetCursorScreenPos();
	const ImVec2 icon_max = icon_min + size;

	idl->AddRectFilled(icon_min, icon_max, color, rounding, ImDrawFlags_RoundCornersAll);
	ImGui::SetCursorScreenPos(icon_min);
}

void Canvas::DrawIcon(IconType bgIconType, ImColor bgShapeColor, ImColor bgInnerColor, IconType fgIconType,
                      ImColor fgShapeColor, ImColor fgInnerColor, ImVec2 size, ImVec4 padding, bool filled,
                      ImVec2 thickness /*=ImVec2(1, 1)*/, float rounding /*= 0*/) const
{
	ImDrawList* idl = ImGui::GetWindowDrawList();

	// TODO: I commented the below code out, it was causing ImGui item / DiwneObject rect / Visual rect misalignment
	//  Centering pins and labels will be separate task to be done later

	// (PF) move Icon slightly lower to match the position of the text
	// todo make it more robust for larger icons?
	//	float h = ImGui::GetTextLineHeight();
	//	float dh = h > size.y ? (h - size.y) / 2.0f : 0.0f;
	//	const ImVec2 icon_min = ImGui::GetCursorScreenPos() + ImVec2(0, dh);

	const ImVec2 icon_min = ImGui::GetCursorScreenPos();

	// const ImVec2 icon_min = ImGui::GetCursorScreenPos();
	const ImVec2 icon_max = icon_min + size;
	const ImVec2 inner_icon_min = icon_min + ImVec2(padding.x, padding.w);
	const ImVec2 inner_icon_max = icon_max - ImVec2(padding.z, padding.y);
	const ImVec2 inner_icon_min2 = icon_min + ImVec2(1.5f * padding.x, 1.5f * padding.w);
	const ImVec2 inner_icon_max2 = icon_max - ImVec2(1.5f * padding.z, 1.5f * padding.y);

	switch (bgIconType)
	{
	case Circle:
		DrawIconCircle(idl, bgShapeColor, bgInnerColor, icon_min, icon_max, filled);
		break;
	case AtFrom:
	case AtTo:
	case Rectangle:
	case Pause:
	case SkipBack:
	case SkipBack2:
	case SkipForward:
	case SkipForward2:
		DrawIconRectangle(idl, bgShapeColor, bgInnerColor, icon_min, icon_max, filled, thickness, rounding);
		break;
	case TriangleLeft:
		DrawIconTriangleLeft(idl, bgShapeColor, bgInnerColor, icon_min, icon_max, filled);
		break;
	case TriangleRight:
		DrawIconTriangleRight(idl, bgShapeColor, bgInnerColor, icon_min, icon_max, filled);
		break;
	case Cross:
		DrawIconCross(idl, bgShapeColor, bgInnerColor, icon_min, icon_max, filled);
		break;
	case Hyphen:
		DrawIconHyphen(idl, bgShapeColor, bgInnerColor, icon_min, icon_max, filled);
		break;
	case TriangleDownLeft:
		DrawIconTriangleDownLeft(idl, bgShapeColor, bgInnerColor, icon_min, icon_max, filled);
		break;
	case TriangleDownRight:
		DrawIconTriangleDownRight(idl, bgShapeColor, bgInnerColor, icon_min, icon_max, filled);
		break;
	case GrabDownLeft:
		DrawIconGrabDownLeft(idl, bgShapeColor, bgInnerColor, icon_min, icon_max, filled);
		break;
	case GrabDownRight:
		DrawIconGrabDownRight(idl, bgShapeColor, bgInnerColor, icon_min, icon_max, filled);
		break;
	case NoIcon:
	default:
		break;
	}

	switch (fgIconType)
	{
	case Circle:
		DrawIconCircle(idl, fgShapeColor, fgInnerColor, inner_icon_min, inner_icon_max, filled);
		break;
	case Rectangle:
		DrawIconRectangle(idl, fgShapeColor, fgInnerColor, inner_icon_min, inner_icon_max, filled);
		break;
	case TriangleLeft:
		DrawIconTriangleLeft(idl, fgShapeColor, fgInnerColor, inner_icon_min, inner_icon_max, filled);
		break;
	case TriangleRight:
		DrawIconTriangleRight(idl, fgShapeColor, fgInnerColor, inner_icon_min, inner_icon_max, filled);
		break;
	case Cross:
		DrawIconCross(idl, fgShapeColor, fgInnerColor, inner_icon_min, inner_icon_max, filled);
		break;
	case Hyphen:
		DrawIconHyphen(idl, fgShapeColor, fgInnerColor, inner_icon_min, inner_icon_max, filled);
		// DrawIconHyphen(idl, fgShapeColor, fgInnerColor, icon_min, icon_max, filled);
		break;
	case TriangleDownLeft:
		DrawIconTriangleDownLeft(idl, fgShapeColor, fgInnerColor, icon_min, icon_max, filled);
		break;
	case TriangleDownRight:

		DrawIconTriangleDownRight(idl, fgShapeColor, fgInnerColor, icon_min, icon_max, filled);
		break;
	case GrabDownLeft:
		DrawIconGrabDownLeft(idl, fgShapeColor, fgInnerColor, icon_min, icon_max, filled);
		break;
	case GrabDownRight:
		DrawIconGrabDownRight(idl, fgShapeColor, fgInnerColor, icon_min, icon_max, filled);
		break;
	case Stop:
		DrawIconRectangle(idl, fgShapeColor, fgInnerColor, inner_icon_min2, inner_icon_max2, filled);
		break;
	case Pause:
		DrawIconPause(idl, fgShapeColor, fgInnerColor, inner_icon_min2, inner_icon_max2, filled);
		break;
	case SkipBack:
		DrawIconSkipBack(idl, fgShapeColor, fgInnerColor, inner_icon_min, inner_icon_max, filled);
		break;
	case SkipBack2:
		DrawIconSkipBack2(idl, fgShapeColor, fgInnerColor, inner_icon_min, inner_icon_max, filled);
		break;
	case SkipForward:
		DrawIconSkipForward(idl, fgShapeColor, fgInnerColor, inner_icon_min, inner_icon_max, filled);
		break;
	case SkipForward2:
		DrawIconSkipForward2(idl, fgShapeColor, fgInnerColor, inner_icon_min, inner_icon_max, filled);
		break;
	case Rewind:
	case AtFrom:
		DrawIconRewind(idl, fgShapeColor, fgInnerColor, inner_icon_min, inner_icon_max, filled);
		break;
	case FastForward:
	case AtTo:
		DrawIconFastForward(idl, fgShapeColor, fgInnerColor, inner_icon_min, inner_icon_max, filled);
		break;
	case NoIcon:
	default:
		break;
	}
}
} // namespace DIWNE