#pragma once

#include "diwne_actions.h"

#include "NodeEditor.h"

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
	ImVec2 offset = editor.input().bypassGetMouseDelta() / editor.getZoom();
	for (auto node : nodes)
	{
		if (!node->isFixed()) // Do not move with "fixed" nodes (would cause visual artefacts)
			node->translate(offset);
	}
}

}; // namespace Actions
} // namespace DIWNE