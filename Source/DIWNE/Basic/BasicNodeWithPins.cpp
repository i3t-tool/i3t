/**
 * \file
 * \brief
 * \author Dan Rakušan <rakusan.dan@gmail.com>, Jaroslav Holeček <holecek.jaroslav@email.cz>
 * \copyright Copyright (C) 2016-2023 I3T team, Department of Computer Graphics
 * and Interaction, FEE, Czech Technical University in Prague, Czech Republic
 *
 * This file is part of I3T - An Interactive Tool for Teaching Transformations
 * http://www.i3t-tool.org
 *
 * GNU General Public License v3.0 (see LICENSE.txt or https://www.gnu.org/licenses/gpl-3.0.txt)
 */
#include "BasicNodeWithPins.h"

#include "DIWNE/Core/Elements/Pin.h"
#include "GUI/Toolkit.h"

namespace DIWNE
{
BasicNodeWithPins::BasicNodeWithPins(NodeEditor& diwne, std::string label) : BasicNode(diwne, label)
{
	m_outputPinsVstack.setSpacing(0); // Pins add their own spacing so the drag areas are flush


	std::shared_ptr<Pin> pinIn = std::make_shared<Pin>(diwne, this, true);
	m_leftPins.push_back(pinIn.get());
	m_inputs.push_back(std::move(pinIn));


	std::shared_ptr<Pin> pinOut = std::make_shared<Pin>(diwne, this, false);
	m_rightPins.push_back(pinOut.get());
	m_outputs.push_back(std::move(pinOut));
}

void BasicNodeWithPins::begin(DrawInfo& context)
{
	Super::begin(context);

	// Pin drag assist
	if (context.inputFullyAvailable() && !(m_drawMode & DrawMode_JustDraw))
		processPinDragAssist();
}

bool BasicNodeWithPins::processPinDragAssist()
{
	float dragAssistRadius = diwne.style().decimal(Style::PIN_DRAG_ASSIST_RADIUS);
	if (dragAssistRadius <= 0)
		return false;
	float minInputDist;
	float minOutputDist;
	Pin* closestInput = findPinClosestToTheMouse(m_inputs, minInputDist);
	Pin* closestOutput = findPinClosestToTheMouse(m_outputs, minOutputDist);
	Pin* closestPin = minInputDist < minOutputDist ? closestInput : closestOutput;
	float minDist = std::min(minInputDist, minOutputDist);
	float pinAssistRadius = diwne.style().decimal(Style::PIN_DRAG_ASSIST_RADIUS);
	if (minDist > pinAssistRadius)
		return false;
	closestPin->m_forceHoverDiwne = true;
	return true;
}

Pin* BasicNodeWithPins::findPinClosestToTheMouse(const std::vector<Ptr<Pin>>& pins, float& minDistance)
{
	minDistance = FLT_MAX;
	Pin* closestPin = nullptr;
	for (auto& pin : pins)
	{
		if (pin->m_isPressed)
		{
			// If the pin is pressed, we declare it as the closest to retain its hover status
			// This prevents issues with mouse being pressed near a pin, and then dragged towards another closer pin
			// without triggering a drag operation (due to mouse drag threshold).
			// The original pressed pin should remain the closest pin even though it is no longer, to allow its drag
			// operation to start.
			minDistance = 0;
			return pin.get();
		}
		ImVec2 diff = diwne.canvas().screen2diwne(diwne.input().bypassGetMousePos()) - pin->getPinRect().GetCenter();
		float dist = DMath::len(diff);
		if (dist < minDistance)
		{
			minDistance = dist;
			closestPin = pin.get();
		}
	}
	return closestPin;
}

void BasicNodeWithPins::afterDraw(DrawInfo& context)
{
	Super::afterDraw(context);

	// If pin icons stick out of the node (when pin offset is negative), their drawing is deferred to us.
	// Meaning this node will draw the pin icons using already prepared data from the pins.
	// This is done so that any node borders or hover/selection indicators are drawn UNDER the pins.
	for (auto& pin : m_leftPins)
	{
		// if (!pin->m_pinIconData.rendered)
		// 	pin->renderPinDiwne(pin->m_pinIconData);
	}
	for (auto& pin : m_rightPins)
	{
		// if (!pin->m_pinIconData.rendered)
		// 	pin->renderPinDiwne(pin->m_pinIconData);
	}
}

void BasicNodeWithPins::leftContent(DrawInfo& context)
{
	bool pinsVisible = false;

	// todo (PF) effectivity???
	for (auto& pin : m_leftPins)
	{
		if (pin->allowDrawing())
		{
			pinsVisible = true;
			break;
		}
	}

	if (pinsVisible)
	{
		drawInputPins(context);
	}
}

void BasicNodeWithPins::rightContent(DrawInfo& context)
{
	bool pinsVisible = false;

	for (auto pin : m_rightPins)
	{
		if (pin->allowDrawing())
		{
			pinsVisible = true;
			break;
		}
	}

	if (pinsVisible)
	{
		drawOutputPins(context);
	}
}

void BasicNodeWithPins::unplugAll()
{
	for (auto& pin : m_inputs)
		pin->unplug();
	for (auto& pin : m_outputs)
		pin->unplug();
}

void BasicNodeWithPins::drawInputPins(DrawInfo& context)
{
	for (auto const& pin : m_leftPins)
	{
		if (pin->allowDrawing())
		{
			pin->drawDiwne(context, m_drawMode);
			DGui::NewLine(); // Remove y item spacing
		}
	}
}

void BasicNodeWithPins::drawOutputPins(DrawInfo& context)
{
	m_outputPinsVstack.begin();
	for (auto const& pin : m_rightPins) // subset of outputs, based of the level
	{
		if (pin->allowDrawing())
		{
			DiwnePanel* row = m_outputPinsVstack.beginRow();
			row->spring(1.0f);
			pin->drawDiwne(context);
			m_outputPinsVstack.endRow();
		}
	}
	m_outputPinsVstack.end();
}

bool BasicNodeWithPins::allowPress(const DrawInfo& context) const
{
	if (!Super::allowPress(context))
		return false;
	return true;
}

void BasicNodeWithPins::onDestroy(bool logEvent)
{
	for (auto pin : m_inputs)
		pin->destroy(logEvent);
	for (auto pin : m_outputs)
		pin->destroy(logEvent);
	Super::onDestroy(logEvent);
}

void BasicNodeWithPins::translate(const ImVec2& vec)
{
	Super::translate(vec);
	// Pin rect's need to be moved as well
	for (auto pin : m_inputs)
		pin->translate(vec);
	for (auto pin : m_outputs)
		pin->translate(vec);
}
} // namespace DIWNE