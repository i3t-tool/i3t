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

#include "GUI/Workspace/WorkspaceDiwne.h"

using namespace Workspace;

CoreNodeWithPins::CoreNodeWithPins(DIWNE::NodeEditor& diwne, Ptr<Core::Node> nodebase, bool showDataOnPins /*=true*/)
    : CoreNode(diwne, nodebase), m_showDataOnPins(showDataOnPins), m_minRightAlignOfRightPins(0)
{
	const auto& inputPins = m_nodebase->getInputPins();
	const auto& outputPins = m_nodebase->getOutputPins();

	m_workspaceInputs.reserve(inputPins.size());
	m_workspaceOutputs.reserve(outputPins.size());

	for (Core::Pin const& pin : inputPins)
	{
		m_workspaceInputs.push_back(std::make_unique<CorePin>(diwne, pin, this, true));
	}

	for (Core::Pin const& corePin : outputPins)
	{
		Ptr<CorePin> pin = std::make_unique<CorePin>(diwne, corePin, this, false);
		// default true, false for Camera and Sequence - they don't show data on their output pins
		pin->m_showData = m_showDataOnPins;
		m_workspaceOutputs.push_back(std::move(pin));
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
			ImRect nodeRect = getRectDiwne();
			ImVec2 pinConnectionPoint = ImVec2(nodeRect.Min.x, (nodeRect.Min.y + nodeRect.Max.y) / 2);
			for (auto const& pin : m_workspaceInputs)
			{
				// if (!pin->getCorePin().isRendered()) // todo (PF) Label did not draw the wires!
				{
					pin->setConnectionPointDiwne(pinConnectionPoint);
					pin->setRendered(false);
				}
			}
		}
		else
		{
			for (auto const& pin : m_workspaceInputs)
			{
				if (pin->getCorePin().isRendered())
				{
					pin->drawDiwne(context, m_drawMode2); // pin + register the wire
				}
			}
		}
	}
}

void CoreNodeWithPins::rightContent(DIWNE::DrawInfo& context)
{
	bool inner_interaction_happen = false;
	bool pinsVisible = false;

	for (auto pin : this->getNodebase()->getOutputPins())
	{
		if (pin.isRendered())
		{
			pinsVisible = true;
			break;
		}
	}

	// TODO: Pins occasionally "vibrate", find out why (probably some kind of pixel rounding issue)
	if (pinsVisible)
	{
		if (m_levelOfDetail == LevelOfDetail::Label ||   // Label draws the wires only
		    m_levelOfDetail == LevelOfDetail::SetValues) // SetValues must add the invisible Pulse outputs
		{
			// register the connected wires only.
			// Connect them to the middle of the box right side (showing just the label)

			const ImRect nodeRect = getRectDiwne();
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
			const float prev_minRightAlign = m_minRightAlignOfRightPins; /* prev is used when node gets
			                                                                smaller (for example when switch from
			                                                                precision 2 to precision 0) */
			m_minRightAlignOfRightPins = FLT_MAX;
			for (auto const& pin : getOutputsToShow()) // subset of outputs, based of the level
			{
				if (pin->getCorePin().isRendered())
				{
					float act_align = std::max(0.0f, (m_right.getWidth() - pin->getRectDiwne().GetWidth()) *
					                                     diwne.getZoom()); /* no shift to the left */
					m_minRightAlignOfRightPins =
					    std::min(m_minRightAlignOfRightPins, act_align); /* over all min align is 0 when no switching
					                                                        between two node sizes */
					const float cursor_pos = ImGui::GetCursorPosX();
					// LOG_INFO(cursor_pos);
					ImGui::SetCursorPosX(cursor_pos + act_align - prev_minRightAlign); /* right align if not all output
					                                                                      pins have the same width */
					pin->drawDiwne(context);
				}
			}
		}
	}
	else
	{
		// TODO: (DR) (zoom-aware) Uncomment perhaps
		// ImGui::Dummy(I3T::getUI()->getTheme().get(ESizeVec2::Nodes_noPinsSpacing));
	}
}

void CoreNodeWithPins::onDestroy(bool logEvent)
{
	Node::onDestroy(logEvent);
	for (auto pin : m_workspaceInputs)
		pin->destroy(logEvent);
	for (auto pin : m_workspaceOutputs)
		pin->destroy(logEvent);
}
