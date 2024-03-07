/**
 * \file
 * \brief
 * \author Jaroslav Holeček <holecek.jaroslav@email.cz>
 * \copyright Copyright (C) 2016-2023 I3T team, Department of Computer Graphics
 * and Interaction, FEE, Czech Technical University in Prague, Czech Republic
 *
 * This file is part of I3T - An Interactive Tool for Teaching Transformations
 * http://www.i3t-tool.org
 *
 * GNU General Public License v3.0 (see LICENSE.txt or https://www.gnu.org/licenses/gpl-3.0.txt)
 */
#include "Logger/Logger.h"
#include "diwne_include.h"

namespace DIWNE
{

/* ===================== */
/* ===== D i w n e ===== */
/* ===================== */

Diwne::Diwne(SettingsDiwne* settingsDiwne)
    : DiwneObject(*this, settingsDiwne->editorId, settingsDiwne->editorlabel), mp_settingsDiwne(settingsDiwne),
      m_workAreaDiwne(settingsDiwne->workAreaDiwne.Min, settingsDiwne->workAreaDiwne.Max),
      m_workAreaZoom(settingsDiwne->workAreaInitialZoom), mp_lastActivePin(nullptr), m_helperLink(diwne, 0),
      m_diwneAction(None), m_diwneAction_previousFrame(m_diwneAction), m_objectFocused(false),
      m_nodesSelectionChanged(false), m_selectionRectangeDiwne(ImRect(0, 0, 0, 0)),
      m_popupPosition(settingsDiwne->initPopupPosition), m_popupDrawn(false), m_tooltipDrawn(false), m_takeSnap(false)
{
	setSelectable(false);
}

DiwneAction Diwne::getDiwneActionActive() const
{
	return m_diwneAction == DiwneAction::None ? m_diwneAction_previousFrame : m_diwneAction;
}

bool Diwne::allowDrawing()
{
	return m_drawing;
}

bool Diwne::initializeDiwne()
{
	m_drawing = ImGui::BeginChild(mp_settingsDiwne->editorlabel.c_str(), ImVec2(0, 0), false,
	                              ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);
	m_diwneAction = DiwneAction::None;
	m_popupDrawn = m_tooltipDrawn = m_objectFocused = m_takeSnap = false;
	return initialize();
}

void ScaleAllSizes(ImGuiStyle& style, float scale_factor)
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
	style.DisplayWindowPadding = style.DisplayWindowPadding * scale_factor;
	style.DisplaySafeAreaPadding = style.DisplaySafeAreaPadding * scale_factor;
	style.MouseCursorScale = style.MouseCursorScale * scale_factor;
}

bool Diwne::beforeBeginDiwne() // todo redesign to https://en.wikipedia.org/wiki/Call_super
{
	updateWorkAreaRectangles();
	m_nodesSelectionChanged = false;
	return beforeBegin();
}

void Diwne::begin()
{
	ImGui::SetCursorScreenPos(m_workAreaScreen.Min);
	ImGui::PushID(mp_settingsDiwne->editorlabel.c_str());
	ImGui::BeginGroup();

#ifdef DIWNE_DEBUG
	ImGui::Checkbox("Show debug texts", &(m_diwneDebug_on));
	DIWNE_DEBUG((*this), {
		ImGui::GetWindowDrawList()->AddRect(m_workAreaScreen.Min, m_workAreaScreen.Max, ImColor(255, 0, 0), 0,
		                                    ImDrawFlags_RoundCornersNone, 10);
		ImGui::Text(fmt::format("\tWADiwne: {}-{}  -  {}-{}\n\tWAScreen: {}-{}  -  {}-{}", m_workAreaDiwne.Min.x,
		                        m_workAreaDiwne.Min.y, m_workAreaDiwne.Max.x, m_workAreaDiwne.Max.y,
		                        m_workAreaScreen.Min.x, m_workAreaScreen.Min.y, m_workAreaScreen.Max.x,
		                        m_workAreaScreen.Max.y)
		                .c_str());

		ImGui::Text(fmt::format("MousePos: {}-{}", ImGui::GetIO().MousePos.x, ImGui::GetIO().MousePos.y).c_str());
		ImGui::Text(fmt::format("PopupPos: {}-{}", getPopupPosition().x, getPopupPosition().y).c_str());
		ImGui::Text(fmt::format("Zoom: {}", m_workAreaZoom).c_str());

		ImGui::Text(
		    fmt::format("WindowPadding: {}_{} ", ImGui::GetStyle().WindowPadding.x, ImGui::GetStyle().WindowPadding.y)
		        .c_str());
		ImGui::Text(fmt::format("WindowClipRect: {} _ {} _ {} _ {} ", ImGui::GetCurrentWindow()->ClipRect.Min.x,
		                        ImGui::GetCurrentWindow()->ClipRect.Min.y, ImGui::GetCurrentWindow()->ClipRect.Max.x,
		                        ImGui::GetCurrentWindow()->ClipRect.Max.y)
		                .c_str());

		switch (m_diwneAction_previousFrame)
		{
		case DiwneAction::None:
			ImGui::Text("PrevDiwneAction: None");
			break;
		case DiwneAction::NewLink:
			ImGui::Text("PrevDiwneAction: NewLink");
			break;
		case DiwneAction::HoldNode:
			ImGui::Text("PrevDiwneAction: HoldNode");
			break;
		case DiwneAction::DragNode:
			ImGui::Text("PrevDiwneAction: DragNode");
			break;
		case DiwneAction::HoldWorkarea:
			ImGui::Text("PrevDiwneAction: HoldWorkarea");
			break;
		case DiwneAction::DragWorkarea:
			ImGui::Text("PrevDiwneAction: DragWorkarea");
			break;
		case DiwneAction::SelectionRectFull:
			ImGui::Text("PrevDiwneAction: SelectionRectFull");
			break;
		case DiwneAction::SelectionRectTouch:
			ImGui::Text("PrevDiwneAction: SelectionRectTouch");
			break;
		case DiwneAction::InteractingContent:
			ImGui::Text("PrevDiwneAction: InteractingContent");
			break;
		case DiwneAction::FocusOnObject:
			ImGui::Text("PrevDiwneAction: FocusOnObject");
			break;
		case DiwneAction::HoldPin:
			ImGui::Text("PrevDiwneAction: HoldPin");
			break;
		case DiwneAction::DragPin:
			ImGui::Text("PrevDiwneAction: DragPin");
			break;
		case DiwneAction::HoldLink:
			ImGui::Text("PrevDiwneAction: HoldLink");
			break;
		case DiwneAction::DragLink:
			ImGui::Text("PrevDiwneAction: DragLink");
			break;
		default:
			ImGui::Text("PrevDiwneAction: Unknown");
		}
	}); /* close of macro */
#endif  // DIWNE_DEBUG
}

bool Diwne::afterContentDiwne()
{
	bool interaction_happen = false;
	if (m_diwneAction == DiwneAction::NewLink)
	{
		interaction_happen |= m_helperLink.drawDiwne(JustDraw);
	}
	interaction_happen |= afterContent();
	return interaction_happen;
}

void Diwne::end()
{
	ImGui::SetCursorScreenPos(m_workAreaScreen.Max); /* for capture whole window/workarea to Group */
	ImGui::EndGroup();
	ImGui::PopID();
}

bool Diwne::afterEndDiwne()
{
	return DiwneObject::afterEndDiwne();
}

bool Diwne::allowProcessFocused()
{
	return m_isActive /* object is active from previous frame */
	       ||
	       (diwne.getDiwneActionActive() == SelectionRectFull || diwne.getDiwneActionActive() == SelectionRectTouch) ||
	       (!diwne.m_objectFocused /* only one object can be focused */
	        && (diwne.getDiwneAction() == None ||
	            diwne.getDiwneActionActive() == NewLink /* we want focus of other object while new link */));
}

bool Diwne::processInteractions()
{
	return processDiwneSelectionRectangle();
}

bool Diwne::processInteractionsDiwne()
{
	bool interaction_happen = false;

	interaction_happen |= DiwneObject::processInteractionsDiwne();

	// for example inner interaction (focus on node) is no problem with this actions
	if (m_drawMode == DrawMode::Interacting && bypassFocusForInteractionAction())
	{
		interaction_happen |= processDiwneZoom();
	}

	return interaction_happen;
}

bool Diwne::finalizeDiwne()
{
	bool interaction_happen = finalize();
	m_diwneAction_previousFrame = m_diwneAction;
	ImGui::EndChild();
	return interaction_happen;
}

bool Diwne::blockRaisePopup()
{
	return m_diwneAction == DiwneAction::SelectionRectFull || m_diwneAction == DiwneAction::SelectionRectTouch ||
	       m_diwneAction_previousFrame == DiwneAction::SelectionRectFull ||
	       m_diwneAction_previousFrame == DiwneAction::SelectionRectTouch;
}

/* be careful for same mouse button in this functions */
bool Diwne::allowProcessSelectionRectangle()
{
	return m_focusedForInteraction;
}
bool Diwne::bypassSelectionRectangleAction()
{
	return bypassIsMouseDragging1();
} /* \todo I suspect bug if dragging start outside of WorkspaceWindow... */
ImVec2 Diwne::bypassDiwneGetSelectionRectangleStartPosition()
{
	return screen2diwne(bypassMouseClickedPos1());
} /* \todo I suspect bug if dragging start outside of WorkspaceWindow... */
ImVec2 Diwne::bypassDiwneGetSelectionRectangleSize()
{
	return bypassGetMouseDragDelta1() / getWorkAreaZoom();
} /* \todo I suspect bug if dragging start outside of WorkspaceWindow... */

bool Diwne::processDiwneSelectionRectangle()
{
	if (bypassSelectionRectangleAction() && allowProcessSelectionRectangle())
	{
		ImVec2 startPos = bypassDiwneGetSelectionRectangleStartPosition();
		ImVec2 dragDelta = bypassDiwneGetSelectionRectangleSize();
		ImColor color;

		if (dragDelta.x > 0)
		{
			setDiwneAction(DiwneAction::SelectionRectFull);
			m_selectionRectangeDiwne.Min.x = startPos.x;
			m_selectionRectangeDiwne.Max.x = startPos.x + dragDelta.x;
			color = mp_settingsDiwne->selectionRectFullColor;
		}
		else
		{
			setDiwneAction(DiwneAction::SelectionRectTouch);
			m_selectionRectangeDiwne.Min.x = startPos.x + dragDelta.x;
			m_selectionRectangeDiwne.Max.x = startPos.x;
			color = mp_settingsDiwne->selectionRectTouchColor;
		}

		if (dragDelta.y > 0)
		{
			m_selectionRectangeDiwne.Min.y = startPos.y;
			m_selectionRectangeDiwne.Max.y = startPos.y + dragDelta.y;
		}
		else
		{
			m_selectionRectangeDiwne.Min.y = startPos.y + dragDelta.y;
			m_selectionRectangeDiwne.Max.y = startPos.y;
		}

		AddRectFilledDiwne(m_selectionRectangeDiwne.Min, m_selectionRectangeDiwne.Max, color);

		return true;
	}
	return false;
}

bool Diwne::processDrag()
{
	translateWorkAreaDiwneZoomed(bypassGetMouseDelta() * -1);
	return true;
}

bool Diwne::processZoom()
{
	ImVec2 mousePosDiwne = screen2diwne(bypassGetMousePos());
	setWorkAreaZoom(m_workAreaZoom + bypassGetZoomDelta());
	translateWorkAreaDiwne(mousePosDiwne - screen2diwne(bypassGetMousePos()));
	return true;
}

bool Diwne::processDiwneZoom()
{
	if (bypassZoomAction() && allowProcessZoom())
	{
		return processZoom();
	}
	return false;
}

void Diwne::updateWorkAreaRectangles()
{
	ImVec2 windowPos = ImGui::GetWindowPos(); /* \todo JH return negative number while sub-window
	                                             can not move outside from aplication window */
	ImVec2 windowSize = ImGui::GetWindowSize();

	m_workAreaScreen.Min = windowPos;
	m_workAreaScreen.Max = windowPos + windowSize;

	m_workAreaDiwne.Max = m_workAreaDiwne.Min + windowSize / m_workAreaZoom;
}

ImVec2 Diwne::transformFromImGuiToDiwne(const ImVec2& point) const
{
	return workArea2screen(screen2workArea(point) / m_workAreaZoom); /* basically just zoom */
}

ImVec2 Diwne::transformFromDiwneToImGui(const ImVec2& point) const
{
	return workArea2screen(screen2workArea(point) * m_workAreaZoom); /* basically just zoom */
}

ImVec4 Diwne::transformFromImGuiToDiwne(const ImVec4& rect) const
{
	ImVec2 const topleft = transformFromImGuiToDiwne(ImVec2(rect.x, rect.y));
	ImVec2 const bottomright = transformFromImGuiToDiwne(ImVec2(rect.z, rect.w));
	return ImVec4(topleft.x, topleft.y, bottomright.x, bottomright.y);
}

ImVec4 Diwne::transformFromDiwneToImGui(const ImVec4& rect) const
{
	ImVec2 const topleft = transformFromDiwneToImGui(ImVec2(rect.x, rect.y));
	ImVec2 const bottomright = transformFromDiwneToImGui(ImVec2(rect.z, rect.w));
	return ImVec4(topleft.x, topleft.y, bottomright.x, bottomright.y);
}

void Diwne::translateWorkAreaDiwneZoomed(ImVec2 const& distance)
{
	translateWorkAreaDiwne(ImVec2(distance.x / m_workAreaZoom, distance.y / m_workAreaZoom));
}

void Diwne::translateWorkAreaDiwne(ImVec2 const& distance)
{
	m_workAreaDiwne.Translate(distance);
}

void Diwne::AddRectFilledDiwne(const ImVec2& p_min, const ImVec2& p_max, ImVec4 col, float rounding /*=0.0f*/,
                               ImDrawFlags rounding_corners /*=ImDrawFlags_RoundCornersAll*/) const
{
	ImDrawList* idl = ImGui::GetWindowDrawList(); /* \todo maybe use other channel with correct
	                                                 Clip rect for drawing of manual shapes, but
	                                                 be careful with order of drew elements */
	float zoom = diwne.getWorkAreaZoom();
	idl->AddRectFilled(diwne2screen(p_min), diwne2screen(p_max), ImGui::ColorConvertFloat4ToU32(col), rounding * zoom,
	                   rounding_corners);
}

void Diwne::AddRectDiwne(const ImVec2& p_min, const ImVec2& p_max, ImVec4 col, float rounding /*=0.0f*/,
                         ImDrawFlags rounding_corners /*=ImDrawFlags_RoundCornersAll*/, float thickness /*=1.0f*/) const
{
	ImDrawList* idl = ImGui::GetWindowDrawList();
	float zoom = diwne.getWorkAreaZoom();
	idl->AddRect(diwne2screen(p_min), diwne2screen(p_max), ImGui::ColorConvertFloat4ToU32(col), rounding * zoom,
	             rounding_corners, thickness * zoom);
}

void Diwne::AddBezierCurveDiwne(const ImVec2& p1, const ImVec2& p2, const ImVec2& p3, const ImVec2& p4, ImVec4 col,
                                float thickness, int num_segments /*=0*/) const
{
	ImDrawList* idl = ImGui::GetWindowDrawList(); /* \todo maybe use other channel with correct
	                                                 Clip rect for drawing of manual shapes, but
	                                                 be careful with order of drew elements */

	idl->AddBezierCubic(diwne2screen(p1), diwne2screen(p2), diwne2screen(p3), diwne2screen(p4),
	                    ImGui::ColorConvertFloat4ToU32(col), thickness * m_workAreaZoom, num_segments);
}

void Diwne::DrawIconCircle(ImDrawList* idl, ImColor shapeColor, ImColor innerColor, ImVec2 topLeft, ImVec2 bottomRight,
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

void Diwne::DrawIconRectangle(ImDrawList* idl, ImColor shapeColor, ImColor innerColor, ImVec2 topLeft,
                              ImVec2 bottomRight, bool filled, ImVec2 thickness /*=ImVec2(1, 1)*/,
                              float rounding /*= 0*/) const
{
	idl->AddRectFilled(topLeft, bottomRight, shapeColor, rounding);
	if (!filled)
	{
		idl->AddRectFilled(topLeft + thickness, bottomRight - thickness, innerColor, rounding);
	}
}
void Diwne::DrawIconPause(ImDrawList* idl, ImColor shapeColor, ImColor innerColor, ImVec2 topLeft, ImVec2 bottomRight,
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

void Diwne::DrawIconTriangleLeft(ImDrawList* idl, ImColor shapeColor, ImColor innerColor, ImVec2 topLeft,
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
void Diwne::DrawIconSkipBack(ImDrawList* idl, ImColor shapeColor, ImColor innerColor, ImVec2 topLeft,
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
void Diwne::DrawIconSkipBack2(ImDrawList* idl, ImColor shapeColor, ImColor innerColor, ImVec2 topLeft,
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
void Diwne::DrawIconRewind(ImDrawList* idl, ImColor shapeColor, ImColor innerColor, ImVec2 topLeft, ImVec2 bottomRight,
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
void Diwne::DrawIconSkipForward(ImDrawList* idl, ImColor shapeColor, ImColor innerColor, ImVec2 topLeft,
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
void Diwne::DrawIconSkipForward2(ImDrawList* idl, ImColor shapeColor, ImColor innerColor, ImVec2 topLeft,
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
void Diwne::DrawIconFastForward(ImDrawList* idl, ImColor shapeColor, ImColor innerColor, ImVec2 topLeft,
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

void Diwne::DrawIconTriangleRight(ImDrawList* idl, ImColor shapeColor, ImColor innerColor, ImVec2 topLeft,
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

void Diwne::DrawIconTriangleDownRight(ImDrawList* idl, ImColor shapeColor, ImColor innerColor, ImVec2 topLeft,
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

void Diwne::DrawIconTriangleDownLeft(ImDrawList* idl, ImColor shapeColor, ImColor innerColor, ImVec2 topLeft,
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

void Diwne::DrawIconGrabDownLeft(ImDrawList* idl, ImColor shapeColor, ImColor innerColor, ImVec2 topLeft,
                                 ImVec2 bottomRight, bool filled, float thickness /*= 1*/) const
{
	topLeft = topLeft + ImVec2(0.5f, 0.5f);
	bottomRight = bottomRight - ImVec2(0.5f, 0.5f);

	int lineCount = 3;
	float padding = 1.5f * m_workAreaZoom;
	float squaredPadding = sqrt(2) * padding;
	float pointOffsetLong = 2 * squaredPadding;
	float pointOffsetShort = padding;
	float size = abs(bottomRight.y - topLeft.y) - pointOffsetLong - pointOffsetShort;
	float step = size / lineCount;

	for (int i = 0; i < lineCount; i++)
	{
		idl->AddLine(ImVec2(bottomRight.x - 1.2f * pointOffsetLong - (i * step), bottomRight.y - pointOffsetShort),
		             ImVec2(topLeft.x + pointOffsetShort, topLeft.y + 1.2f * pointOffsetLong + (i * step)), shapeColor,
		             thickness * m_workAreaZoom);
	}
}

void Diwne::DrawIconGrabDownRight(ImDrawList* idl, ImColor shapeColor, ImColor innerColor, ImVec2 topLeft,
                                  ImVec2 bottomRight, bool filled, float thickness /*= 1*/) const
{
	topLeft = topLeft + ImVec2(0.5f, 0.5f);
	bottomRight = bottomRight - ImVec2(0.5f, 0.5f);

	int lineCount = 3;
	float padding = 1.5f * m_workAreaZoom;
	float squaredPadding = sqrt(2) * padding;
	float pointOffsetLong = 2 * squaredPadding;
	float pointOffsetShort = padding;
	float size = abs(bottomRight.y - topLeft.y) - pointOffsetLong - pointOffsetShort;
	float step = size / lineCount;

	for (int i = 0; i < lineCount; i++)
	{
		idl->AddLine(ImVec2(topLeft.x + 1.2f * pointOffsetLong + (i * step), bottomRight.y - pointOffsetShort),
		             ImVec2(bottomRight.x - pointOffsetShort, topLeft.y + 1.2f * pointOffsetLong + (i * step)),
		             shapeColor, thickness * m_workAreaZoom);
	}
}

void Diwne::DrawIconCross(ImDrawList* idl, ImColor shapeColor, ImColor innerColor, ImVec2 topLeft, ImVec2 bottomRight,
                          bool filled, float shapeThickness /*=4*/, float innerThickness /*=2*/) const
{
	bottomRight = bottomRight - ImVec2(1.0f, 1.0f);

	innerThickness *= m_workAreaZoom;
	shapeThickness *= m_workAreaZoom;

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

	//	LOG_INFO("zoom: {}", m_workAreaZoom);
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

void Diwne::DrawIconHyphen(ImDrawList* idl, ImColor shapeColor, ImColor innerColor, ImVec2 topLeft, ImVec2 bottomRight,
                           bool filled, float thickness) const
{
	bottomRight = bottomRight - ImVec2(1.0f, 1.0f);

	float middleY = (topLeft.y + bottomRight.y) / 2;
	ImVec2 start = ImVec2(topLeft.x, middleY);
	ImVec2 end = ImVec2(bottomRight.x, middleY);


	idl->AddLine(start, end, shapeColor, thickness * m_workAreaZoom);
}

bool Diwne::IconButton(DIWNE::IconType bgIconType, ImColor bgShapeColor, ImColor bgInnerColor, ImVec2 size,
                       ImVec4 padding, bool filled, std::string const id) const
{
	return IconButton(bgIconType, bgShapeColor, bgInnerColor, DIWNE::IconType::NoIcon, IM_COL32_BLACK, IM_COL32_BLACK,
	                  size, padding, filled, id);
}

void Diwne::EmptyButton(ImVec2 size, ImColor color, float rounding /*= 0*/)
{
	ImDrawList* idl = ImGui::GetWindowDrawList();

	const ImVec2 icon_min = ImGui::GetCursorScreenPos();
	const ImVec2 icon_max = icon_min + size;

	idl->AddRectFilled(icon_min, icon_max, color, rounding, ImDrawFlags_RoundCornersAll);
	ImGui::SetCursorScreenPos(icon_min);
}

bool Diwne::IconButton(DIWNE::IconType bgIconType, ImColor bgShapeColor, ImColor bgInnerColor,
                       DIWNE::IconType fgIconType, ImColor fgShapeColor, ImColor fgInnerColor, ImVec2 size,
                       ImVec4 padding, bool filled, std::string const id) const
{
	ImVec2 initPos = ImGui::GetCursorScreenPos();

	DrawIcon(bgIconType, bgShapeColor, bgInnerColor, fgIconType, fgShapeColor, fgInnerColor, size, padding, filled);

	ImGui::SetCursorScreenPos(initPos);
	bool result = ImGui::InvisibleButton(id.c_str(), size);
	//    ImGui::SetItemAllowOverlap();
	return result;
}

void Diwne::DrawIcon(DIWNE::IconType bgIconType, ImColor bgShapeColor, ImColor bgInnerColor, DIWNE::IconType fgIconType,
                     ImColor fgShapeColor, ImColor fgInnerColor, ImVec2 size, ImVec4 padding, bool filled,
                     ImVec2 thickness /*=ImVec2(1, 1)*/, float rounding /*= 0*/) const
{
	ImDrawList* idl = ImGui::GetWindowDrawList();

	// (PF) move Icon slightly lower to match the position of the text
	// todo make it more robust for larger icons?
	float h = ImGui::GetTextLineHeight();
	float dh = h > size.y ? (h - size.y) / 2.0f : 0.0f;
	const ImVec2 icon_min = ImGui::GetCursorScreenPos() + ImVec2(0, dh);

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

	ImGui::Dummy(size);
}

ImVec2 Diwne::screen2workArea(const ImVec2& point) const
{
	return point - m_workAreaScreen.Min;
}

ImVec2 Diwne::workArea2screen(const ImVec2& point) const
{
	return point + m_workAreaScreen.Min;
}

ImVec2 Diwne::diwne2workArea(const ImVec2& point) const
{
	return diwne2workArea_noZoom(point) * m_workAreaZoom;
}

ImVec2 Diwne::workArea2diwne(const ImVec2& point) const
{
	return workArea2diwne_noZoom(point / m_workAreaZoom);
}

ImVec2 Diwne::screen2diwne(const ImVec2& point) const
{
	return workArea2diwne(screen2workArea(point));
}

ImVec2 Diwne::diwne2screen(const ImVec2& point) const
{
	return workArea2screen(diwne2workArea(point));
}

ImVec2 Diwne::diwne2workArea_noZoom(const ImVec2& point) const
{
	return point - m_workAreaDiwne.Min;
}

ImVec2 Diwne::workArea2diwne_noZoom(const ImVec2& point) const
{
	return point + m_workAreaDiwne.Min;
}

ImVec2 Diwne::screen2diwne_noZoom(const ImVec2& point) const
{
	return workArea2diwne_noZoom(screen2workArea(point));
}

ImVec2 Diwne::diwne2screen_noZoom(const ImVec2& point) const
{
	return workArea2screen(diwne2workArea_noZoom(point));
}

bool Diwne::bypassIsItemClicked0()
{
	return ImGui::IsItemClicked(0);
}
bool Diwne::bypassIsItemClicked1()
{
	return ImGui::IsItemClicked(1);
}
bool Diwne::bypassIsItemClicked2()
{
	return ImGui::IsItemClicked(2);
}
bool Diwne::bypassIsMouseDown0()
{
	return ImGui::IsMouseDown(0);
}
bool Diwne::bypassIsMouseDown1()
{
	return ImGui::IsMouseDown(1);
}
bool Diwne::bypassIsMouseDown2()
{
	return ImGui::IsMouseDown(2);
}
bool Diwne::bypassIsMouseClicked0()
{
	return ImGui::IsMouseClicked(0);
}
bool Diwne::bypassIsMouseClicked1()
{
	return ImGui::IsMouseClicked(1);
}
bool Diwne::bypassIsMouseClicked2()
{
	return ImGui::IsMouseClicked(2);
}
bool Diwne::bypassIsMouseReleased0()
{
	return ImGui::IsMouseReleased(0);
}
bool Diwne::bypassIsMouseReleased1()
{
	return ImGui::IsMouseReleased(1);
}
bool Diwne::bypassIsMouseReleased2()
{
	return ImGui::IsMouseReleased(2);
}
ImVec2 Diwne::bypassMouseClickedPos0()
{
	return ImGui::GetIO().MouseClickedPos[0];
}
ImVec2 Diwne::bypassMouseClickedPos1()
{
	return ImGui::GetIO().MouseClickedPos[1];
}
ImVec2 Diwne::bypassMouseClickedPos2()
{
	return ImGui::GetIO().MouseClickedPos[2];
}
bool Diwne::bypassIsItemActive()
{
	return ImGui::IsItemActive();
}
bool Diwne::bypassIsMouseDragging0()
{
	return ImGui::IsMouseDragging(0);
}
bool Diwne::bypassIsMouseDragging1()
{
	return ImGui::IsMouseDragging(1);
}
bool Diwne::bypassIsMouseDragging2()
{
	return ImGui::IsMouseDragging(2);
}
ImVec2 Diwne::bypassGetMouseDragDelta0()
{
	return ImGui::GetMouseDragDelta(0);
}
ImVec2 Diwne::bypassGetMouseDragDelta1()
{
	return ImGui::GetMouseDragDelta(1);
}
ImVec2 Diwne::bypassGetMouseDragDelta2()
{
	return ImGui::GetMouseDragDelta(2);
}
ImVec2 Diwne::bypassGetMouseDelta()
{
	return ImGui::GetIO().MouseDelta;
}
ImVec2 Diwne::bypassGetMousePos()
{
	return ImGui::GetIO().MousePos;
}
float Diwne::bypassGetMouseWheel()
{
	return ImGui::GetIO().MouseWheel;
}
float Diwne::bypassGetZoomDelta()
{
	return bypassGetMouseWheel() / mp_settingsDiwne->zoomWheelReverseSenzitivity;
}

bool Diwne::allowProcessZoom()
{
	return true;
}
bool Diwne::bypassZoomAction()
{
	return diwne.bypassGetZoomDelta() != 0;
}
bool Diwne::bypassDiwneSetPopupPositionAction()
{
	return bypassIsMouseClicked1();
}
ImVec2 Diwne::bypassDiwneGetPopupNewPositionAction()
{
	return bypassGetMousePos();
}

ImRect Diwne::getSelectionRectangleDiwne()
{
	return m_selectionRectangeDiwne;
}

void Diwne::setWorkAreaZoom(float val /*=1*/)
{
	double old = m_workAreaZoom;
	if (val < mp_settingsDiwne->minWorkAreaZoom)
	{
		m_workAreaZoom = mp_settingsDiwne->minWorkAreaZoom;
	}
	else if (val > mp_settingsDiwne->maxWorkAreaZoom)
	{
		m_workAreaZoom = mp_settingsDiwne->maxWorkAreaZoom;
	}
	else
		m_workAreaZoom = val;
}

bool Diwne::applyZoomScaling()
{
	if (m_zoomScalingApplied)
		return true;

	// Fringe scale can stay at 1 (default), it's a parameter that specifies how "blurry" anti aliased lines/shapes are
	// ImGui::GetCurrentWindow()->DrawList->_FringeScale = 1 / m_workAreaZoom;
	// ImGui::SetWindowFontScale(m_workAreaZoom);

	/// \todo
	auto* font = ImGui::GetFont();
	if (!font)
	{
		return false;
	}

	m_zoomOriginalFontScale = applyZoomScalingToFont(font);
	ImGui::PushFont(font);

	//	static int d = 0;
	//	int dMax = 120;
	//	d++;

	// Scale the whole ImGui style, will be restored later
	m_zoomOriginalStyle = ImGui::GetStyle();
	//	ImGui::GetStyle().ScaleAllSizes(d > dMax ? m_workAreaZoom : 1.0f);
	// ScaleAllSizes(ImGui::GetStyle(), d > dMax ? m_workAreaZoom : 1.0f);
	ScaleAllSizes(ImGui::GetStyle(), m_workAreaZoom);

	//	d %= dMax * 2;

	m_zoomScalingApplied = true;
	return false;
}

bool Diwne::restoreZoomScaling()
{
	if (!m_zoomScalingApplied)
		return false;

	ImGui::GetCurrentContext()->Style = m_zoomOriginalStyle;

	// Need to reset default font BEFORE popping font
	restoreZoomScalingToFont(ImGui::GetFont(), m_zoomOriginalFontScale);
	ImGui::PopFont();

	m_zoomScalingApplied = false;
	return true;
}

float Diwne::applyZoomScalingToFont(ImFont* font, ImFont* largeFont)
{
	if (!font)
	{
		return 1.0f;
	}

	ImFont* f = font;
	if (largeFont != nullptr)
	{
		f = largeFont;
	}
	float originalScale = f->Scale;
	f->Scale = m_workAreaZoom;
	ImGui::PushFont(f);

	return originalScale;

	// TODO: Test dynamic font switching based on zoom level

	// Temporary font idea from imgui password impl
	//	const ImFontGlyph* glyph = g.Font->FindGlyph('*');
	//	ImFont* password_font = &g.InputTextPasswordFont;
	//	password_font->FontSize = g.Font->FontSize;
	//	password_font->Scale = g.Font->Scale;
	//	password_font->Ascent = g.Font->Ascent;
	//	password_font->Descent = g.Font->Descent;
	//	password_font->ContainerAtlas = g.Font->ContainerAtlas;
	//	password_font->FallbackGlyph = glyph;
	//	password_font->FallbackAdvanceX = glyph->AdvanceX;
	//	IM_ASSERT(password_font->Glyphs.empty() && password_font->IndexAdvanceX.empty() &&
	// password_font->IndexLookup.empty()); 	PushFont(password_font);
}

void Diwne::restoreZoomScalingToFont(ImFont* font, float originalScale)
{
	font->Scale = originalScale;
	ImGui::PopFont();
}

bool Diwne::ensureZoomScaling(bool active)
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
			restoreZoomScaling();
		}
	}
	return activeBefore;
}

} /* namespace DIWNE */
