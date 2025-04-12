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
#include "CoreNodeWithPins.h"

#include "GUI/Toolkit.h"
#include "GUI/Workspace/WorkspaceDiwne.h"
#include "GUI/Workspace/WorkspaceModule.h"

using namespace Workspace;

CoreNodeWithPins::CoreNodeWithPins(DIWNE::NodeEditor& diwne, Ptr<Core::Node> nodebase, bool showDataOnPins /*=true*/)
    : CoreNode(diwne, nodebase), m_showDataOnPins(showDataOnPins)
{
	const auto& inputPins = m_nodebase->getInputPins();
	const auto& outputPins = m_nodebase->getOutputPins();

	m_workspaceInputs.reserve(inputPins.size());
	m_workspaceOutputs.reserve(outputPins.size());

	for (Core::Pin const& corePin : inputPins)
	{
		Ptr<CorePin> pin = std::make_shared<CorePin>(diwne, corePin, this, true);
		m_workspaceInputs.push_back(std::move(pin));
	}

	for (Core::Pin const& corePin : outputPins)
	{
		Ptr<CorePin> pin = std::make_shared<CorePin>(diwne, corePin, this, false);
		// default true, false for Camera and Sequence - they don't show data on their output pins
		pin->m_showData = m_showDataOnPins;
		m_workspaceOutputs.push_back(std::move(pin));
	}
}

void CoreNodeWithPins::afterDraw(DIWNE::DrawInfo& context)
{
	Super::afterDraw(context);

	// If pin icons stick out of the node (when pin offset is negative), their drawing is deferred to us.
	// Meaning this node will draw the pin icons using already prepared data from the pins.
	// This is done so that any node borders or hover/selection indicators are drawn UNDER the pins.
	for (auto& pin : m_workspaceInputs)
	{
		if (!pin->m_pinIconData.rendered)
			pin->renderPinDiwne(pin->m_pinIconData);
	}
	for (auto& pin : m_workspaceOutputs)
	{
		if (!pin->m_pinIconData.rendered)
			pin->renderPinDiwne(pin->m_pinIconData);
	}
}

void CoreNodeWithPins::leftContent(DIWNE::DrawInfo& context)
{
	bool pinsVisible = false;

	// todo (PF) effectivity???
	for (auto pin : this->getNodebase()->getInputPins())
	{
		if (pin.isRendered())
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
			for (auto const& pin : m_workspaceInputs)
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

	for (auto pin : this->getNodebase()->getOutputPins())
	{
		if (pin.isRendered())
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
			for (auto const& pin : getOutputs())
			{
				if (pin->getCorePin().isRendered())
					pin->setConnectionPointDiwne(pinConnectionPoint);
				else
					int i = 7; // NOOP
			}
		}

		// else - include SetValues
		// uses getOutputsToShow()) = subset of outputs, based of the level. Override function in the WorkspaceCycle
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

void CoreNodeWithPins::drawInputPins(DIWNE::DrawInfo& context)
{
	for (auto const& pin : m_workspaceInputs)
	{
		updatePinStyle(*pin);
		if (pin->allowDrawing())
		{
			pin->drawDiwne(context, m_drawMode);
		}
	}
}

void CoreNodeWithPins::drawOutputPins(DIWNE::DrawInfo& context)
{
	outputPinsVstack.begin();
	for (auto const& pin : getOutputsToShow()) // subset of outputs, based of the level
	{
		updatePinStyle(*pin);
		if (pin->allowDrawing())
		{
			DIWNE::DiwnePanel* row = outputPinsVstack.beginRow();
			row->spring(1.0f);
			pin->drawDiwne(context);
			outputPinsVstack.endRow();
		}
	}
	outputPinsVstack.end();
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
	CoreNode::translate(vec);
	// Pin rect's need to be moved as well
	for (auto pin : m_workspaceInputs)
		pin->translate(vec);
	for (auto pin : m_workspaceOutputs)
		pin->translate(vec);
}
