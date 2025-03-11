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

#include "Logger/Logger.h"

#include "DIWNE/Core/NodeEditor.h"
#include "DIWNE/Core/diwne_actions.h"
#include "DIWNE/Utils/diwne_utils.h"
#include "Link.h"
#include "Node.h"

namespace DIWNE
{

Pin::Pin(DIWNE::NodeEditor& diwne, Node* node, bool isInput, std::string labelDiwne)
    : DiwneObject(diwne, labelDiwne), m_node(node), m_isInput(isInput)
{
	setSelectable(false);
	if (m_isInput)
		m_allowMultipleConnections = false;
}


void Pin::begin(DrawInfo& context)
{
	m_connectionChanged = false;

	ImGui::PushID(m_labelDiwne.c_str());
	ImGui::BeginGroup();
}

void Pin::end(DrawInfo& context)
{
	ImGui::EndGroup();
	ImGui::PopID();
	m_internalHover = ImGui::IsItemHovered();
}

void Pin::updateLayout(DrawInfo& context)
{
	m_rect.Min = diwne.canvas().screen2diwne(ImGui::GetItemRectMin());
	m_rect.Max = diwne.canvas().screen2diwne(ImGui::GetItemRectMax());

	updateConnectionPoint();
}

bool Pin::isInput() const
{
	return m_isInput;
}

void Pin::processInteractions(DrawInfo& context)
{
	DiwneObject::processInteractions(context);

	// Check if another pin was dragged over this one
	if (m_hovered && context.state.dragging && allowConnection())
	{
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
				DIWNE_FAIL("[DIWNE] Dragged link both or no ends connected!")
			}
			preparePlug(otherPin, action->draggedLink, !context.state.dragEnd);
		}
	}
}

void Pin::onDrag(DrawInfo& context, bool dragStart, bool dragEnd)
{
	DiwneObject::onDrag(context, dragStart, dragEnd);
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
		if (action->draggedLink->getEndPin() == nullptr)
			action->draggedLink->setEndPoint(diwne.canvas().screen2diwne(diwne.input().bypassGetMousePos()));
		else
			action->draggedLink->setStartPoint(diwne.canvas().screen2diwne(diwne.input().bypassGetMousePos()));

		if (dragEnd)
		{
			action->end();
		}
	}
}

// TODO: (DR) Not tested standalone
bool Pin::preparePlug(Pin* otherPin, Link* link, bool hovering)
{
	if (!hovering)
		return plugLink(otherPin, link);
	return false;
}

// TODO: (DR) Not tested standalone
bool Pin::plug(Pin* otherPin, bool logEvent)
{
	auto link = createLink();
	return plugLink(otherPin, link.get(), logEvent);
}

bool Pin::plugLink(Pin* otherPin, Link* link, bool logEvent)
{
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

bool Pin::allowConnection() const
{
	return true;
}

bool Pin::allowPopup() const
{
	return false;
}
bool Pin::allowDragStart() const
{
	return DiwneObject::allowDragStart() && allowConnection();
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

void Pin::setConnectionPointDiwne(const ImVec2 value)
{
	m_connectionPoint = value;
}
const ImVec2& Pin::getConnectionPoint()
{
	return m_connectionPoint;
}
void Pin::updateConnectionPoint()
{
	m_connectionPoint = m_pinRect.GetCenter();
}
void Pin::initialize(DrawInfo& context)
{
	DiwneObject::initialize(context);
	if (!m_allowMultipleConnections && m_links.size() > 1)
	{
		DIWNE_WARN(std::string() + "Pin " + m_labelDiwne +
		           " has multiple links but multiple connections are not allowed!");
	}
}

void Pin::onDestroy(bool logEvent)
{
	for (auto link : m_links)
		link->destroy(logEvent);
	DiwneObject::onDestroy(logEvent);
}

} /* namespace DIWNE */
