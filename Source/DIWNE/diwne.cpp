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
	m_selectable = false;
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
		                                    ImDrawCornerFlags_None, 10);
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
                               ImDrawCornerFlags rounding_corners /*=ImDrawCornerFlags_All*/) const
{
	ImDrawList* idl = ImGui::GetWindowDrawList(); /* \todo maybe use other channel with correct
	                                                 Clip rect for drawing of manual shapes, but
	                                                 be careful with order of drew elements */
	float zoom = diwne.getWorkAreaZoom();
	idl->AddRectFilled(diwne2screen(p_min), diwne2screen(p_max), ImGui::ColorConvertFloat4ToU32(col), rounding * zoom,
	                   rounding_corners);
}

void Diwne::AddRectDiwne(const ImVec2& p_min, const ImVec2& p_max, ImVec4 col, float rounding /*=0.0f*/,
                         ImDrawCornerFlags rounding_corners /*=ImDrawCornerFlags_All*/, float thickness /*=1.0f*/) const
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

	idl->AddBezierCurve(diwne2screen(p1), diwne2screen(p2), diwne2screen(p3), diwne2screen(p4),
	                    ImGui::ColorConvertFloat4ToU32(col), thickness * m_workAreaZoom, num_segments);
}

void Diwne::DrawIconCircle(ImDrawList* idl, ImColor ShapeColor, ImColor InnerColor, ImVec2 topLeft, ImVec2 bottomRight,
                           bool filled, float thicknes /*=1*/) const
{
	ImVec2 center = ImVec2((topLeft.x + bottomRight.x) / 2, (topLeft.y + bottomRight.y) / 2);
	float radius = std::min(bottomRight.x - topLeft.x, bottomRight.y - topLeft.y) / 2;

	idl->AddCircleFilled(center, radius, ShapeColor);
	if (!filled)
	{
		idl->AddCircleFilled(center, radius - thicknes, InnerColor);
	}
}

void Diwne::DrawIconRectangle(ImDrawList* idl, ImColor ShapeColor, ImColor InnerColor, ImVec2 topLeft,
                              ImVec2 bottomRight, bool filled, ImVec2 thicknes /*=ImVec2(1, 1)*/,
                              float rounding /*= 0*/) const
{
	idl->AddRectFilled(topLeft, bottomRight, ShapeColor, rounding);
	if (!filled)
	{
		idl->AddRectFilled(topLeft + thicknes, bottomRight - thicknes, InnerColor, rounding);
	}
}

void Diwne::DrawIconTriangleLeft(ImDrawList* idl, ImColor ShapeColor, ImColor InnerColor, ImVec2 topLeft,
                                 ImVec2 bottomRight, bool filled, float thicknes /*= 1*/) const
{
	idl->AddTriangleFilled(ImVec2(bottomRight.x, topLeft.y), ImVec2(topLeft.x, (topLeft.y + bottomRight.y) / 2),
	                       bottomRight, ShapeColor);
	if (!filled)
	{
		idl->AddTriangleFilled(ImVec2(bottomRight.x - thicknes, topLeft.y + thicknes),
		                       ImVec2(topLeft.x + thicknes, (topLeft.y + bottomRight.y) / 2),
		                       bottomRight - ImVec2(thicknes, thicknes), InnerColor);
	}
}

void Diwne::DrawIconTriangleRight(ImDrawList* idl, ImColor ShapeColor, ImColor InnerColor, ImVec2 topLeft,
                                  ImVec2 bottomRight, bool filled, float thicknes /*= 1*/) const
{
	idl->AddTriangleFilled(topLeft, ImVec2(bottomRight.x, (topLeft.y + bottomRight.y) / 2),
	                       ImVec2(topLeft.x, bottomRight.y), ShapeColor);
	if (!filled)
	{
		idl->AddTriangleFilled(topLeft + ImVec2(thicknes, thicknes),
		                       ImVec2(bottomRight.x - thicknes, (topLeft.y + bottomRight.y) / 2),
		                       ImVec2(topLeft.x + thicknes, bottomRight.y - thicknes), InnerColor);
	}
}

void Diwne::DrawIconTriangleDownRight(ImDrawList* idl, ImColor ShapeColor, ImColor InnerColor, ImVec2 topLeft,
                                      ImVec2 bottomRight, bool filled, float thicknes /*= 1*/) const
{
	ImVec2 p1 = ImVec2(topLeft.x + 0.5f, bottomRight.y - 0.5f);
	ImVec2 p2 = bottomRight - ImVec2(0.5f, 0.5f);
	ImVec2 p3 = ImVec2(bottomRight.x - 0.5f, topLeft.y + 0.5f);

	idl->AddTriangleFilled(p1, p2, p3, ShapeColor);
	if (!filled)
	{
		idl->AddTriangleFilled(p1 + ImVec2(2 * thicknes, -thicknes), p2 - ImVec2(thicknes, thicknes),
		                       p3 + ImVec2(-thicknes, 2 * thicknes), InnerColor);
	}
}

void Diwne::DrawIconTriangleDownLeft(ImDrawList* idl, ImColor ShapeColor, ImColor InnerColor, ImVec2 topLeft,
                                     ImVec2 bottomRight, bool filled, float thicknes /*= 1*/) const
{
	ImVec2 p1 = topLeft + ImVec2(0.5f, 0.5f);
	ImVec2 p2 = ImVec2(topLeft.x + 0.5f, bottomRight.y - 0.5f);
	ImVec2 p3 = bottomRight - ImVec2(0.5f, 0.5f);

	idl->AddTriangleFilled(p1, p2, p3, ShapeColor);
	if (!filled)
	{
		idl->AddTriangleFilled(p1 + ImVec2(thicknes, 2 * thicknes), p2 + ImVec2(thicknes, -thicknes),
		                       p3 - ImVec2(2 * thicknes, thicknes), InnerColor);
	}
}

void Diwne::DrawIconGrabDownLeft(ImDrawList* idl, ImColor ShapeColor, ImColor InnerColor, ImVec2 topLeft,
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
		             ImVec2(topLeft.x + pointOffsetShort, topLeft.y + 1.2f * pointOffsetLong + (i * step)), ShapeColor,
		             thickness * m_workAreaZoom);
	}
}

void Diwne::DrawIconGrabDownRight(ImDrawList* idl, ImColor ShapeColor, ImColor InnerColor, ImVec2 topLeft,
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
		             ShapeColor, thickness * m_workAreaZoom);
	}
}

void Diwne::DrawIconCross(ImDrawList* idl, ImColor ShapeColor, ImColor InnerColor, ImVec2 topLeft, ImVec2 bottomRight,
                          bool filled, float thicknesShape /*=4*/, float thicknesInner /*=2*/) const
{
	bottomRight = bottomRight - ImVec2(1.0f, 1.0f);

	thicknesInner *= m_workAreaZoom;
	thicknesShape *= m_workAreaZoom;

	ImVec2 pTL = topLeft;
	ImVec2 pBR = bottomRight;
	ImVec2 pTR = ImVec2(bottomRight.x, topLeft.y);
	ImVec2 pBL = ImVec2(topLeft.x, bottomRight.y);

	// (DR): This is janky, but it seems to help making the cross look more symmetrical
	//   The true reason for the asymmetry probably lies in the zoom level value, on my machine it steps in 0.125
	//   intervals, every other zoom level is fine, the ones between can cause asymmetry, might be a good idea to
	//   investigate this further and maybe restrict what values the zoom can have
	pTR = pTR + ImVec2(0.01f, -0.01f);
	pBR = pBR + ImVec2(0.01f, 0.01f);

	//	LOG_INFO("zoom: {}", m_workAreaZoom);
	//	LOG_INFO("pTL: {:10.3f},{:10.3f}  pBR: {:10.3f},{:10.3f}", pTL.x, pTL.y, pBR.x, pBR.y);
	idl->AddLine(pTL, pBR, ShapeColor, thicknesShape);
	idl->AddLine(pBL, pTR, ShapeColor, thicknesShape);

	// TODO: (DR) Commenting this out for the time being as it isn't used anyway, needs to be rewritten to support
	//   zooming like the code above
	//	float thicknesDiff = (thicknesShape - thicknesInner) / 2;
	//	ImVec2 thicknesDiffVec = ImVec2(thicknesDiff, thicknesDiff);
	//	if (!filled)
	//	{
	//				idl->AddLine(topLeft + thicknesDiffVec, bottomRight - thicknesDiffVec, InnerColor, thicknesInner);
	//		idl->AddLine(ImVec2(topLeft.x, bottomRight.y) + thicknesDiffVec,
	//		             ImVec2(bottomRight.x, topLeft.y) - thicknesDiffVec, InnerColor, thicknesInner);
	//	}
}

bool Diwne::IconButton(DIWNE::IconType bgIconType, ImColor bgShapeColor, ImColor bgInnerColor, ImVec2 size,
                       ImVec4 padding, bool filled, std::string const id) const
{
	return IconButton(bgIconType, bgShapeColor, bgInnerColor, DIWNE::IconType::NoIcon, IM_COL32_BLACK, IM_COL32_BLACK,
	                  size, padding, filled, id);
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
                     ImColor fgShapeColor, ImColor fgInnerColor, ImVec2 size, ImVec4 padding, bool filled) const
{
	ImDrawList* idl = ImGui::GetWindowDrawList();
	ImVec2 icon_min = ImGui::GetCursorScreenPos();
	ImVec2 icon_max = icon_min + size;
	ImVec2 inner_icon_min = icon_min + ImVec2(padding.x, padding.w);
	ImVec2 inner_icon_max = icon_max - ImVec2(padding.z, padding.y);

	switch (bgIconType)
	{
	case Circle:
		DrawIconCircle(idl, bgShapeColor, bgInnerColor, icon_min, icon_max, filled);
		break;
	case Rectangle:
		DrawIconRectangle(idl, bgShapeColor, bgInnerColor, icon_min, icon_max, filled);
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

bool Diwne::applyZoomScaling()
{
	if (m_zoomScalingApplied)
		return true;

	// Fringe scale can stay at 1 (default), it's a parameter that specifies how "blurry" anti aliased lines/shapes are
	// ImGui::GetCurrentWindow()->DrawList->_FringeScale = 1 / m_workAreaZoom;
	// ImGui::SetWindowFontScale(m_workAreaZoom);
	m_zoomOriginalFontScale = ImGui::GetFont()->Scale;
	ImGui::GetFont()->Scale = m_workAreaZoom;
	ImGui::PushFont(ImGui::GetFont());

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
	// ImGui::GetStyle().ItemSpacing = m_StoreItemSpacing;

	ImGui::GetFont()->Scale = m_zoomOriginalFontScale; // Need to reset default font BEFORE popping font
	ImGui::PopFont();

	m_zoomScalingApplied = false;
	return true;
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
