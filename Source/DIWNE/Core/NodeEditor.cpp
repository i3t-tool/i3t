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
#include "NodeEditor.h"

#include "Logger/Logger.h"

#include "Link.h"
#include "Node.h"
#include "Pin.h"

// TODO: REMOVE <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
#include "GUI/Workspace/Nodes/Basic/CoreNode.h"

namespace DIWNE
{
NodeEditor::NodeEditor(SettingsDiwne* settingsDiwne)
    : DiwneObject(*this, settingsDiwne->editorlabel), mp_settingsDiwne(settingsDiwne),
      m_workAreaDiwne(settingsDiwne->workAreaDiwne.Min, settingsDiwne->workAreaDiwne.Max),
      m_workAreaZoom(settingsDiwne->workAreaInitialZoom), m_helperLink(std::make_unique<Link>(diwne)),
      m_popupPosition(settingsDiwne->initPopupPosition), m_renderer(std::make_unique<DrawHelper>(this))
{
	setSelectable(false);
}

NodeEditor::~NodeEditor()
{
	clear();
}

void NodeEditor::draw(DrawMode drawMode)
{
	DiwneObject::draw(drawMode);
}

void NodeEditor::initializeDiwne(DrawInfo& context)
{
	m_diwneAction = DiwneAction::None;
	m_popupDrawn = m_tooltipDrawn = m_objectFocused = m_takeSnap = false;
	DiwneObject::initializeDiwne(context);
}

void NodeEditor::begin(DrawInfo& context)
{
	ImGui::BeginChild(this->m_labelDiwne.c_str(), ImVec2(0, 0), false,
	                  ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoMove);

	updateWorkAreaRectangles();
	m_nodesSelectionChanged = false;

	ImGui::SetCursorScreenPos(m_workAreaScreen.Min);
	ImGui::PushID(m_labelDiwne.c_str()); // TODO: Is this necessary? We already do this in the Beign child no?
	ImGui::BeginGroup();

	applyZoomScaling();

	DIWNE_DEBUG_EXTRA_2((*this), {
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
	});
}

void NodeEditor::content(DrawInfo& context)
{
	// Debug work area rect
	ImGui::GetWindowDrawList()->AddRect(m_workAreaScreen.Min, m_workAreaScreen.Max, ImColor(255, 0, 0, 255));

	// NOTE: Nodes are "constructed" front to back, eg. the first UI code to run is from the TOP node, however the nodes
	//  are then later drawn by ImGui back to front because we reorder the ImGui draw commands using a channel splitter.
	// This is desirable as when ImGui elements overlap, the first element to receive input is the FIRST one that's
	//  submitted to ImGui, as it is able to "grab" the input first, the later elements which are drawn on top of it
	//  are not the ones to receive input even though they're "FIRST" in the sense they're on top and drawn last.
	// This is rather counterintuitive and a direct result of the immediate mode methodology. There is a way around
	//  this in ImGui in the form of ImGui::SetNextItemAllowOverlap() but it is a tricky multi-frame workaround.

	int number_of_nodes = m_workspaceCoreNodes.size();
	int node_count = number_of_nodes - 1; /* -1 for space for top node drawn above links */
	                                      // TODO: Wait what? So if there is just one node it's not rendered?
	if (number_of_nodes > 0)
	{
		m_channelSplitter.Split(ImGui::GetWindowDrawList(), number_of_nodes + 1 /*+1 for links channel on top */);

		/* draw nodes from back to begin (front to back) to catch interactions in
		 * correct order */
		int prev_size = m_workspaceCoreNodes.size();
		bool takeSnap = false;
		for (auto it = m_workspaceCoreNodes.rbegin(); it != m_workspaceCoreNodes.rend(); ++it)
		{
			if (it == m_workspaceCoreNodes.rbegin()) /* node on top */
			{
				m_channelSplitter.SetCurrentChannel(ImGui::GetWindowDrawList(),
				                                    number_of_nodes); /* top node is above links */
			}
			else
			{
				m_channelSplitter.SetCurrentChannel(ImGui::GetWindowDrawList(), --node_count);
			}

			if ((*it) != nullptr)
			{
				// TODO: Viz related TODO comment above the drawNodeDiwne method (eg. why not call node->drawDiwne()?)
				(*it)->drawNodeDiwne<Node>(context, DIWNE::DrawModeNodePosition::OnItsPosition);
			}

			// TODO: This seems like a bit of a "hacky" solution here, we just don't draw the rest of nodes after a
			//  sequence that just lost or gained a subnode? Investigate, the node list can simply just be copied
			//  beforehand or the insertion/deletion deferred after rendering, no?
			if (prev_size != m_workspaceCoreNodes.size())
				break; /* when push/pop to/from Sequence size of m_workspaceCoreNodes is
				          affected and iterator is invalidated (at least with MVSC) */
		}

		// TODO: Generally in other node editors it seems links are ALWAYS drawn BELOW nodes, simply because having a
		//  node overlaid with a link is disruptive and a link popping in front or behind a node suddenly is not
		//  desirable. So maybe draw links as very first thing

		/* draw links under last (on top) node */
		// TODO: UNCOMMENT! Will be handling link drawing later <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
		//		m_channelSplitter.SetCurrentChannel(ImGui::GetWindowDrawList(), number_of_nodes - 1);
		//		for (auto link : m_linksToDraw)
		//		{
		//			link->drawDiwne(context);
		//		}

		m_channelSplitter.Merge(ImGui::GetWindowDrawList());

		DIWNE_DEBUG((*this), {
			float zoom = getWorkAreaZoom();
			ImGui::GetWindowDrawList()->AddCircleFilled(diwne2screen(ImVec2(0, 0)), 10.f * zoom,
			                                            IM_COL32(255, 0, 255, 150));
		});
	}
}

void NodeEditor::end(DrawInfo& context)
{
	bool interaction_happen = false;
	if (m_diwneAction == DiwneAction::NewLink)
	{
		m_helperLink->drawDiwne(context, DrawMode::JustDraw);
	}

	diwne.restoreZoomScaling();

	// TODO: This doesnt work anymore in newer ImGui versions, a dummy needs to be added
	ImGui::SetCursorScreenPos(m_workAreaScreen.Max); /* for capture whole window/workarea to Group */
	ImGui::EndGroup();
	ImGui::PopID();
	ImGui::EndChild();
}

void NodeEditor::clear()
{
	setLastActiveNode<DIWNE::Node>(nullptr);
	setLastActivePin<DIWNE::Pin>(nullptr);
	m_workspaceCoreNodes.clear();
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

bool NodeEditor::allowProcessFocused()
{
	return m_isActive /* object is active from previous frame */
	       ||
	       (diwne.getDiwneActionActive() == SelectionRectFull || diwne.getDiwneActionActive() == SelectionRectTouch) ||
	       (!diwne.m_objectFocused /* only one object can be focused */
	        && (diwne.getDiwneAction() == None ||
	            diwne.getDiwneActionActive() == NewLink /* we want focus of other object while new link */));
}

// bool NodeEditor::processInteractions()
//{
//	return processDiwneSelectionRectangle();
// }
//
// bool NodeEditor::processInteractionsDiwne()
//{
//	bool interaction_happen = false;
//
//	interaction_happen |= DiwneObject::processInteractionsDiwne();
//
//	// for example inner interaction (focus on node) is no problem with this actions
//	if (m_drawMode == DrawMode::Interacting && bypassFocusForInteractionAction())
//	{
//		interaction_happen |= processDiwneZoom();
//	}
//
//	return interaction_happen;
// }

void NodeEditor::finalizeDiwne(DrawInfo& context)
{
	DiwneObject::finalizeDiwne(context);
	m_diwneAction_previousFrame = m_diwneAction; // TODO: This might end up being removed
}

bool NodeEditor::blockRaisePopup()
{
	return m_diwneAction == DiwneAction::SelectionRectFull || m_diwneAction == DiwneAction::SelectionRectTouch ||
	       m_diwneAction_previousFrame == DiwneAction::SelectionRectFull ||
	       m_diwneAction_previousFrame == DiwneAction::SelectionRectTouch;
}

DiwneAction NodeEditor::getDiwneActionActive() const
{
	return m_diwneAction == DiwneAction::None ? m_diwneAction_previousFrame : m_diwneAction;
}

DIWNE::Link& NodeEditor::getHelperLink()
{
	return *m_helperLink;
}

/* be careful for same mouse button in this functions */
bool NodeEditor::allowProcessSelectionRectangle()
{
	return m_focusedForInteraction;
}
bool NodeEditor::bypassSelectionRectangleAction()
{
	return bypassIsMouseDragging1();
} /* \todo I suspect bug if dragging start outside of WorkspaceWindow... */
ImVec2 NodeEditor::bypassDiwneGetSelectionRectangleStartPosition()
{
	return screen2diwne(bypassMouseClickedPos1());
} /* \todo I suspect bug if dragging start outside of WorkspaceWindow... */
ImVec2 NodeEditor::bypassDiwneGetSelectionRectangleSize()
{
	return bypassGetMouseDragDelta1() / getWorkAreaZoom();
} /* \todo I suspect bug if dragging start outside of WorkspaceWindow... */

bool NodeEditor::processDiwneSelectionRectangle()
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

		m_renderer->AddRectFilledDiwne(m_selectionRectangeDiwne.Min, m_selectionRectangeDiwne.Max, color);

		return true;
	}
	return false;
}

// bool NodeEditor::processDrag()
//{
//	translateWorkAreaDiwneZoomed(bypassGetMouseDelta() * -1);
//	return true;
// }

bool NodeEditor::processZoom()
{
	ImVec2 mousePosDiwne = screen2diwne(bypassGetMousePos());
	setWorkAreaZoom(m_workAreaZoom + bypassGetZoomDelta());
	translateWorkAreaDiwne(mousePosDiwne - screen2diwne(bypassGetMousePos()));
	return true;
}

bool NodeEditor::processDiwneZoom()
{
	if (bypassZoomAction() && allowProcessZoom())
	{
		return processZoom();
	}
	return false;
}

void NodeEditor::updateWorkAreaRectangles()
{
	ImVec2 windowPos = ImGui::GetWindowPos(); /* \todo JH return negative number while sub-window
	                                             can not move outside from aplication window */
	ImVec2 windowSize = ImGui::GetWindowSize();

	m_workAreaScreen.Min = windowPos;
	m_workAreaScreen.Max = windowPos + windowSize;

	m_workAreaDiwne.Max = m_workAreaDiwne.Min + windowSize / m_workAreaZoom;
}

ImVec2 NodeEditor::transformFromImGuiToDiwne(const ImVec2& point) const
{
	return workArea2screen(screen2workArea(point) / m_workAreaZoom); /* basically just zoom */
}

ImVec2 NodeEditor::transformFromDiwneToImGui(const ImVec2& point) const
{
	return workArea2screen(screen2workArea(point) * m_workAreaZoom); /* basically just zoom */
}

ImVec4 NodeEditor::transformFromImGuiToDiwne(const ImVec4& rect) const
{
	ImVec2 const topleft = transformFromImGuiToDiwne(ImVec2(rect.x, rect.y));
	ImVec2 const bottomright = transformFromImGuiToDiwne(ImVec2(rect.z, rect.w));
	return ImVec4(topleft.x, topleft.y, bottomright.x, bottomright.y);
}

ImVec4 NodeEditor::transformFromDiwneToImGui(const ImVec4& rect) const
{
	ImVec2 const topleft = transformFromDiwneToImGui(ImVec2(rect.x, rect.y));
	ImVec2 const bottomright = transformFromDiwneToImGui(ImVec2(rect.z, rect.w));
	return ImVec4(topleft.x, topleft.y, bottomright.x, bottomright.y);
}

void NodeEditor::translateWorkAreaDiwneZoomed(ImVec2 const& distance)
{
	translateWorkAreaDiwne(ImVec2(distance.x / m_workAreaZoom, distance.y / m_workAreaZoom));
}

void NodeEditor::translateWorkAreaDiwne(ImVec2 const& distance)
{
	m_workAreaDiwne.Translate(distance);
}

ImVec2 NodeEditor::screen2workArea(const ImVec2& point) const
{
	return point - m_workAreaScreen.Min;
}

ImVec2 NodeEditor::workArea2screen(const ImVec2& point) const
{
	return point + m_workAreaScreen.Min;
}

ImVec2 NodeEditor::diwne2workArea(const ImVec2& point) const
{
	return diwne2workArea_noZoom(point) * m_workAreaZoom;
}

ImVec2 NodeEditor::workArea2diwne(const ImVec2& point) const
{
	return workArea2diwne_noZoom(point / m_workAreaZoom);
}

ImVec2 NodeEditor::screen2diwne(const ImVec2& point) const
{
	return workArea2diwne(screen2workArea(point));
}

ImVec2 NodeEditor::diwne2screen(const ImVec2& point) const
{
	return workArea2screen(diwne2workArea(point));
}

ImVec2 NodeEditor::diwne2workArea_noZoom(const ImVec2& point) const
{
	return point - m_workAreaDiwne.Min;
}

ImVec2 NodeEditor::workArea2diwne_noZoom(const ImVec2& point) const
{
	return point + m_workAreaDiwne.Min;
}

ImVec2 NodeEditor::screen2diwne_noZoom(const ImVec2& point) const
{
	return workArea2diwne_noZoom(screen2workArea(point));
}

ImVec2 NodeEditor::diwne2screen_noZoom(const ImVec2& point) const
{
	return workArea2screen(diwne2workArea_noZoom(point));
}

bool NodeEditor::bypassIsItemClicked0()
{
	return ImGui::IsItemClicked(0);
}
bool NodeEditor::bypassIsItemClicked1()
{
	return ImGui::IsItemClicked(1);
}
bool NodeEditor::bypassIsItemClicked2()
{
	return ImGui::IsItemClicked(2);
}
bool NodeEditor::bypassIsMouseDown0()
{
	return ImGui::IsMouseDown(0);
}
bool NodeEditor::bypassIsMouseDown1()
{
	return ImGui::IsMouseDown(1);
}
bool NodeEditor::bypassIsMouseDown2()
{
	return ImGui::IsMouseDown(2);
}
bool NodeEditor::bypassIsMouseClicked0()
{
	return ImGui::IsMouseClicked(0);
}
bool NodeEditor::bypassIsMouseClicked1()
{
	return ImGui::IsMouseClicked(1);
}
bool NodeEditor::bypassIsMouseClicked2()
{
	return ImGui::IsMouseClicked(2);
}
bool NodeEditor::bypassIsMouseReleased0()
{
	return ImGui::IsMouseReleased(0);
}
bool NodeEditor::bypassIsMouseReleased1()
{
	return ImGui::IsMouseReleased(1);
}
bool NodeEditor::bypassIsMouseReleased2()
{
	return ImGui::IsMouseReleased(2);
}
ImVec2 NodeEditor::bypassMouseClickedPos0()
{
	return ImGui::GetIO().MouseClickedPos[0];
}
ImVec2 NodeEditor::bypassMouseClickedPos1()
{
	return ImGui::GetIO().MouseClickedPos[1];
}
ImVec2 NodeEditor::bypassMouseClickedPos2()
{
	return ImGui::GetIO().MouseClickedPos[2];
}
bool NodeEditor::bypassIsItemActive()
{
	return ImGui::IsItemActive();
}
bool NodeEditor::bypassIsMouseDragging0()
{
	return ImGui::IsMouseDragging(0);
}
bool NodeEditor::bypassIsMouseDragging1()
{
	return ImGui::IsMouseDragging(1);
}
bool NodeEditor::bypassIsMouseDragging2()
{
	return ImGui::IsMouseDragging(2);
}
ImVec2 NodeEditor::bypassGetMouseDragDelta0()
{
	return ImGui::GetMouseDragDelta(0);
}
ImVec2 NodeEditor::bypassGetMouseDragDelta1()
{
	return ImGui::GetMouseDragDelta(1);
}
ImVec2 NodeEditor::bypassGetMouseDragDelta2()
{
	return ImGui::GetMouseDragDelta(2);
}
ImVec2 NodeEditor::bypassGetMouseDelta()
{
	return ImGui::GetIO().MouseDelta;
}
ImVec2 NodeEditor::bypassGetMousePos()
{
	return ImGui::GetIO().MousePos;
}
float NodeEditor::bypassGetMouseWheel()
{
	return ImGui::GetIO().MouseWheel;
}
float NodeEditor::bypassGetZoomDelta()
{
	return bypassGetMouseWheel() / mp_settingsDiwne->zoomWheelReverseSenzitivity;
}

bool NodeEditor::allowProcessZoom()
{
	return true;
}
bool NodeEditor::bypassZoomAction()
{
	return diwne.bypassGetZoomDelta() != 0;
}
bool NodeEditor::bypassDiwneSetPopupPositionAction()
{
	return bypassIsMouseClicked1();
}
ImVec2 NodeEditor::bypassDiwneGetPopupNewPositionAction()
{
	return bypassGetMousePos();
}

ImRect NodeEditor::getSelectionRectangleDiwne()
{
	return m_selectionRectangeDiwne;
}

void NodeEditor::setWorkAreaZoom(float val /*=1*/)
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

bool NodeEditor::applyZoomScaling()
{
	if (m_zoomScalingApplied)
		return true;

	// Fringe scale can stay at 1 (default), it's a parameter that specifies how "blurry" anti aliased lines/shapes
	// are ImGui::GetCurrentWindow()->DrawList->_FringeScale = 1 / m_workAreaZoom;
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

bool NodeEditor::restoreZoomScaling()
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

float NodeEditor::applyZoomScalingToFont(ImFont* font, ImFont* largeFont)
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

void NodeEditor::restoreZoomScalingToFont(ImFont* font, float originalScale)
{
	font->Scale = originalScale;
	ImGui::PopFont();
}

bool NodeEditor::ensureZoomScaling(bool active)
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

void NodeEditor::addNode(std::shared_ptr<Workspace::CoreNode> node, const ImVec2 position, bool shiftToLeftByNodeWidth)
{
	// Nodes should be created in the diwne zoom scaling environment (so ImGui calls return scaled values like font
	// size, padding etc.)
	// Hence scaling is applied here if not active, and then restored to its original state at the end of this
	// method
	bool zoomScalingWasActive = this->ensureZoomScaling(true);
	node->setNodePositionDiwne(position);
	if (shiftToLeftByNodeWidth)
	{
		node->draw(DIWNE::DrawMode::JustDraw); /* to obtain size */
		node->move(ImVec2(-node->getRectDiwne().GetSize().x - 10, 0));
	}

	LOG_INFO("ADDED NODE {}, {}", node->m_labelDiwne, this->m_workspaceCoreNodes.size());
	// TODO: A subclass node editor might keep its own storage, we could add internal callbacks to add node
	m_workspaceCoreNodes.push_back(node);
	this->ensureZoomScaling(zoomScalingWasActive); // Restore zoom scaling to original state
}

} /* namespace DIWNE */
