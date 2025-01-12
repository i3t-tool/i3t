#pragma once

#include "diwne_actions.h"

#include "Pin.h"
#include "Link.h"

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

}; // namespace Actions
} // namespace DIWNE