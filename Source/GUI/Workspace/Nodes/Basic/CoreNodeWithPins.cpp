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
		switch (pin.ValueType)
		{
		case Core::EValueType::MatrixMul:
			//                m_workspaceInputs.push_back(std::make_unique<WorkspaceCoreInputPinMatrixMul>(
			//                pin.Id
			//                                                                ,   pin
			//                                                                ,
			//                                                                *this));
			m_workspaceInputs.push_back(std::make_unique<CoreInPin>(diwne, pin.Id, pin, *this));
			break;
			//            case Core::EValueType::Vec4:
			//                m_workspaceInputs.push_back(std::make_unique<WorkspaceCoreInputPinVector4>(
			//                pin.Id
			//                                                                                            ,   pin
			//                                                                                            ,   *this));
			//                break;
			//            case Core::EValueType::Vec3:
			//                m_workspaceInputs.push_back(std::make_unique<WorkspaceCoreInputPinVector3>(
			//                pin.Id
			//                                                                                            ,   pin
			//                                                                                            ,   *this));
			//                break;
			//            case Core::EValueType::Float:
			//                m_workspaceInputs.push_back(std::make_unique<WorkspaceCoreInputPinFloat>(
			//                pin.Id
			//                                                                                            ,   pin
			//                                                                                            ,   *this));
			//                break;
			//            case Core::EValueType::Quat:
			//                m_workspaceInputs.push_back(std::make_unique<WorkspaceCoreInputPinQuaternion>(
			//                pin.Id
			//                                                                                            ,   pin
			//                                                                                            ,   *this));
			//                break;
			//            case Core::EValueType::Pulse:
			//                m_workspaceInputs.push_back(std::make_unique<WorkspaceCoreInputPinPulse>(
			//                pin.Id
			//                                                                                            ,   pin
			//                                                                                            ,   *this));
			//                break;
			//            case Core::EValueType::MatrixMul:
			//                m_workspaceInputs.push_back(std::make_unique<WorkspaceCoreInputPinMatrixMul>(
			//                pin.Id
			//                                                                                            ,   pin
			//                                                                                            ,   *this));
			//                break;
			//            case Core::EValueType::Screen:
			//                m_workspaceInputs.push_back(std::make_unique<WorkspaceCoreInputPinScreen>(
			//                pin.Id
			//                                                                                            ,   pin
			//                                                                                            ,   *this));
			//                break;
			//            case Core::EValueType::Ptr:
			//                /* Pin with type Ptr have no graphic representation */
			//                break;
		default:
			m_workspaceInputs.push_back(std::make_unique<CoreInPin>(diwne, pin.Id, pin, *this));
			// Debug::Assert(false , "Unknown Pin type while loading input pins from
			// Core to Workspace");
		}
	}
	if (!m_showDataOnPins) /* default true, false for Camera and Sequence - they don't show data on their output pins*/
	{
		for (Core::Pin const& pin : outputPins)
		{
			m_workspaceOutputs.push_back(std::make_unique<CoreOutPin>(diwne, pin.Id, pin, *this));
		}
	}
	else
	{

		auto outs = nodebase->getOutputPins();
		auto outsBegin = outs.begin();
		auto outsEnd = outs.end();

		for (Core::Pin const& pin : outputPins)
		{
			switch (pin.ValueType)
			{
			case Core::EValueType::Matrix:
				m_workspaceOutputs.push_back(std::make_unique<DataOutPinMatrix>(diwne, pin.Id, pin, *this));
				break;
			case Core::EValueType::Vec4:
				m_workspaceOutputs.push_back(std::make_unique<DataOutPinVector4>(diwne, pin.Id, pin, *this));
				break;
			case Core::EValueType::Vec3:
				m_workspaceOutputs.push_back(std::make_unique<DataOutPinVector3>(diwne, pin.Id, pin, *this));
				break;
			case Core::EValueType::Float:
				m_workspaceOutputs.push_back(std::make_unique<DataOutPinFloat>(diwne, pin.Id, pin, *this));
				break;
			case Core::EValueType::Quat:
				m_workspaceOutputs.push_back(std::make_unique<DataOutPinQuat>(diwne, pin.Id, pin, *this));
				break;
			case Core::EValueType::Pulse:
				m_workspaceOutputs.push_back(std::make_unique<DataOutPinPulse>(diwne, pin.Id, pin, *this));
				break;
			case Core::EValueType::MatrixMul:
				m_workspaceOutputs.push_back(std::make_unique<CoreOutPinMatrixMultiply>(diwne, pin.Id, pin, *this));
				break;
			case Core::EValueType::Screen:
				m_workspaceOutputs.push_back(std::make_unique<DataOutPinScreen>(diwne, pin.Id, pin, *this));
				break;
			case Core::EValueType::Ptr:
				/* Pin with type Ptr have no graphic representation */
				break;
			default:
				I3T_ABORT("Unknown Pin type while loading output pins from "
				          "Core to Workspace");
			}
		}
	}
}

bool CoreNodeWithPins::finalize()
{
	bool inner_interaction_happen = false;
	WorkspaceDiwne& wd = static_cast<WorkspaceDiwne&>(diwne);

	if (!allowDrawing())
	{
		for (auto const& pin : m_workspaceInputs)
		{
			if (pin->isConnected())
			{
				Ptr<CoreInPin> in = std::dynamic_pointer_cast<CoreInPin>(pin);
				if (in->getLink().isLinkOnWorkArea())
				{
					// inner_interaction_happen |= in->getLink().drawDiwne();
					wd.m_linksToDraw.push_back(&pin->getLink());
				}
			}
		}
	}
	inner_interaction_happen |= CoreNode::finalize();
	return inner_interaction_happen;
}

bool CoreNodeWithPins::leftContent()
{
	bool inner_interaction_happen = false;
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
			ImRect nodeRect = getNodeRectDiwne();
			ImVec2 pinConnectionPoint = ImVec2(nodeRect.Min.x, (nodeRect.Min.y + nodeRect.Max.y) / 2);
			for (auto const& pin : m_workspaceInputs)
			{
				// if (!pin->getCorePin().isRendered()) // todo (PF) Label did not draw the wires!
				{
					pin->setConnectionPointDiwne(pinConnectionPoint);
					if (pin->isConnected())
					{
						wd.m_linksToDraw.push_back(&pin->getLink()); // register the wire
					}
				}
			}
		}
		else
		{
			for (auto const& pin : m_workspaceInputs)
			{
				if (pin->getCorePin().isRendered())
				{
					inner_interaction_happen |= pin->drawDiwne(); // pin + register the wire
				}
			}
		}
	}
	return inner_interaction_happen;
}

bool CoreNodeWithPins::rightContent()
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

	if (pinsVisible)
	{
		if (m_levelOfDetail == LevelOfDetail::Label ||   // Label draws the wires only
		    m_levelOfDetail == LevelOfDetail::SetValues) // SetValues must add the invisible Pulse outputs
		{
			// register the connected wires only.
			// Connect them to the middle of the box right side (showing just the label)

			const ImRect nodeRect = getNodeRectDiwne();
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
					float act_align = std::max(0.0f, (m_rightRectDiwne.GetWidth() - pin->getRectDiwne().GetWidth()) *
					                                     diwne.getWorkAreaZoom()); /* no shift to the left */
					m_minRightAlignOfRightPins =
					    std::min(m_minRightAlignOfRightPins, act_align); /* over all min align is 0 when no switching
					                                                        between two node sizes */
					const float cursor_pos = ImGui::GetCursorPosX();
					// LOG_INFO(cursor_pos);
					ImGui::SetCursorPosX(cursor_pos + act_align - prev_minRightAlign); /* right align if not all output
					                                                                      pins have the same width */
					inner_interaction_happen |= pin->drawDiwne();
				}
			}
		}
	}
	else
	{
		// TODO: (DR) (zoom-aware) Uncomment perhaps
		// ImGui::Dummy(I3T::getUI()->getTheme().get(ESizeVec2::Nodes_noPinsSpacing));
	}
	return inner_interaction_happen;
}
