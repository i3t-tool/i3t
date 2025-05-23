/**
 * \file
 * \brief
 * \author Jaroslav Holeček <holecek.jaroslav@email.cz>, Dan Rakušan <rakusan.dan@gmail.com>
 * \copyright Copyright (C) 2016-2023 I3T team, Department of Computer Graphics
 * and Interaction, FEE, Czech Technical University in Prague, Czech Republic
 *
 * This file is part of I3T - An Interactive Tool for Teaching Transformations
 * http://www.i3t-tool.org
 *
 * GNU General Public License v3.0 (see LICENSE.txt or https://www.gnu.org/licenses/gpl-3.0.txt)
 */
#include "CoreNodeWithPins.h"

#include "GUI/Toolkit.h"
#include "GUI/Workspace/WorkspaceDiwne.h"
#include "GUI/Workspace/WorkspaceModule.h"

using namespace Workspace;

CoreNodeWithPins::CoreNodeWithPins(DIWNE::NodeEditor& diwne, Ptr<Core::Node> nodebase, bool showDataOnPins /*=true*/)
    : CoreNode(diwne, nodebase), m_showDataOnPins(showDataOnPins)
{
	m_outputPinsVstack.setSpacing(0); // Pins add their own spacing so the drag areas are flush

	const auto& inputPins = m_nodebase->getInputPins();
	const auto& outputPins = m_nodebase->getOutputPins();

	m_workspaceInputs.reserve(inputPins.size());
	m_workspaceOutputs.reserve(outputPins.size());
	m_leftPins.reserve(inputPins.size());
	m_rightPins.reserve(outputPins.size());

	for (Core::Pin const& corePin : inputPins)
	{
		Ptr<CorePin> pin = std::make_shared<CorePin>(diwne, corePin, this, true);
		m_leftPins.push_back(pin.get());
		m_workspaceInputs.push_back(std::move(pin));
	}

	for (Core::Pin const& corePin : outputPins)
	{
		Ptr<CorePin> pin = std::make_shared<CorePin>(diwne, corePin, this, false);
		// default true, false for Camera and Sequence - they don't show data on their output pins
		pin->m_showData = m_showDataOnPins;
		m_rightPins.push_back(pin.get());
		m_workspaceOutputs.push_back(std::move(pin));
	}
}

void CoreNodeWithPins::begin(DIWNE::DrawInfo& context)
{
	CoreNode::begin(context);

	// Pin drag assist
	if (context.inputFullyAvailable() && !(m_drawMode & DIWNE::DrawMode_JustDraw))
		processPinDragAssist();
}

bool CoreNodeWithPins::processPinDragAssist()
{
	float dragAssistRadius = diwne.style().decimal(DIWNE::Style::PIN_DRAG_ASSIST_RADIUS);
	if (dragAssistRadius <= 0)
		return false;
	float minInputDist;
	float minOutputDist;
	CorePin* closestInput = findPinClosestToTheMouse(m_workspaceInputs, minInputDist);
	CorePin* closestOutput = findPinClosestToTheMouse(m_workspaceOutputs, minOutputDist);
	CorePin* closestPin = minInputDist < minOutputDist ? closestInput : closestOutput;
	float minDist = std::min(minInputDist, minOutputDist);
	float pinAssistRadius = diwne.style().decimal(DIWNE::Style::PIN_DRAG_ASSIST_RADIUS);
	if (minDist > pinAssistRadius)
		return false;
	closestPin->m_forceHoverDiwne = true;
	return true;
}

CorePin* CoreNodeWithPins::findPinClosestToTheMouse(const std::vector<Ptr<CorePin>>& pins, float& minDistance)
{
	minDistance = FLT_MAX;
	CorePin* closestPin = nullptr;
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
		float dist = DIWNE::DMath::len(diff);
		if (dist < minDistance)
		{
			minDistance = dist;
			closestPin = pin.get();
		}
	}
	return closestPin;
}

void CoreNodeWithPins::afterDraw(DIWNE::DrawInfo& context)
{
	Super::afterDraw(context);

	// If pin icons stick out of the node (when pin offset is negative), their drawing is deferred to us.
	// Meaning this node will draw the pin icons using already prepared data from the pins.
	// This is done so that any node borders or hover/selection indicators are drawn UNDER the pins.
	for (auto& pin : m_leftPins)
	{
		if (!pin->m_pinIconData.rendered)
			pin->renderPinDiwne(pin->m_pinIconData);
	}
	for (auto& pin : m_rightPins)
	{
		if (!pin->m_pinIconData.rendered)
			pin->renderPinDiwne(pin->m_pinIconData);
	}
}

void CoreNodeWithPins::leftContent(DIWNE::DrawInfo& context)
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
		WorkspaceDiwne& wd = static_cast<WorkspaceDiwne&>(diwne);

		if (m_levelOfDetail == LevelOfDetail::Label)
		{
			// register the connected wires only.
			// Connect them to the middle of the box left side (showing just the label)
			ImRect nodeRect = getRect();
			ImVec2 pinConnectionPoint = ImVec2(nodeRect.Min.x, (nodeRect.Min.y + nodeRect.Max.y) / 2);
			for (auto const& pin : m_leftPins)
			{
				// if (!pin->getCorePin().isRendered()) // todo (PF) Label did not draw the wires!
				{
					pin->setConnectionPointDiwne(pinConnectionPoint);
				}
			}
		}
		else
		{
			drawInputPins(context);
		}
	}
}

void CoreNodeWithPins::rightContent(DIWNE::DrawInfo& context)
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
		if (m_levelOfDetail == LevelOfDetail::Label ||   // Label draws the wires only
		    m_levelOfDetail == LevelOfDetail::SetValues) // SetValues must add the invisible Pulse outputs
		{
			// register the connected wires only.
			// Connect them to the middle of the box right side (showing just the label)

			const ImRect nodeRect = getRect();
			// todo (PF) pinConnectionPoint is wrong when output pulse pins are not drawn
			const ImVec2 pinConnectionPoint = ImVec2(nodeRect.Max.x, (nodeRect.Min.y + nodeRect.Max.y) / 2);
			for (auto const& pin : m_rightPins)
			{
				pin->setConnectionPointDiwne(pinConnectionPoint);
			}
		}

		// else - include SetValues
		if (m_levelOfDetail != LevelOfDetail::Label) // SetValues must draw the value pin
		{
			drawOutputPins(context);
		}
	}
	else
	{
		// TODO: (DR) (zoom-aware) Uncomment perhaps
		// ImGui::Dummy(I3T::getUI()->getTheme().get(ESizeVec2::Nodes_noPinsSpacing));
	}
}

void CoreNodeWithPins::unplugAll()
{
	for (auto& pin : m_workspaceInputs)
		pin->unplug();
	for (auto& pin : m_workspaceOutputs)
		pin->unplug();
}

void CoreNodeWithPins::drawInputPins(DIWNE::DrawInfo& context)
{
	for (auto const& pin : m_leftPins)
	{
		updatePinStyle(*pin);
		if (pin->allowDrawing())
		{
			pin->drawDiwne(context, m_drawMode);
			DIWNE::DGui::NewLine(); // Remove y item spacing
		}
	}
}

void CoreNodeWithPins::drawOutputPins(DIWNE::DrawInfo& context)
{
	m_outputPinsVstack.begin();
	for (auto const& pin : m_rightPins) // subset of outputs, based of the level
	{
		updatePinStyle(*pin);
		if (pin->allowDrawing())
		{
			DIWNE::DiwnePanel* row = m_outputPinsVstack.beginRow();
			row->spring(1.0f);
			pin->drawDiwne(context);
			m_outputPinsVstack.endRow();
		}
	}
	m_outputPinsVstack.end();
}
void CoreNodeWithPins::updatePinStyle(CorePin& pin)
{
	// Pin styling
	if (pin.getType() == Core::EValueType::MatrixMul)
		pin.m_pinStyle = static_cast<PinStyle>(WorkspaceModule::g_pinStyleMul);
	else if (pin.getType() == Core::EValueType::Pulse)
		pin.m_pinStyle = static_cast<PinStyle>(WorkspaceModule::g_pinStylePulse);
	else if (pin.getType() == Core::EValueType::Screen)
		pin.m_pinStyle = static_cast<PinStyle>(WorkspaceModule::g_pinStyleScreen);
	else
		pin.m_pinStyle = static_cast<PinStyle>(WorkspaceModule::g_pinStyle);
}
bool CoreNodeWithPins::allowPress(const DIWNE::DrawInfo& context) const
{
	if (!Super::allowPress(context))
		return false;
	return true;
}

void CoreNodeWithPins::onDestroy(bool logEvent)
{
	for (auto pin : m_workspaceInputs)
		pin->destroy(logEvent);
	for (auto pin : m_workspaceOutputs)
		pin->destroy(logEvent);
	Super::onDestroy(logEvent);
}

void CoreNodeWithPins::translate(const ImVec2& vec)
{
	Super::translate(vec);
	// Pin rect's need to be moved as well
	for (auto pin : m_workspaceInputs)
		pin->translate(vec);
	for (auto pin : m_workspaceOutputs)
		pin->translate(vec);
}
