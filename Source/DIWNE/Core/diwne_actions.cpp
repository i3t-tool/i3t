/**
 * \file
 * \brief
 * \author Dan Rakušan <rakusan.dan@gmail.com>
 * \copyright Copyright (C) 2016-2025 I3T team, Department of Computer Graphics
 * and Interaction, FEE, Czech Technical University in Prague, Czech Republic
 *
 * This file is part of I3T - An Interactive Tool for Teaching Transformations
 * http://www.i3t-tool.org
 *
 * GNU General Public License v3.0 (see LICENSE.txt or https://www.gnu.org/licenses/gpl-3.0.txt)
 */
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
	// This must be called *right* before rendering to achieve the lowest input delay.
	// Eg. this expects that onFrameEnd() is actually called onNextFrameBegin() //TODO: Maybe rename this method.
	ImVec2 offset = editor.canvas().screen2diwneSize(editor.input().bypassGetMouseDelta());
	for (auto& node : nodes)
	{
		if (!node->isFixed()) // Do not move with "fixed" nodes (would cause visual artefacts)
			node->translate(offset);
	}
}

}; // namespace Actions
} // namespace DIWNE