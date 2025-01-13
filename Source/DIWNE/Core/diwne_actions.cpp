#pragma once

#include "diwne_actions.h"

#include "Link.h"
#include "Node.h"
#include "Pin.h"

namespace DIWNE
{
namespace Actions
{

void ConnectPinAction::onEnd()
{
	// When the connect pin action ends, if the dragged link was not plugged in, we destroy it
	if (!draggedLink->isPlugged())
		draggedLink->destroy(false);
}

void DragNodeAction::onUpdate()
{
	ImVec2 offset = editor.m_input->bypassGetMouseDelta() / editor.getWorkAreaZoom();
	for (auto node : nodes)
	{
		node->move(offset);
	}
}

}; // namespace Actions
} // namespace DIWNE