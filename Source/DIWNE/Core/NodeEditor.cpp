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

#include "Elements/Link.h"
#include "Elements/Node.h"
#include "Elements/Pin.h"

#include "diwne_actions.h"

namespace DIWNE
{
NodeEditor::NodeEditor(SettingsDiwne* settingsDiwne)
    : DiwneObject(*this, settingsDiwne->editorlabel), mp_settingsDiwne(settingsDiwne), NodeContainer(this),
      m_popupPosition(settingsDiwne->initPopupPosition)
{
	setSelectable(false);
}

NodeEditor::~NodeEditor()
{
	// Generally DiwneObjects can have an empty destructor as they should be explictly destroyed beforehand
	// using the destroy() method. NodeEditor is special case which destroys itself automatically.
	// However this destroy() call below will not propagate to any NodeEditor subclasses which need their own destructor
	// implementation. This is because of the order in which destructors are called by C++.
	NodeEditor::destroy(true);
}

void NodeEditor::draw(DrawMode drawMode)
{
	interactionState.nextFrame();
	DrawInfo context(interactionState);
	drawDiwne(context, drawMode);
}

void NodeEditor::initializeDiwne(DrawInfo& context)
{
	// clang-format off
	assert("DIWNE::NodeEditor cannot be created on the stack, using the new operator or as a unique_ptr. It MUST be managed by a shared pointer! Use std::make_shared or std::shared_ptr to define it." &&
	       !this->weak_from_this().expired());
	// clang-format on

	m_popupDrawn = m_tooltipDrawn = m_objectFocused = m_takeSnap = false; // TODO: (DR) Remove some of these

	// TODO: Unify bringToFront/shifting behavior across pasting, dragging and last active node
	//   - Last active node should use the bringToFront marking system same as any others
	//   - Pasting should use marking as well, does pasting even need it? shouldn't it append to the end of the list?
	//   - Dragged nodes use the setBringToFront() marking

	if (m_lastActiveNodeChanged)
	{
		bringLastActiveNodeToFront();
		m_lastActiveNodeChanged = false;
	}
	// TODO: (Dr) This requires an extra O(n) pass, could be merged with object purging
	bringMarkedNodesToFront();

	purgeObjects(); // Erase objects marked for destruction or removal in the previous frame

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
		ImGui::TextUnformatted(fmt::format("Nodes: {}", m_nodes.size()).c_str());
		ImGui::TextUnformatted(fmt::format("Links: {}", m_links.size()).c_str());
	});
}

void NodeEditor::content(DrawInfo& context)
{
	// Debug work area rect
	ImGui::GetWindowDrawList()->AddRect(m_canvas->m_viewRectScreen.Min, m_canvas->m_viewRectScreen.Max,
	                                    ImColor(255, 0, 0, 255));

	// TODO: [Performance] Could the channel splitter with its channel for each node be causing performance issues?
	//  I haven't investigated too deep but presumably all draw commands (which are many) are being reordered in
	//  an array every frame.

	// NOTE: Nodes are "constructed" front to back, eg. the first UI code to run is from the TOP node, however the nodes
	//  are then later drawn by ImGui back to front because we reorder the ImGui draw commands using a channel splitter.
	// This is desirable as when ImGui elements overlap, the first element to receive input is the FIRST one that's
	//  submitted to ImGui, as it is able to "grab" the input first, the later elements which are drawn on top of it
	//  are not the ones to receive input even though they're "FIRST" in the sense they're on top and drawn last.
	// This is rather counterintuitive and a direct result of the immediate mode methodology. There is a way around
	//  this in ImGui in the form of ImGui::SetNextItemAllowOverlap() but it is a tricky multi-frame workaround.

	int number_of_nodes = m_nodes.size();
	int node_count = number_of_nodes; /* -1 for space for top node drawn above links */
	if (number_of_nodes > 0)
	{
		// Each node is in its own channel as they all need to be drawn in the reverse order.
		// Then the temporary helper link is drawn atop everything
		// And one channel is for all the links below everything
		m_channelSplitter.Split(ImGui::GetWindowDrawList(), number_of_nodes + 2 /*+1 for links channel on top */);

		bool takeSnap = false;

		// Draw nodes in reverse order (front to back) to process interactions in correct "bubble down" order.
		// Any nodes added during drawing should be added at the end of the vector and thus not iterated over.
		// Any nodes removed should be removed in a lazy manner using either Node::destroy() or remove().
		// Index based iteration is used to avoid iterator invalidation when adding new elements.
		int initialNodeCount = m_nodes.size();
		for (int i = initialNodeCount - 1; i >= 0; --i)
		//		for (auto it = m_nodes.rbegin(); it != m_nodes.rend(); ++it)
		{
			// We must work with the underlying raw pointer to avoid invalidation when nodes are added to the vector
			Node* node = m_nodes[i].get();
			//			std::shared_ptr<Node> node = m_nodes[i];
			//			if (it == m_workspaceCoreNodes.rbegin()) /* node on top */
			//			{
			//				m_channelSplitter.SetCurrentChannel(ImGui::GetWindowDrawList(),
			//				                                    number_of_nodes); /* top node is above links */
			//			}
			//			else
			//			{
			//				m_channelSplitter.SetCurrentChannel(ImGui::GetWindowDrawList(), --node_count);
			//			}
			m_channelSplitter.SetCurrentChannel(ImGui::GetWindowDrawList(), node_count--);

			if (node != nullptr && node->isRendered())
			{
				// Set the ImGui cursor position to the position of the node and draw it
				ImGui::SetCursorScreenPos(canvas().diwne2screen(node->getPosition()));
				DrawInfo drawResult = node->drawDiwneEx(context, m_drawMode);

				// If the node requests focus, bring it to front (unless it's going to be destroyed or removed)
				if (drawResult.logicalUpdates && !node->isDestroyed() && !node->willBeRemovedFromContainer(this))
				{
					setLastActiveNode(std::static_pointer_cast<Node>(node->shared_from_this()));
				}
			}
		}

		auto connectPinAction = context.state.getActiveAction<DIWNE::Actions::ConnectPinAction>();

		m_channelSplitter.SetCurrentChannel(ImGui::GetWindowDrawList(), 0);
		// Draw links, first in drawing order, last in logical order
		for (auto link : m_links)
		{
			// Avoid drawing the dragged link if a new connection is being made
			std::string draggedLinkLabel = "";
			if (connectPinAction)
			{
				draggedLinkLabel = connectPinAction->draggedLink->m_labelDiwne;
			}
			if (link->isRendered() && draggedLinkLabel != link->m_labelDiwne)
				link->drawDiwne(context);
		}

		m_channelSplitter.SetCurrentChannel(ImGui::GetWindowDrawList(), number_of_nodes + 1);

		// Draw the dragged link on top if a new connection is made
		if (connectPinAction)
		{
			connectPinAction->draggedLink->drawDiwne(context, DIWNE::DrawMode_JustDraw);
		}
		m_channelSplitter.Merge(ImGui::GetWindowDrawList());
	}

	DIWNE_DEBUG((*this), {
		float zoom = m_canvas->getZoom();
		ImGui::GetWindowDrawList()->AddCircleFilled(m_canvas->diwne2screen(ImVec2(0, 0)), 10.f * zoom,
		                                            IM_COL32(255, 0, 255, 150));
	});

	// TODO: Figure out if we need to take a snap here
	// m_takeSnap |= interaction_happen;
	/// \todo see #111, wrongly computed m_takeSnap value.
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
	DIWNE_DEBUG_LAYOUT(diwne, {
		ImVec2 originPos = ImVec2(getRect().Min.x, getRect().Min.y);
		ImGui::GetForegroundDrawList()->AddText(
		    m_canvas->diwne2screen(originPos) + ImVec2(getRect().GetWidth() * 0.2, 0), IM_COL32_WHITE,
		    (std::string() + "zoom: " + std::to_string(m_canvas->getZoom()) + ", " + "workArea: " +
		     std::to_string(m_canvas->m_viewRectDiwne.Min.x) + ", " + std::to_string(m_canvas->m_viewRectDiwne.Min.y))
		        .c_str());
	});
	// The editor child window is ended here instead of end() so that afterDraw() drawing is in the same window DrawList
	ImGui::EndChild();
}

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

		if (!context.logicalUpdates)
		{
			if (input().selectAllNodes())
				this->selectAllNodes(true);
			if (input().invertSelection())
				this->invertSelection();
			if (input().deleteSelectedNodes())
				this->deleteSelectedNodes();
		}
	}
}

void NodeEditor::onReleased(bool justReleased, DrawInfo& context)
{
	// Unselect nodes on click
	if (justReleased)
	{
		if (!context.logicalUpdates && !context.state.dragging && !m_input->multiSelectionActive())
		{
			deselectAllNodes();
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
			context.state.startAction<SelectionRectAction>(shared_from_this());
		}
	}
	if (context.state.isActionActive(Actions::selectionRect, this))
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
			action->end();
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

	//	editor.mp_settingsDiwne->zoomWheelReverseSenzitivity;

	//	m_canvas->setZoom(m_canvas->getZoom() + m_input->getZoomDelta());
	m_canvas->setZoom(m_canvas->getZoom() * powf(1.0f + mp_settingsDiwne->zoomSensitivity, m_input->getZoomDelta()));
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

void NodeEditor::shiftNodesToBegin(const NodeList& nodesToShift)
{
	// TODO: (DR) Performance concerns see shiftNodesToEnd()
	for (int i = 0; i < nodesToShift.size(); i++)
	{
		auto ith_selected_node =
		    std::find_if(m_nodes.begin(), m_nodes.end(), [nodesToShift, i](const auto& node) -> bool {
			    return node->getId() == nodesToShift.at(i)->getId();
		    });

		if (ith_selected_node != m_nodes.end())
		{
			std::iter_swap(m_nodes.begin() + i, ith_selected_node);
		}
	}
}

void NodeEditor::shiftNodesToEnd(const NodeList& nodesToShift)
{
	// TODO: (Dr) This is O(n * m), m being size of nodesToShift
	//  Worst case we're doing n^2 unnecessarily (dragging all nodes?)
	//  Also it is NOT stable (order of other nodes may get modified)
	//  Better to use partition + rotate but that wouldn't preserve order either
	//  Thus stable partition could be used which is potentially O(n) + memory overhead (or O(n log n))
	//  --> Best to decide based on some actual performance metrics

	int node_num = nodesToShift.size();
	for (int i = 0; i < node_num; i++)
	{
		auto ith_selected_node =
		    std::find_if(m_nodes.begin(), m_nodes.end(), [nodesToShift, i](const auto& node) -> bool {
			    return node->getId() == nodesToShift.at(i)->getId();
		    });
		if (ith_selected_node != m_nodes.end())
		{
			std::iter_swap(m_nodes.end() - node_num + i, ith_selected_node);
		}
	}
}

void NodeEditor::bringLastActiveNodeToFront()
{
	if (mp_lastActiveNode.expired() || m_nodes.empty())
		return;
	std::shared_ptr<Node> lastActiveNode = mp_lastActiveNode.lock();
	if (lastActiveNode->isChildObject()) // The last interacted node is in a child object and handled there
		return;
	if (lastActiveNode.get() != m_nodes.back().get())
	{
		auto draged_node_it = std::find_if(m_nodes.begin(), m_nodes.end(), [&lastActiveNode](const auto& node) -> bool {
			return node == lastActiveNode;
		});

		if (draged_node_it != m_nodes.end() && draged_node_it != m_nodes.end() - 1)
		{
			std::rotate(draged_node_it, draged_node_it + 1, m_nodes.end());
		}
	}
}

void NodeEditor::bringMarkedNodesToFront()
{
	// TODO: (DR) Optimize
	NodeList shift;
	for (auto& node : this->getNodeList())
	{
		if (node->isToBeBroughtToFront())
		{
			shift.push_back(node);
			node->setBringToFront(false);
		}
	}
	if (!shift.empty())
		shiftNodesToEnd(shift);
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
	for (auto& node : m_nodes)
	{
		node->destroy(false);
	}
	for (auto& link : m_links)
	{
		link->destroy(false);
	}
	m_nodes.clear();
	m_links.clear();

	setLastActiveNode(nullptr);
}

NodeList& NodeEditor::getNodeList()
{
	return NodeContainer::getNodeList();
}
NodeRange<> NodeEditor::getNodes() const
{
	return NodeContainer::getNodes();
};
RecursiveNodeRange<> NodeEditor::getAllNodesInnerIncluded() const
{
	return RecursiveNodeRange<>(&m_nodes);
}
FilteredNodeRange<> NodeEditor::getSelectedNodes() const
{
	return FilteredNodeRange<>(
	    [](const Node* node) -> bool {
		    return node->getSelected();
	    },
	    &m_nodes);
}
FilteredRecursiveNodeRange<> NodeEditor::getSelectedNodesInnerIncluded() const
{
	return FilteredRecursiveNodeRange<>(
	    [](const Node* node) -> bool {
		    return node->getSelected();
	    },
	    &m_nodes);
}

void NodeEditor::deselectAllNodes()
{
	for (auto& node : getAllNodesInnerIncluded())
	{
		node.setSelected(false);
	}
}

void NodeEditor::addNode(const std::shared_ptr<Node>& node)
{
	NodeContainer::addNode(node);
}

void NodeEditor::addNode(const std::shared_ptr<Node>& node, const ImVec2 position, bool shiftToLeftByNodeWidth)
{
	// Nodes should be created in the diwne zoom scaling environment (so ImGui calls return scaled values like font
	// size, padding etc.)
	// Hence scaling is applied here if not active, and then restored to its original state at the end of this
	// method
	bool zoomScalingWasActive = m_canvas->ensureZoomScaling(true);
	node->setPosition(position);
	if (shiftToLeftByNodeWidth)
	{
		// We draw the node out of context to obtain its initial size
		node->draw(DIWNE::DrawMode_JustDraw | DrawMode_ForceDraw);
		node->translate(ImVec2(-node->getRect().GetSize().x - ImGui::GetFontSize() * 2, 0));
	}
	m_canvas->ensureZoomScaling(zoomScalingWasActive); // Restore zoom scaling to original state
	m_takeSnap = true;
	NodeContainer::addNode(node);
}

void NodeEditor::addLink(std::shared_ptr<Link> link)
{
	m_links.push_back(link);
}

void NodeEditor::selectAllNodes(bool deselectIfAllAreSelected)
{
	bool allSelected = true;
	for (auto& node : getAllNodesInnerIncluded())
	{
		if (!node.getSelected() && node.getSelectable())
		{
			allSelected = false;
			node.setSelected(true);
		}
	}
	if (allSelected && deselectIfAllAreSelected)
		deselectAllNodes();
}

void NodeEditor::invertSelection()
{
	for (auto& node : getAllNodesInnerIncluded())
	{
		bool selected = !node.getSelected();
		if (node.getSelected() != selected)
		{
			node.setSelected(selected);
		}
	}
}

void NodeEditor::deleteSelectedNodes()
{
	for (Node& node : getSelectedNodesInnerIncluded())
	{
		if (node.m_deletable)
			node.destroy();
	}
}

void NodeEditor::purgeObjects()
{
	purgeNodes();

	auto endIt = std::remove_if(m_links.begin(), m_links.end(), [this](const auto& e) -> bool {
		return e->m_destroy;
	});
	m_links.erase(endIt, m_links.end());
}

void NodeEditor::purgeAllNodes()
{
	for (auto& node : getAllNodesInnerIncluded())
	{
		// TODO: Avoid dynamic cast using node flags
		if (auto container = dynamic_cast<NodeContainer*>(&node))
		{
			container->purgeNodes();
		}
	}
	purgeNodes();
}

const ImVec2& NodeEditor::getPopupPosition() const
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
}
void NodeEditor::onDestroy(bool logEvent)
{
	clear();
	DiwneObject::onDestroy(logEvent);
}

} /* namespace DIWNE */
