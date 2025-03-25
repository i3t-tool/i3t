#include "diwne_actions.h"

#include "NodeEditor.h"

#include "Elements/Link.h"
#include "Elements/Node.h"
#include "Elements/Pin.h"

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

void DragNodeAction::onFrameEnd()
{
	ImVec2 offset = editor.canvas().screen2diwneSize(editor.input().bypassGetMouseDelta());
	for (auto node : nodes)
	{
		if (!node->isFixed()) // Do not move with "fixed" nodes (would cause visual artefacts)
			node->translate(offset);
	}
}

}; // namespace Actions
} // namespace DIWNE