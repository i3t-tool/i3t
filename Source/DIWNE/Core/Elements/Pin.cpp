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
#include "Pin.h"

#include "DIWNE/Core/NodeEditor.h"
#include "DIWNE/Core/diwne_actions.h"
#include "Link.h"
#include "Node.h"

namespace DIWNE
{

Pin::Pin(NodeEditor& diwne, Node* node, bool isInput, std::string labelDiwne)
    : DiwneObject(diwne, labelDiwne), m_node(node), m_isInput(isInput)
{
	assert(node != nullptr);
	setSelectable(false);
	setParentObject(node);
	m_isLeft = m_isInput;
	if (m_isInput)
		Pin::makeInput();
	else
		Pin::makeOutput();
}

void Pin::initialize(DrawInfo& context)
{
	Super::initialize(context);
	if (!m_allowMultipleConnections && m_links.size() > 1)
	{
		DIWNE_WARN(std::string() + "Pin " + m_labelDiwne +
		           " has multiple links but multiple connections are not allowed!");
	}
}

void Pin::begin(DrawInfo& context)
{
	ImGui::PushID(m_labelDiwne.c_str());
	DGui::BeginGroup();

	// Drawing pin hover background, uses hover information from the last frame
	if (m_hovered && allowConnection())
	{
		m_previewPluggedInternal = true;
		if (style().boolean(Style::PIN_ENABLE_HOVER_BG))
			drawPinBackground();
	}
	else
	{
		m_previewPluggedInternal = false;
	}
}
void Pin::content(DrawInfo& context)
{
	m_pinRect = m_rect;
	m_dragRect = m_rect;
	// TODO: Implement basic pin from CorePin
	assert(false && "Not implemented!");
}

void Pin::end(DrawInfo& context)
{
	ImGui::EndGroup();
	ImGui::PopID();
	m_internalHover = ImGui::IsItemHovered();
	m_internalHover |= m_pinIconData.hovered; // Allow pins sticking outside the node to be hovered

	m_previewPlugged = m_previewPluggedInternal;
}

void Pin::updateLayout(DrawInfo& context)
{
	ImRect itemRect = ImGui::GetCurrentContext()->LastItemData.Rect;
	// If this pin is sticking out of a node, we pretend it's imgui item is smaller, special handling is needed
	// to keep DIWNE rects contain the whole pin.
	if (m_pinIconData.protrusion != 0.f)
	{
		if (m_pinIconData.protrusion > 0) // Adjust drag rect if the pin is stickin out of the node
			itemRect.Max.x += m_pinIconData.protrusion;
		else if (m_pinIconData.protrusion < 0)
			itemRect.Min.x += m_pinIconData.protrusion;
		m_rect = diwne.canvas().screen2diwne(itemRect);
	}
	else
	{
		setSize(diwne.canvas().screen2diwneSize(itemRect.GetSize()));
	}
	updateConnectionPoint();
}

void Pin::onDestroy(bool logEvent)
{
	for (auto& link : m_links)
		link->destroy(logEvent);
	Super::onDestroy(logEvent);
}

void Pin::drawPinBackground()
{
	// The background is drawn based on the m_dragRect, eg. the area that will create a new connection when dragged.
	// But notably, it is expanded by ItemSpacing, to create spacing without actually needing to submit it in the pin
	// This only works if the content around is aware of this, so it might be necessary to modify this based on how
	// pins are laid out exactly.
	// Input pins are assumed to be on the left side of the node, flush with the node edge, hence either left or right
	// side of the background is not offset by spacing, and the other side is also not rounded.
	// All of this can be modifed by changing the relevant style variables or drawing a different background
	// in a subclass.
	bool leftSide = isLeft();
	ImVec2 eOffset = style().size(Style::PIN_BG_SPACING) * diwne.getZoom();
	ImVec2 offsetMin = ImVec2(leftSide ? 0 : -eOffset.x, -eOffset.y);
	ImVec2 offsetMax = ImVec2(leftSide ? eOffset.x : 0, eOffset.y);
	ImDrawFlags cornerFlags = leftSide ? ImDrawFlags_RoundCornersRight : ImDrawFlags_RoundCornersLeft;
	if (m_pinIconData.protrusion != 0.f)
	{
		cornerFlags = ImDrawFlags_RoundCornersAll;
		offsetMin = ImVec2(-eOffset.x, -eOffset.y);
		offsetMax = ImVec2(eOffset.x, eOffset.y);
	}
	ImRect displayRectScreen = diwne.canvas().diwne2screenTrunc(m_dragRect);
	if (style().boolean(Style::PIN_ENABLE_DRAG_LABEL))
	{
		ImVec2 pinSpacing = style().size(Style::PIN_SPACING) * diwne.getZoom();
		displayRectScreen.Max.y -= pinSpacing.y; // Reduce drag rect by the pin vertical spacing so it stays centered
	}
	ImGui::GetWindowDrawList()->AddRectFilled(displayRectScreen.Min + offsetMin, displayRectScreen.Max + offsetMax,
	                                          ImGui::ColorConvertFloat4ToU32(style().color(Style::PIN_BG_COLOR)),
	                                          style().decimal(Style::PIN_BG_ROUNDING) * diwne.getZoom(), cornerFlags);
}

bool Pin::isInput() const
{
	return m_isInput;
}

bool Pin::isLeft() const
{
	return m_isLeft;
}

void Pin::processInteractions(DrawInfo& context)
{
	Super::processInteractions(context);

	bool connectionChangedLastFrame = m_connectionChanged;
	m_connectionChanged = false;

	// Check if a link was dragged over this one this frame and released
	// To allow connecting pins of the same Node, we must also check if the dragged pin was released the previous frame.
	bool draggingThisOrPrevFrame = (context.state.dragging || context.state.dragEndedLastFrame);
	if (m_hovered)
	{
		if (draggingThisOrPrevFrame && allowConnection() && !connectionChangedLastFrame)
		{
			// The connect pin action is active during the frame the drag has ended,
			// but also the one frame right after that, so we can still react to it's end with a one frame delay.
			Actions::ConnectPinAction* action = context.state.getActiveAction<Actions::ConnectPinAction>();
			if (action)
			{
				if (!action->sourcePin)
				{
					DIWNE_FAIL("[DIWNE] Source pin was not defined for the drag link action!");
					return;
				}
				if (!action->draggedLink)
				{
					DIWNE_FAIL("[DIWNE] Drag link action has no link set!");
					return;
				}
				Pin* otherPin = action->draggedLink->getSinglePin();
				if (!otherPin)
				{
					DIWNE_FAIL("[DIWNE] Dragged link has both or no ends connected!");
				}

				if (otherPin == this) // Link cannot be plugged into a single pin on both sides
					return;

				// The link is released when the drag ends this frame
				// It is possible that this pin has missed the initial drag end event and so we also check whether
				// the drag has ended the previous frame, but we MUST avoid plugging the link twice, hence on the next
				// frame we must ensure the link wasn't plugged in already by inspecting our m_connectionChanged flag.
				bool linkReleased = context.state.dragEnd || context.state.dragEndedLastFrame;
				action->validConnection = preparePlug(otherPin, action->draggedLink, !linkReleased);
				action->connectionPoint = getConnectionPoint();
			}
		}
		else
		{
			if (isDisabled() && isDragAreaHovered())
				diwne.showTooltipLabel("Pin is disabled!", ImGui::GetStyleColorVec4(ImGuiCol_TextDisabled));
		}
	}
}

void Pin::onDrag(DrawInfo& context, bool dragStart, bool dragEnd)
{
	Super::onDrag(context, dragStart, dragEnd);
	if (dragStart)
	{
		Actions::ConnectPinAction* action = context.state.startAction<Actions::ConnectPinAction>(shared_from_this());
		if (action)
		{
			action->sourcePin = this;
			if (isInput() && !m_allowMultipleConnections && isPlugged())
			{
				// Link of inputs that don't allow multiple connections can be reconnected elsewhere
				Link* link = getLink();
				link->disconnectPin(this, true);
				action->draggedLink = link;
			}
			else
			{
				// Create a new link and set it to be the dragged link in the node editor
				Pin* start = this->isInput() ? nullptr : this;
				Pin* end = this->isInput() ? this : nullptr;
				action->draggedLink = createLink().get();
				action->draggedLink->setStartPin(start);
				action->draggedLink->setEndPin(end);
			}
		}
	}
	if (context.state.isActionActive(Actions::connectPin, this))
	{
		Actions::ConnectPinAction* action = context.state.getAction<Actions::ConnectPinAction>();

		// Update temporary link start/end point
		// Snap the dragged link to this pin if the connection is valid
		if (action->draggedLink->getEndPin() == nullptr)
		{
			if (action->validConnection)
				action->draggedLink->setEndPoint(action->connectionPoint);
			else
				action->draggedLink->setEndPoint(diwne.canvas().screen2diwne(diwne.input().bypassGetMousePos()));
			action->draggedLink->getStartPin()->m_previewPlugged = true;
		}
		else
		{
			if (action->validConnection)
				action->draggedLink->setStartPoint(action->connectionPoint);
			else
				action->draggedLink->setStartPoint(diwne.canvas().screen2diwne(diwne.input().bypassGetMousePos()));
			action->draggedLink->getEndPin()->m_previewPlugged = true;
		}
		action->draggedLink->m_previewPlugged = action->validConnection;
		action->validConnection = false; /// Reset valid flag

		// Allow editor panning
		diwne.processPan();

		// Force drawing of the pin's node to ensure this pin is being drawn while dragging
		assert(m_node && "Pin doesn't have a node assigned!");
		m_node->m_forceDraw = true;

		if (dragEnd)
		{
			action->end();
		}
	}
}

// TODO: (DR) Not tested standalone
bool Pin::preparePlug(Pin* otherPin, Link* link, bool hovering)
{
	assert(this != otherPin && "Pin cannot be plugged into itself");
	if (!hovering)
		return plugLink(otherPin, link);
	return true;
}

// TODO: (DR) Not tested standalone
bool Pin::plug(Pin* otherPin, bool logEvent)
{
	auto link = createLink();
	return plugLink(otherPin, link.get(), logEvent);
}

bool Pin::plugLink(Pin* otherPin, Link* link, bool logEvent)
{
	assert(this != otherPin && "Pin cannot be plugged into itself");
	assert(!isInput() || !otherPin->isInput() && "Both pins cannot be input pins!");
	if (!m_allowMultipleConnections)
	{
		// Disconnect existing link(s)
		unplug(logEvent);
	}

	Pin* startPin = otherPin;
	Pin* endPin = this;
	if (otherPin->isInput())
		std::swap(startPin, endPin); // Other pin is input, this pin is output

	// Connecting using an existing link
	link->connect(startPin, endPin);
	return true;
}

bool Pin::unplug(bool logEvent, bool deleteLinks)
{
	if (m_links.empty())
		return false;

	for (auto& link : m_links)
	{
		// TODO: Figure out the return type, maybe destroy() can return a bool
		if (deleteLinks)
			link->destroy(logEvent);
		else
			link->disconnect(logEvent);
	}
	return true;
}

void Pin::onPlug(Pin* otherPin, Link* link, bool isStartPin, bool logEvent)
{
	if (logEvent)
		diwne.m_takeSnap = true;
	m_connectionChanged = true;
}
void Pin::onUnplug(Pin* otherPin, Link* link, bool wasStartPin, bool logEvent)
{
	if (logEvent)
		diwne.m_takeSnap = true;
	m_connectionChanged = true;
}

std::shared_ptr<Link> Pin::createLink()
{
	return diwne.createLink<Link>();
}

bool Pin::isPlugged() const
{
	return !m_links.empty();
}

bool Pin::connectionChanged() const
{
	return m_connectionChanged;
}
const ImRect& Pin::getPinRect() const
{
	return m_pinRect;
}
const ImRect& Pin::getDragRect() const
{
	return m_dragRect;
}

bool Pin::allowConnection() const
{
	return !isDisabled();
}

bool Pin::allowPopup() const
{
	return false;
}
bool Pin::allowDragStart() const
{
	return Super::allowDragStart() && allowConnection();
}

Node* Pin::getNode()
{
	return m_node;
}
Link* Pin::getLink(size_t index)
{
	assert(index >= 0);
	if (index >= m_links.size())
		return nullptr;
	return m_links[index];
}

bool Pin::registerLink(Link* link)
{
	assert(link != nullptr);
	auto it = std::find(m_links.begin(), m_links.end(), link);
	if (it != m_links.end())
	{
		// Link already registered
		return false;
	}
	m_links.push_back(link);
	return true;
}

bool Pin::unregisterLink(Link* link)
{
	assert(link != nullptr);
	auto it = std::find(m_links.begin(), m_links.end(), link);
	if (it != m_links.end())
	{
		m_links.erase(it);
		return true;
	}
	return false;
}

void Pin::makeInput()
{
	m_allowMultipleConnections = false;
	m_isInput = true;
}
void Pin::makeOutput()
{
	m_allowMultipleConnections = true;
	m_isInput = false;
}

void Pin::translate(const ImVec2& vec)
{
	Super::translate(vec);
	m_connectionPoint += vec;
	// We don't translate pin and drag rect as they can be outside of m_rect
	// m_pinRect.Translate(vec);
	// m_dragRect.Translate(vec);
	// updateConnectionPoint();
}

void Pin::setConnectionPointDiwne(const ImVec2& value)
{
	m_connectionPoint = value;
}
const ImVec2& Pin::getConnectionPoint()
{
	return m_connectionPoint;
}
bool Pin::isDisabled() const
{
	return false;
}

void Pin::updateConnectionPoint()
{
	m_connectionPoint = m_pinRect.GetCenter();
}

bool Pin::isDragAreaHovered() const
{
	if (!m_hovered)
		return false;
	const ImVec2 mousePos = diwne.canvas().screen2diwne(diwne.input().bypassGetMousePos());
	if (style().boolean(DIWNE::Style::PIN_ENABLE_DRAG_LABEL))
		return m_dragRect.Contains(mousePos);
	return m_pinRect.Contains(mousePos);
}

bool Pin::canPlug(Pin* other) const
{
	if (this == other)
		return false;
	return true;
}

} /* namespace DIWNE */
