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

#include "diwne_actions.h"

// TODO: REMOVE <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
#include "GUI/Workspace/Nodes/Basic/CoreNode.h"

namespace DIWNE
{
NodeEditor::NodeEditor(SettingsDiwne* settingsDiwne)
    : DiwneObject(*this, settingsDiwne->editorlabel), mp_settingsDiwne(settingsDiwne),
      m_popupPosition(settingsDiwne->initPopupPosition)
{
	setSelectable(false);
}

NodeEditor::~NodeEditor()
{
	clear();
}

void NodeEditor::draw(DrawMode drawMode)
{
	interactionState.nextFrame();
	DrawInfo context(interactionState);
	drawDiwne(context, drawMode);
}

void NodeEditor::initializeDiwne(DrawInfo& context)
{
	m_popupDrawn = m_tooltipDrawn = m_objectFocused = m_takeSnap = false;

	destroyObjects(); // Delete objects marked for destruction in the previous frame

	DiwneObject::initializeDiwne(context);
}

void NodeEditor::begin(DrawInfo& context)
{
	ImGui::BeginChild(this->m_labelDiwne.c_str(), ImVec2(0, 0), false,
	                  ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoMove);

	m_canvas->updateViewportRects();
	m_nodesSelectionChanged = false;

	ImGui::SetCursorScreenPos(m_canvas->m_viewRectScreen.Min);
	ImGui::PushID(m_labelDiwne.c_str()); // TODO: Is this necessary? We already do this in the Begin child no?
	ImGui::BeginGroup();

	assert(!m_canvas->m_zoomScalingApplied);
	m_canvas->applyZoomScaling();

	DIWNE_DEBUG_OBJECTS((*this), {
		ImGui::GetWindowDrawList()->AddRect(m_canvas->m_viewRectScreen.Min, m_canvas->m_viewRectScreen.Max,
		                                    ImColor(255, 0, 0), 0, ImDrawFlags_RoundCornersNone, 10);
		ImGui::Text(fmt::format("\tWADiwne: {}-{}  -  {}-{}\n\tWAScreen: {}-{}  -  {}-{}",
		                        m_canvas->m_viewRectDiwne.Min.x, m_canvas->m_viewRectDiwne.Min.y,
		                        m_canvas->m_viewRectDiwne.Max.x, m_canvas->m_viewRectDiwne.Max.y,
		                        m_canvas->m_viewRectScreen.Min.x, m_canvas->m_viewRectScreen.Min.y,
		                        m_canvas->m_viewRectScreen.Max.x, m_canvas->m_viewRectScreen.Max.y)
		                .c_str());

		ImGui::Text(fmt::format("MousePos: {}-{}", ImGui::GetIO().MousePos.x, ImGui::GetIO().MousePos.y).c_str());
		ImGui::Text(fmt::format("PopupPos: {}-{}", getPopupPosition().x, getPopupPosition().y).c_str());
		ImGui::Text(fmt::format("Zoom: {}", m_canvas->m_zoom).c_str());

		ImGui::Text(
		    fmt::format("WindowPadding: {}_{} ", ImGui::GetStyle().WindowPadding.x, ImGui::GetStyle().WindowPadding.y)
		        .c_str());
		ImGui::Text(fmt::format("WindowClipRect: {} _ {} _ {} _ {} ", ImGui::GetCurrentWindow()->ClipRect.Min.x,
		                        ImGui::GetCurrentWindow()->ClipRect.Min.y, ImGui::GetCurrentWindow()->ClipRect.Max.x,
		                        ImGui::GetCurrentWindow()->ClipRect.Max.y)
		                .c_str());
		ImGui::TextUnformatted(fmt::format("Nodes: {}", m_workspaceCoreNodes.size()).c_str());
		ImGui::TextUnformatted(fmt::format("Links: {}", m_links.size()).c_str());
	});
}

void NodeEditor::content(DrawInfo& context)
{
	// Debug work area rect
	ImGui::GetWindowDrawList()->AddRect(m_canvas->m_viewRectScreen.Min, m_canvas->m_viewRectScreen.Max,
	                                    ImColor(255, 0, 0, 255));

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

		m_channelSplitter.Merge(ImGui::GetWindowDrawList());

		DIWNE_DEBUG((*this), {
			float zoom = m_canvas->getZoom();
			ImGui::GetWindowDrawList()->AddCircleFilled(m_canvas->diwne2screen(ImVec2(0, 0)), 10.f * zoom,
			                                            IM_COL32(255, 0, 255, 150));
		});
	}
}

void NodeEditor::end(DrawInfo& context)
{
	m_canvas->stopZoomScaling();

	// TODO: This doesnt work anymore in newer ImGui versions, a dummy needs to be added (I am not sure on that anymore)
	ImGui::SetCursorScreenPos(m_canvas->m_viewRectScreen.Max); /* for capture whole window/workarea to Group */
	ImGui::EndGroup();
	m_internalHover = ImGui::IsItemHovered();
	// TODO: NodeEditor is hovered even when another active item like a DragFloat within a node is dragged.
	//  This is because the actual node editor does not have an InvisibleButton or other interactive item "behind" it,
	//  representing it. The EndGroup call does not change ActiveId and thus it gets hovered even when there is an
	//  active item. This does not happen to nodes, since those add an interactive InvisibleButton after EndGroup in the
	//  afterDraw method. I am not sure what the desired behaviour here is.
	//  I suppose there is no problem in making NodeEditor behave like a Node
	//  (that would be required anyway for the ultimate test of a node editor inside a node editor)
	ImGui::PopID();
}

void NodeEditor::updateLayout(DrawInfo& context)
{
	m_rect = m_canvas->getViewportRectDiwne();
}

void NodeEditor::afterDraw(DrawInfo& context)
{
	shiftInteractingNodeToEnd();
	DIWNE_DEBUG_LAYOUT(diwne, {
		ImVec2 originPos = ImVec2(getRectDiwne().Min.x, getRectDiwne().Min.y);
		ImGui::GetForegroundDrawList()->AddText(
		    m_canvas->diwne2screen(originPos) + ImVec2(getRectDiwne().GetWidth() * 0.2, 0), IM_COL32_WHITE,
		    (std::string() + "zoom: " + std::to_string(m_canvas->getZoom()) + ", " + "workArea: " +
		     std::to_string(m_canvas->m_viewRectDiwne.Min.x) + ", " + std::to_string(m_canvas->m_viewRectDiwne.Min.y))
		        .c_str());
	});
	// The editor child window is ended here instead of end() so that afterDraw() drawing is in the same window DrawList
	ImGui::EndChild();
}

bool NodeEditor::allowHover() const
{
	return true; // TODO: Finish this <<<<<<<<<<<<<<<<<<<<<<<<
	             //	return m_isActive /* object is active from previous frame */
	             //	       ||
	//	       (diwne.getDiwneActionActive() == SelectionRectFull || diwne.getDiwneActionActive() == SelectionRectTouch)
	//||
	//	       (!diwne.m_objectFocused /* only one object can be focused */
	//	        && (diwne.getDiwneAction() == None ||
	//	            diwne.getDiwneActionActive() == NewLink /* we want focus of other object while new link */));
}

//
void NodeEditor::processInteractions(DrawInfo& context)
{
	// Stop any action on ESC
	if (ImGui::IsKeyDown(ImGuiKey_Escape))
	{
		if (interactionState.anyActionActive())
			interactionState.endAction();
	}

	if (!context.inputConsumed)
	{
		// Handle zooming
		if (m_hovered)
			processDiwneZoom();
	}
}

void NodeEditor::onReleased(bool justReleased, DrawInfo& context)
{
	// Unselect nodes on click
	if (justReleased)
	{
		if (!context.logicalUpdates && !context.state.dragging && !m_input->multiSelectionActive())
		{
			deselectNodes();
		}
	}
	DiwneObject::onReleased(justReleased, context);
}

void NodeEditor::onDrag(DrawInfo& context, bool dragStart, bool dragEnd)
{
	DiwneObject::onDrag(context, dragStart, dragEnd);
	if (m_input->bypassIsMouseDown2())
	{
		m_canvas->moveViewportZoomed(m_input->bypassGetMouseDelta() * -1);
	}

	using namespace Actions;

	if (m_input->selectionRectangleKeyDown())
	{
		if (dragStart && !context.state.action)
		{
			context.state.startAction<SelectionRectAction>(m_labelDiwne);
		}
	}
	if (context.state.isActionActive(Actions::selectionRect, m_labelDiwne))
	{
		ImVec2 startPos = m_input->selectionRectangleStartPosition();
		ImVec2 dragDelta = m_input->selectionRectangleSize();
		ImColor color;

		SelectionRectAction* action = context.state.getAction<SelectionRectAction>();
		if (dragDelta.x > 0)
		{
			action->rect.Min.x = startPos.x;
			action->rect.Max.x = startPos.x + dragDelta.x;
			color = mp_settingsDiwne->selectionRectFullColor;
			action->touch = false;
		}
		else
		{
			action->rect.Min.x = startPos.x + dragDelta.x;
			action->rect.Max.x = startPos.x;
			color = mp_settingsDiwne->selectionRectTouchColor;
			action->touch = true;
		}
		if (dragDelta.y > 0)
		{
			action->rect.Min.y = startPos.y;
			action->rect.Max.y = startPos.y + dragDelta.y;
		}
		else
		{
			action->rect.Min.y = startPos.y + dragDelta.y;
			action->rect.Max.y = startPos.y;
		}
		m_canvas->AddRectFilledDiwne(action->rect.Min, action->rect.Max, color);
		m_canvas->AddRectDiwne(
		    action->rect.Min, action->rect.Max,
		    ImVec4(color.Value.x, color.Value.y, color.Value.z, mp_settingsDiwne->selectionRectBorderAlpha));

		if (dragEnd)
		{
			context.state.endAction();
		}
	}
}

bool NodeEditor::isPressedDiwne()
{
	return m_input->selectionRectangleKeyDown() || m_input->panKeyDown();
}

bool NodeEditor::isJustPressedDiwne()
{
	return m_input->selectionRectangleKeyPressed() || m_input->panKeyPressed();
}

bool NodeEditor::isDraggedDiwne()
{
	return m_input->selectionRectangleDragging() || m_input->panDragging();
}

bool NodeEditor::processZoom()
{
	ImVec2 mousePosDiwne = m_canvas->screen2diwne(m_input->bypassGetMousePos());
	m_canvas->setZoom(m_canvas->getZoom() + m_input->getZoomDelta());
	m_canvas->moveViewport(mousePosDiwne - m_canvas->screen2diwne(m_input->bypassGetMousePos()));
	return true;
}

bool NodeEditor::processDiwneZoom()
{
	if (isZoomingDiwne() && allowProcessZoom())
	{
		return processZoom();
	}
	return false;
}

void NodeEditor::shiftNodesToBegin(std::vector<std::shared_ptr<Workspace::CoreNode>> const& nodesToShift)
{
	for (int i = 0; i < nodesToShift.size(); i++)
	{
		auto ith_selected_node =
		    std::find_if(m_workspaceCoreNodes.begin(), m_workspaceCoreNodes.end(),
		                 [nodesToShift, i](std::shared_ptr<Workspace::CoreNode> const& node) -> bool {
			                 return node->getId() == nodesToShift.at(i)->getId();
		                 });

		if (ith_selected_node != m_workspaceCoreNodes.end())
		{
			std::iter_swap(m_workspaceCoreNodes.begin() + i, ith_selected_node);
		}
	}
}

void NodeEditor::shiftNodesToEnd(std::vector<std::shared_ptr<Workspace::CoreNode>> const& nodesToShift)
{
	int node_num = nodesToShift.size();
	//    str2.erase(std::remove_if(str2.begin(),
	//                              str2.end(),
	//                              [](unsigned char x){return std::isspace(x);})
	//    a.erase(std::remove_if(a.begin(), a.end(), predicate), a.end());
	for (int i = 0; i < node_num; i++)
	{
		auto ith_selected_node =
		    std::find_if(m_workspaceCoreNodes.begin(), m_workspaceCoreNodes.end(),
		                 [nodesToShift, i](std::shared_ptr<Workspace::CoreNode> const& node) -> bool {
			                 return node->getId() == nodesToShift.at(i)->getId();
		                 });
		if (ith_selected_node != m_workspaceCoreNodes.end())
		{
			std::iter_swap(m_workspaceCoreNodes.end() - node_num + i, ith_selected_node);
		}
	}
}

void NodeEditor::shiftInteractingNodeToEnd()
{
	if (mp_lastActiveNode == nullptr || m_workspaceCoreNodes.empty())
		return;
	if (mp_lastActiveNode.get() != m_workspaceCoreNodes.back().get())
	{
		auto draged_node_it = std::find_if(m_workspaceCoreNodes.begin(), m_workspaceCoreNodes.end(),
		                                   [this](Ptr<Workspace::CoreNode> const& node) -> bool {
			                                   return node.get() == this->mp_lastActiveNode.get();
		                                   });

		if (draged_node_it != m_workspaceCoreNodes.end() && draged_node_it != m_workspaceCoreNodes.end() - 1)
		{
			std::rotate(draged_node_it, draged_node_it + 1, m_workspaceCoreNodes.end());
		}
	}
}

bool NodeEditor::allowProcessZoom()
{
	return true;
}
bool NodeEditor::isZoomingDiwne()
{
	return diwne.m_input->getZoomDelta() != 0;
}

void NodeEditor::clear()
{
	for (auto& node : m_workspaceCoreNodes)
	{
		node->destroy(false);
	}
	for (auto& link : m_links)
	{
		link->destroy(false);
	}
	m_workspaceCoreNodes.clear();
	m_links.clear();

	setLastActiveNode<DIWNE::Node>(nullptr);
}

std::vector<std::shared_ptr<Workspace::CoreNode>> NodeEditor::getAllNodesInnerIncluded()
{
	return getAllNodes();
}

std::vector<std::shared_ptr<Workspace::CoreNode>> NodeEditor::getSelectedNodesInnerIncluded()
{
	return getSelectedNodes();
}

std::vector<Ptr<Workspace::CoreNode>> NodeEditor::getSelectedNodes()
{
	std::vector<Ptr<Workspace::CoreNode>> selected;
	for (auto const& node : m_workspaceCoreNodes)
	{
		if (node->getSelected())
		{
			selected.push_back(node);
		};
	}
	return selected;
}

void NodeEditor::deselectNodes()
{
	for (auto node : getAllNodesInnerIncluded())
	{
		node->setSelected(false);
	}
}

void NodeEditor::addNode(std::shared_ptr<Workspace::CoreNode> node, const ImVec2 position, bool shiftToLeftByNodeWidth)
{
	// Nodes should be created in the diwne zoom scaling environment (so ImGui calls return scaled values like font
	// size, padding etc.)
	// Hence scaling is applied here if not active, and then restored to its original state at the end of this
	// method
	bool zoomScalingWasActive = m_canvas->ensureZoomScaling(true);
	node->setNodePositionDiwne(position);
	if (shiftToLeftByNodeWidth)
	{
		node->draw(DIWNE::DrawMode::JustDraw); /* to obtain size */
		node->move(ImVec2(-node->getRectDiwne().GetSize().x - 10, 0));
	}

	// TODO: A subclass node editor might keep its own storage, we could add internal callbacks to add node
	m_workspaceCoreNodes.push_back(node);
	node->m_parentLabel = m_labelDiwne;                // Set the editor as the node's parent
	m_canvas->ensureZoomScaling(zoomScalingWasActive); // Restore zoom scaling to original state
}

void NodeEditor::addLink(std::shared_ptr<Link> link)
{
	m_links.push_back(link);
}

void NodeEditor::destroyObjects()
{
	m_workspaceCoreNodes.erase(std::remove_if(m_workspaceCoreNodes.begin(), m_workspaceCoreNodes.end(),
	                                          [](std::shared_ptr<Workspace::CoreNode> const& node) -> bool {
		                                          return node->m_destroy;
	                                          }),
	                           m_workspaceCoreNodes.end());
	m_links.erase(std::remove_if(m_links.begin(), m_links.end(),
	                             [](const std::shared_ptr<Link>& link) -> bool {
		                             return link->m_destroy;
	                             }),
	              m_links.end());
}

ImVec2 const& NodeEditor::getPopupPosition() const
{
	return m_popupPosition;
};
void NodeEditor::setPopupPosition(ImVec2 position)
{
	m_popupPosition = position;
};

void NodeEditor::setNodesSelectionChanged(bool value)
{
	m_nodesSelectionChanged = value;
};
bool NodeEditor::getNodesSelectionChanged()
{
	return m_nodesSelectionChanged;
};

} /* namespace DIWNE */
