/**
 * \file
 * \brief Jaroslav Holeček <holecek.jaroslav@email.cz>
 * \authors Sofie Sasorina, Martin Herich <martin.herich@phire.cz>
 * \copyright Copyright (C) 2016-2023 I3T team, Department of Computer Graphics
 * and Interaction, FEE, Czech Technical University in Prague, Czech Republic
 *
 * This file is part of I3T - An Interactive Tool for Teaching Transformations
 * http://www.i3t-tool.org
 *
 * GNU General Public License v3.0 (see LICENSE.txt or https://www.gnu.org/licenses/gpl-3.0.txt)
 */
#include "WorkspaceCycle.h"
#include "../Windows/WorkspaceWindow.h"

WorkspaceCycle::WorkspaceCycle(DIWNE::Diwne& diwne, Ptr<Core::Node> nodebase /*=Core::GraphManager::createCycle()*/,
                               bool showDataOnPins /*=true*/)
    : WorkspaceNodeWithCoreDataWithPins(diwne, nodebase, showDataOnPins)
{
	// m_workspaceInputs[Core::I3T_CYCLE_IN_PLAY]->m_iconType = DIWNE::TriangleRight;
	m_workspaceInputs[Core::I3T_CYCLE_IN_PAUSE]->m_iconType = DIWNE::Pause;
	m_workspaceInputs[Core::I3T_CYCLE_IN_STOP]->m_iconType = DIWNE::Stop;
	// m_workspaceInputs[Core::I3T_CYCLE_IN_PREV]->m_iconType = DIWNE::SkipBack;
	m_workspaceInputs[Core::I3T_CYCLE_IN_PREV]->m_iconType = DIWNE::SkipBack2;
	// m_workspaceInputs[Core::I3T_CYCLE_IN_NEXT]->m_iconType = DIWNE::SkipForward;
	m_workspaceInputs[Core::I3T_CYCLE_IN_NEXT]->m_iconType = DIWNE::SkipForward2;

	// m_workspaceOutputs[Core::I3T_CYCLE_OUT_PLAY]->m_iconType = DIWNE::TriangleRight;
	m_workspaceOutputs[Core::I3T_CYCLE_OUT_PAUSE]->m_iconType = DIWNE::Pause;
	m_workspaceOutputs[Core::I3T_CYCLE_OUT_STOP]->m_iconType = DIWNE::Stop;
	m_workspaceOutputs[Core::I3T_CYCLE_OUT_PREV]->m_iconType = DIWNE::SkipBack2;
	m_workspaceOutputs[Core::I3T_CYCLE_OUT_NEXT]->m_iconType = DIWNE::SkipForward2;
	m_workspaceOutputs[Core::I3T_CYCLE_OUT_BEGIN]->m_iconType = DIWNE::Rewind;
	m_workspaceOutputs[Core::I3T_CYCLE_OUT_END]->m_iconType = DIWNE::FastForward;

	updateDataItemsWidth(); /* \todo Jh make "processinfirstframe" function in Node
	                        and run settings data width in it */
}

bool WorkspaceCycle::isCycle()
{
	return true;
}

void WorkspaceCycle::drawMenuLevelOfDetail()
{
	drawMenuLevelOfDetail_builder(std::dynamic_pointer_cast<WorkspaceNodeWithCoreData>(shared_from_this()),
	                              {WorkspaceLevelOfDetail::Full, WorkspaceLevelOfDetail::SetValues,
	                               WorkspaceLevelOfDetail::LightCycle, WorkspaceLevelOfDetail::Label});
}

bool WorkspaceCycle::buttonPlayPause()
{
	ImGui::PushFont(I3T::getUI()->getTheme().get(EFont::TutorialTitle));
	// std::u8string playCharacter = u8"⯈";
	// if (ImGui::SmallButton(reinterpret_cast<const char*>(playCharacter.c_str())))
	// if (ImGui::SmallButton("\u2BC8"))
	if (ImGui::SmallButton(fmt::format("{}", "⏩").c_str()))
	{
		if (m_nodebase->as<Core::Cycle>()->isRunning())
		{
			m_nodebase->as<Core::Cycle>()->pause();
		}
		else
		{
			m_nodebase->as<Core::Cycle>()->play();
		}
		ImGui::PopFont();
		return true;
	}
	ImGui::PopFont();
	return false;
}

bool WorkspaceCycle::buttonStopAndReset()
{
	if (ImGui::SmallButton("SaR"))
	{
		m_nodebase->as<Core::Cycle>()->stopAndReset();
		return true;
	}
	return false;
}

bool WorkspaceCycle::buttonStepBack()
{
	if (ImGui::SmallButton("SB"))
	{
		m_nodebase->as<Core::Cycle>()->stepBack();
		return true;
	}
	return false;
}

bool WorkspaceCycle::buttonStepNext()
{
	if (ImGui::SmallButton("SN"))
	{
		m_nodebase->as<Core::Cycle>()->stepNext();
		return true;
	}
	return false;
}


bool WorkspaceCycle::buttonRewind()
{
	if (ImGui::SmallButton("RR"))
	{
		m_nodebase->as<Core::Cycle>()->rewind();
		return true;
	}
	return false;
}

bool WorkspaceCycle::buttonWind()
{
	if (ImGui::SmallButton("WW"))
	{
		m_nodebase->as<Core::Cycle>()->wind();
		return true;
	}
	return false;
}


bool WorkspaceCycle::leftContent()
{
	// TODO
	// - icons for input pins
	// - no labels for pulse pins - put them to the hover

	bool inner_interaction_happen = false;

	ImGui::PushStyleVar(ImGuiStyleVar_FramePadding,
	                    I3T::getSize(ESizeVec2::Nodes_FloatPadding) * diwne.getWorkAreaZoom());
	ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing,
	                    I3T::getSize(ESizeVec2::Nodes_ItemsSpacing) * diwne.getWorkAreaZoom());

	if (m_levelOfDetail == WorkspaceLevelOfDetail::LightCycle)
	{
		for (auto const i : {Core::I3T_CYCLE_IN_FROM, Core::I3T_CYCLE_IN_TO, Core::I3T_CYCLE_IN_STEP})
		{
			m_workspaceInputs.at(i)->drawDiwne();
		}

		WorkspaceDiwne& wd = static_cast<WorkspaceDiwne&>(diwne);
		ImRect nodeRect = getNodeRectDiwne();
		ImVec2 pinConnectionPoint =
		    ImVec2(nodeRect.Min.x, (diwne.screen2diwne(ImGui::GetCursorScreenPos()).y + nodeRect.Max.y) / 2);

		for (auto const i : {Core::I3T_CYCLE_IN_PLAY, Core::I3T_CYCLE_IN_PAUSE, Core::I3T_CYCLE_IN_STOP,
		                     Core::I3T_CYCLE_IN_PREV, Core::I3T_CYCLE_IN_NEXT})
		{
			Ptr<WorkspaceCoreInputPin> const pin = m_workspaceInputs.at(i);
			pin->setConnectionPointDiwne(pinConnectionPoint);
			if (pin->isConnected())
			{
				wd.m_linksToDraw.push_back(&pin->getLink());
			}
		}
	}
	else if (m_levelOfDetail == WorkspaceLevelOfDetail::Label)
	{
		// inner_interaction_happen |= WorkspaceNode::leftContent();
		inner_interaction_happen |= WorkspaceNodeWithCoreDataWithPins::leftContent();
	}
	else // setValues or Full  // todo (PF) missing wires to pulse input pins for setValues mode
	{
		// all pins - icons and labels only
		// inner_interaction_happen |= WorkspaceNodeWithCoreDataWithPins::leftContent(); // icon and label

		// ImGui::AlignTextToFramePadding();


		// From: -------- | > | from | value |
		ImGui::BeginGroup();
		inner_interaction_happen |= m_workspaceInputs.at(Core::I3T_CYCLE_IN_FROM)->drawDiwne(); // icon and label
		ImGui::EndGroup();
		const float fromWidth = ImGui::GetItemRectSize().x; // length of the icon + label (from is the longest text)

		// value
		float localData = m_nodebase->as<Core::Cycle>()->getFrom();
		bool valueChanged = false;

		ImGui::PushStyleVar(ImGuiStyleVar_FramePadding,
		                    I3T::getSize(ESizeVec2::Nodes_FloatPadding) * diwne.getWorkAreaZoom());
		ImGui::SameLine();
		ImGui::Text(""); // probably adds an item separator
		ImGui::SameLine();
		ImGui::SetNextItemWidth(getDataItemsWidth());
		inner_interaction_happen |= drawDragFloatWithMap_Inline(
		    diwne, getNumberOfVisibleDecimal(), m_floatPopupMode, fmt::format("##{}from", getId()), localData,
		    m_workspaceInputs.at(Core::I3T_CYCLE_IN_FROM)->isConnected() ? Core::EValueState::Locked
		                                                                 : Core::EValueState::Editable,
		    valueChanged);
		ImGui::PopStyleVar(); // ImGuiStyleVar_FramePadding
		// if (ImGui::IsItemHovered())
		// {
		//	ImGui::SetTooltip("From (begin of the interval of the cycle value)");
		// }
		if (valueChanged)
		{
			m_nodebase->as<Core::Cycle>()->setFrom(localData);
			updateDataItemsWidth();
		}

		inner_interaction_happen |= valueChanged;


		// To: --------
		ImGui::BeginGroup();
		inner_interaction_happen |= m_workspaceInputs.at(Core::I3T_CYCLE_IN_TO)->drawDiwne(); // icon and label
		ImGui::EndGroup();
		float toWidth = ImGui::GetItemRectSize().x;

		// space to maintain values in the column
		ImGui::SameLine();
		ImGui::Dummy(ImVec2(fromWidth - toWidth, 0.0));

		// value
		localData = m_nodebase->as<Core::Cycle>()->getTo();
		valueChanged = false;

		ImGui::SameLine();
		ImGui::SetNextItemWidth(getDataItemsWidth());
		ImGui::PushStyleVar(ImGuiStyleVar_FramePadding,
		                    I3T::getSize(ESizeVec2::Nodes_FloatPadding) * diwne.getWorkAreaZoom());
		inner_interaction_happen |= drawDragFloatWithMap_Inline(
		    diwne, getNumberOfVisibleDecimal(), m_floatPopupMode, fmt::format("##{}to", getId()), localData,
		    m_workspaceInputs.at(Core::I3T_CYCLE_IN_TO)->isConnected() ? Core::EValueState::Locked
		                                                               : Core::EValueState::Editable,
		    valueChanged);
		ImGui::PopStyleVar(); // ImGuiStyleVar_FramePadding

		// if (ImGui::IsItemHovered())
		// {
		//	ImGui::SetTooltip("To (end of the interval of the cycle value)");
		// }
		if (valueChanged)
		{
			m_nodebase->as<Core::Cycle>()->setTo(localData);
			updateDataItemsWidth();
		}

		// Step: --------
		ImGui::BeginGroup();
		inner_interaction_happen |= m_workspaceInputs.at(Core::I3T_CYCLE_IN_STEP)->drawDiwne(); // icon and label
		ImGui::EndGroup();
		float stepWidth = ImGui::GetItemRectSize().x;

		// space to maintain values in the column
		ImGui::SameLine();
		ImGui::Dummy(ImVec2(fromWidth - stepWidth, 0.0));

		// value
		localData = m_nodebase->as<Core::Cycle>()->getStep();
		valueChanged = false;
		ImGui::SameLine();
		ImGui::SetNextItemWidth(getDataItemsWidth());
		ImGui::PushStyleVar(ImGuiStyleVar_FramePadding,
		                    I3T::getSize(ESizeVec2::Nodes_FloatPadding) * diwne.getWorkAreaZoom());
		inner_interaction_happen |= drawDragFloatWithMap_Inline(
		    diwne, getNumberOfVisibleDecimal(), m_floatPopupMode, fmt::format("##{}step", getId()), localData,
		    m_workspaceInputs.at(Core::I3T_CYCLE_IN_STEP)->isConnected() ? Core::EValueState::Locked
		                                                                 : Core::EValueState::Editable,
		    valueChanged);
		ImGui::PopStyleVar(); // ImGuiStyleVar_FramePadding
		// if (ImGui::IsItemHovered())
		// {
		//	ImGui::SetTooltip("Step (single step value in the defined step duration)");
		// }
		if (valueChanged)
		{
			m_nodebase->as<Core::Cycle>()->setStep(localData);
			updateDataItemsWidth();
		}
		if (m_levelOfDetail == WorkspaceLevelOfDetail::Full)
		{
			// the remaining pins
			for (auto const i : {Core::I3T_CYCLE_IN_PLAY, Core::I3T_CYCLE_IN_PAUSE, Core::I3T_CYCLE_IN_STOP,
			                     Core::I3T_CYCLE_IN_PREV, Core::I3T_CYCLE_IN_NEXT})
			{
				// inner_interaction_happen |= m_workspaceInputs.at(i)->m_iconType
				inner_interaction_happen |= m_workspaceInputs.at(i)->drawDiwne();
			}
		}
		else if (m_levelOfDetail == WorkspaceLevelOfDetail::SetValues)
		{
			// the remaining pins are hidden, draw the wires only
			WorkspaceDiwne& wd = static_cast<WorkspaceDiwne&>(diwne);
			const ImRect nodeRect = getNodeRectDiwne();
			const ImVec2 pinConnectionPoint =
			    ImVec2(nodeRect.Min.x, (nodeRect.Min.y + nodeRect.Max.y) / 2.0f + 30.0f); // (PF) The Piscvejc constant
			for (auto const i : {Core::I3T_CYCLE_IN_PLAY, Core::I3T_CYCLE_IN_PAUSE, Core::I3T_CYCLE_IN_STOP,
			                     Core::I3T_CYCLE_IN_PREV, Core::I3T_CYCLE_IN_NEXT})
			{
				auto const& pin = m_workspaceInputs.at(i);
				// if (!pin->getCorePin().isRendered()) // todo (PF) Label did not draw the wires!
				{
					pin->setConnectionPointDiwne(pinConnectionPoint);
					if (pin->isConnected())
					{
						wd.m_linksToDraw.push_back(&pin->getLink());
					}
				}
			}
		}
	}
	ImGui::PopStyleVar(2);
	return inner_interaction_happen;
}

bool WorkspaceCycle::rightContent()
{
	// TODO
	// - icons for output pins DONE
	// - no labels for pulse pins DONE
	// - put labels to the hover - postponed

	bool inner_interaction_happen = false;
	// if (m_levelOfDetail == WorkspaceLevelOfDetail::LightCycle)
	//{
	//	for (auto const i : {Core::I3T_CYCLE_OUT_VAL})
	//	{
	//		m_workspaceOutputs.at(i)->drawDiwne();
	//	}

	//	WorkspaceDiwne& wd = static_cast<WorkspaceDiwne&>(diwne);
	//	ImRect nodeRect = getNodeRectDiwne();
	//	// todo (PF) - connection point is wrong (in the inputs, not in the outputs)
	//	ImVec2 pinConnectionPoint =
	//	    ImVec2(nodeRect.Min.x, (diwne.screen2diwne(ImGui::GetCursorScreenPos()).y + nodeRect.Max.y) / 2);

	//	for (auto const i : {Core::I3T_CYCLE_OUT_PLAY, Core::I3T_CYCLE_OUT_PAUSE, Core::I3T_CYCLE_OUT_STOP,
	//	                     Core::I3T_CYCLE_OUT_PREV, Core::I3T_CYCLE_OUT_NEXT, Core::I3T_CYCLE_OUT_END})
	//	{
	//		m_workspaceOutputs.at(i)->setConnectionPointDiwne(pinConnectionPoint);
	//	}
	//}
	// else
	{
		inner_interaction_happen |= WorkspaceNodeWithCoreDataWithPins::rightContent();
	}

	return inner_interaction_happen;
}

bool WorkspaceCycle::topContent()
{
	diwne.AddRectFilledDiwne(m_topRectDiwne.Min, m_topRectDiwne.Max, I3T::getTheme().get(EColor::NodeHeader),
	                         I3T::getSize(ESize::Nodes_Rounding), ImDrawCornerFlags_Top);

	return WorkspaceNodeWithCoreData::topContent();
}

bool WorkspaceCycle::myRadioButton(const char* label, int* v, int v_button)
{
	// ImGui::PushID() / PopID()

	if (*v == v_button)
	{
		// todo - set the colors in the theme file
		ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4) ImColor::HSV(*v / 7.0f, 0.6f, 0.6f));
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4) ImColor::HSV(*v / 7.0f, 0.7f, 0.7f));
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4) ImColor::HSV(*v / 7.0f, 0.8f, 0.8f));
	}
	else
	{
		ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4) ImColor::HSV(*v / 7.0f, 0.6f / 4.0f, 0.6f));
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4) ImColor::HSV(*v / 7.0f, 0.7f / 4.0f, 0.7f));
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4) ImColor::HSV(*v / 7.0f, 0.8f / 4.0f, 0.8f));
	}

	bool pressed = false;
	// pressed = ImGui::Button(fmt::format("{}##{}",label, v_button).c_str());
	pressed = ImGui::Button(label);
	if (pressed)
	{
		*v = v_button;
		std::cout << "button " << v_button << ", diwneID " << getId() << ", label >" << label << "<" << std::endl;
	}
	ImGui::PopStyleColor(3);
	return pressed;
}

bool WorkspaceCycle::middleContent()
{
	// \todo Add icons to buttons
	// std::u8string string = u8"ěščřžýáíé";
	// std::string s(string.cbegin(), string.cend());

	// fmt::print("{}\n", "ф");
	// std::cout << fmt::format("{}\n", "⏩") << std::endl;


	bool inner_interaction_happen = false;
	// ImVec2 button_sz =
	// I3T::getSize(ESizeVec2::Nodes_FloatCycleButtonSize)*diwne.getWorkAreaZoom();
	float localData;
	bool valueChanged;

	ImGui::PushStyleVar(ImGuiStyleVar_FramePadding,
	                    I3T::getSize(ESizeVec2::Nodes_FloatPadding) * diwne.getWorkAreaZoom());
	ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing,
	                    I3T::getSize(ESizeVec2::Nodes_ItemsSpacing) * diwne.getWorkAreaZoom());

	switch (m_levelOfDetail)
	{
	case WorkspaceLevelOfDetail::Label:
		break;
	// case WorkspaceLevelOfDetail::LightCycle:

	//	inner_interaction_happen |= buttonPlayPause();
	//	ImGui::SameLine();
	//	inner_interaction_happen |= buttonStopAndReset();

	//	valueChanged = false;
	//	localData = m_nodebase->as<Core::Cycle>()->getData().getFloat();
	//	ImGui::SetNextItemWidth(getDataItemsWidth());
	//	inner_interaction_happen |= drawDragFloatWithMap_Inline(
	//	    diwne, getNumberOfVisibleDecimal(), m_floatPopupMode, fmt::format("##{}Value", getId()), localData,
	//	    /* m_nodebase->as<Core::Cycle>()->getValueState({0, 0}) - not work */
	//	    Core::EValueState::Editable, valueChanged); /* \todo MH PF Always editable?*/
	//	if (ImGui::IsItemHovered())
	//	{
	//		ImGui::SetTooltip("Value");
	//	}
	//	if (valueChanged)
	//	{
	//		// ask MH \\ \todo SetValue does not call setDirectionMultiplier()
	//		m_nodebase->setValue(localData);
	//		updateDataItemsWidth();
	//	}

	//	break;
	case WorkspaceLevelOfDetail::Full:
	case WorkspaceLevelOfDetail::SetValues:
	case WorkspaceLevelOfDetail::LightCycle:
		// TODO
		// 1. center
		// 2. big buttons with icons
		// 4. radio button once/repeat/ping-pong
		// 5. reorder the rest

		// button height
		// CalcTextSize("") should return ImVec2(0.0f, g.FontSize)
		// const ImVec2 label_size = ImGui::CalcTextSize("") * 1.2;

		// static float scale = 0.0;
		// ImGui::SetNextItemWidth(30.0f);
		// ImGui::DragFloat("Scale ", &scale, 0.1f, -0.5f, 2.0f);


		ImGui::Indent();
		ImGui::Indent();

		// -----------------------------------------------------
		// LINE SPACING
		// -----------------------------------------------------

		const ImVec2 itemSpacing = ImGui::GetStyle().ItemSpacing;
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, (ImVec2(itemSpacing.x, 2.5f * itemSpacing.y)));


		// -----------------------------------------------------
		// BUTTONS - play, stop, skip
		// -----------------------------------------------------

		ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding,
		                    I3T::getUI()->getTheme().get(ESize::StartWindow_FrameRounding));


		inner_interaction_happen |= buttonPlayPause();
		ImGui::SameLine();
		inner_interaction_happen |= buttonStopAndReset();

		if (m_levelOfDetail == WorkspaceLevelOfDetail::LightCycle) // draw play and stop buttons only
		{
			ImGui::PopStyleVar(2); // ImGuiStyleVar_ItemSpacing, ImGuiStyleVar_FrameRounding
			break;
		}

		ImGui::SameLine();
		inner_interaction_happen |= buttonRewind();
		ImGui::SameLine();
		inner_interaction_happen |= buttonWind();


		// ImGui::Button("New", ImVec2(radioBtnWidth, buttonHeight))

		const float radioBtnWidth = I3T::getUI()->getTheme().get(ESize::StartWindow_LoadButtonWidth);
		const float buttonHeight = I3T::getUI()->getTheme().get(ESize::StartWindow_ButtonHeight);
		// if (ImGui::Button("New", ImVec2(radioBtnWidth, buttonHeight)))
		// {
		//	// this->hide();
		// }

		// ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4) ImColor::HSV(i / 7.0f, 0.6f, 0.6f));
		// ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4) ImColor::HSV(i / 7.0f, 0.7f, 0.7f));
		// ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4) ImColor::HSV(i / 7.0f, 0.8f, 0.8f));
		// inner_interaction_happen |=
		// ImGui::Button("   once_   ", ImVec2(ImGui::CalcTextSize("   once   ") * ImVec2(1.0f, scale)));
		// ImGui::SameLine();
		// inner_interaction_happen |=
		// ImGui::Button("   repeat_   ", ImVec2(ImGui::CalcTextSize("   repeat   ") * ImVec2(1.0f, scale)));
		// ImGui::SameLine();
		// inner_interaction_happen |=
		// ImGui::Button("   ping-pong_   ", ImVec2(ImGui::CalcTextSize("   ping-pong   ") * ImVec2(1.0f, scale)));

		// ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4) ImColor::HSV(i / 7.0f, 0.6f, 0.6f));

		if (inner_interaction_happen)
			int i = 7;

		// -----------------------------------------------------
		// MODE select
		// -----------------------------------------------------
		int mode = static_cast<int>(m_nodebase->as<Core::Cycle>()->getMode());

		const char* str_id = fmt::format("{}modeTest", getId()).c_str();
		ImGui::BeginTable(str_id, 7, ImGuiTableFlags_NoHostExtendX | ImGuiTableFlags_SizingFixedFit);
		ImGui::TableNextRow();

		ImGui::TableNextColumn();
		ImGui::Text("  ");

		ImGui::TableNextColumn();
		inner_interaction_happen |= myRadioButton("   once   ", &mode, 0);

		ImGui::TableNextColumn();
		ImGui::Text(" | ");


		// ImGui::SameLine();
		ImGui::TableNextColumn();
		inner_interaction_happen |= myRadioButton("   repeat   ", &mode, 1);

		ImGui::TableNextColumn();
		ImGui::Text(" | ");

		// ImGui::SameLine();
		ImGui::TableNextColumn();
		inner_interaction_happen |= myRadioButton("   ping-pong   ", &mode, 2);

		ImGui::TableNextColumn();
		ImGui::Text("  ");

		ImGui::EndTable();

		// ImGui::DebugDrawItemRect(ImColor(255, 127, 0, 1));

		// ImGui::TableGetBoundSettings(id);
		// table->OuterRect =
		// ImGuiTableFlags_BordersOuter


		// inner_interaction_happen |= myRadioButton("   once   ##1", &mode, 1);
		// ImGui::SameLine();
		// inner_interaction_happen |= myRadioButton("   repeat   ##2", &mode, 2);
		// ImGui::SameLine();
		// inner_interaction_happen |= myRadioButton("   ping-pong   ##3", &mode, 3);


		// inner_interaction_happen |= myRadioButton("   once   ##2", &mode, 1);
		// ImGui::SameLine();
		// inner_interaction_happen |= myRadioButton("   repeat   ##1", &mode, 2);
		// ImGui::SameLine();
		// inner_interaction_happen |= myRadioButton("   ping-pong   ##3", &mode, 3);

		switch (mode)
		{
		case 0:
			m_nodebase->as<Core::Cycle>()->setMode(Core::Cycle::EMode::Once);
			break;
		case 1:
			m_nodebase->as<Core::Cycle>()->setMode(Core::Cycle::EMode::Repeat);
			break;
		case 2:
			m_nodebase->as<Core::Cycle>()->setMode(Core::Cycle::EMode::PingPong);
			break;
		default:
			break;
		}

		ImGui::PopStyleVar(); // FrameRounding


		// -----------------------------------------------------
		// smooth step line
		// -----------------------------------------------------

#if 0
						valueChanged = false;
		localData = m_nodebase->as<Core::Cycle>()->getData().getFloat();
		ImGui::SetNextItemWidth(getDataItemsWidth());
		inner_interaction_happen |= drawDragFloatWithMap_Inline(
		    diwne, getNumberOfVisibleDecimal(), m_floatPopupMode, fmt::format("##{}Value", getId()), localData,
		    /* m_nodebase->as<Core::Cycle>()->getValueState({0, 0}) - not work */
		    Core::EValueState::Editable, valueChanged); /* \todo MH PF Always editable?*/
		if (ImGui::IsItemHovered())
		{
			ImGui::SetTooltip("Value");
		}
		if (valueChanged)
		{
			// ask MH \\ \todo SetValue does not call setDirectionMultiplier()
			m_nodebase->setValue(localData);
			updateDataItemsWidth();
		}
#endif // 0


		// -----------------------------------------------------
		// smooth step line
		// -----------------------------------------------------

#if 1                                      // use table for the third row
		ImGuiID id = ImGui::GetID(str_id); // once-repeat-ping table ID
		ImGuiTable* table = ImGui::TableFindByID(id);
		ImRect tableRectangle = table->OuterRect;
		ImVec2 tableSize =
		    ImVec2(tableRectangle.Max.x - tableRectangle.Min.x, tableRectangle.Max.y - tableRectangle.Min.y);
		ImVec2 tableSize1 = tableRectangle.GetSize();
		float prevWidth = tableRectangle.GetWidth();


		ImGui::BeginTable("BAF", 2, ImGuiTableFlags_NoHostExtendX | ImGuiTableFlags_SizingFixedFit, tableSize);
		ImGui::TableNextRow();

		// smooth checkbox
		ImGui::TableNextColumn();
		ImGui::Text(" smooth step: ");

		ImGui::SameLine();
		auto smooth = m_nodebase->as<Core::Cycle>()->getSmoothStep();
		if (ImGui::Checkbox(fmt::format("##{}smooth", getId()).c_str(), &smooth))
		{
			m_nodebase->as<Core::Cycle>()->setSmoothStep(smooth);
		}

		// step duration
		ImGui::TableNextColumn();
		ImGui::Text("      step duration: ");

		ImGui::SameLine();
		localData = m_nodebase->as<Core::Cycle>()->getStepDuration();
		valueChanged = false;
		ImGui::SetNextItemWidth(getDataItemsWidth()); // \todo according to the number of visible decimals
		inner_interaction_happen |= drawDragFloatWithMap_Inline(diwne, getNumberOfVisibleDecimal(), m_floatPopupMode,
		                                                        fmt::format("##{}stepduration", getId()), localData,
		                                                        Core::EValueState::Editable, valueChanged);
		if (ImGui::IsItemHovered())
		{
			ImGui::SetTooltip("Automatic step duration");
		}
		if (valueChanged)
		{
			m_nodebase->as<Core::Cycle>()->setStepDuration(localData);
			updateDataItemsWidth();
		}


		ImGui::EndTable();


#else // use table for the third row -- use just the same line

		// smooth checkbox
		ImGui::Text("  smooth step: ");

		ImGui::SameLine();
		auto smooth = m_nodebase->as<Core::Cycle>()->getSmoothStep();
		if (ImGui::Checkbox(fmt::format("##{}smooth", getId()).c_str(), &smooth))
		{
			m_nodebase->as<Core::Cycle>()->setSmoothStep(smooth);
		}

		// step duration
		ImGui::SameLine();
		ImGui::Text("step duration: ");

		ImGui::SameLine();
		localData = m_nodebase->as<Core::Cycle>()->getStepDuration();
		valueChanged = false;
		ImGui::SetNextItemWidth(getDataItemsWidth()); // \todo according to the number of visible decimals
		inner_interaction_happen |= drawDragFloatWithMap_Inline(diwne, getNumberOfVisibleDecimal(), m_floatPopupMode,
		                                                        fmt::format("##{}stepduration", getId()), localData,
		                                                        Core::EValueState::Editable, valueChanged);
		if (ImGui::IsItemHovered())
		{
			ImGui::SetTooltip("Automatic step duration");
		}
		if (valueChanged)
		{
			m_nodebase->as<Core::Cycle>()->setStepDuration(localData);
			updateDataItemsWidth();
		}

#endif // 0

		// -----------------------------------------------------
		// Single step line
		// -----------------------------------------------------
		ImGui::Text("             single step: ");

		ImGui::SameLine();

		localData = m_nodebase->as<Core::Cycle>()->getManualStep();
		valueChanged = false;
		ImGui::SetNextItemWidth(getDataItemsWidth());
		inner_interaction_happen |= drawDragFloatWithMap_Inline(diwne, getNumberOfVisibleDecimal(), m_floatPopupMode,
		                                                        fmt::format("##{}manstep", getId()), localData,
		                                                        Core::EValueState::Editable, valueChanged);
		if (ImGui::IsItemHovered())
		{
			ImGui::SetTooltip("Step for manual Next/Prev");
		}
		if (valueChanged)
		{
			m_nodebase->as<Core::Cycle>()->setManualStep(localData);
			updateDataItemsWidth();
		}

		// ImGui::SameLine(0,0);
		ImGui::SameLine();
		ImGui::Text("   ");

		//---------------------------
		ImGui::SameLine();
		inner_interaction_happen |= buttonStepBack();
		ImGui::SameLine();
		inner_interaction_happen |= buttonStepNext();

		//---------------------------

		ImGui::Unindent();
		ImGui::Unindent();
		ImGui::PopStyleVar(1); // ItemSpacing
		break;
	} // switch(m_levelOfDetail))

	ImGui::PopStyleVar(2); // FramePadding, ItemSpacing

	return inner_interaction_happen;
}

int WorkspaceCycle::maxLengthOfData()
{
	Ptr<Core::Cycle> nodebase = m_nodebase->as<Core::Cycle>();
	return std::max({numberOfCharWithDecimalPoint(nodebase->getFrom(), m_numberOfVisibleDecimal),
	                 numberOfCharWithDecimalPoint(nodebase->getTo(), m_numberOfVisibleDecimal),
	                 numberOfCharWithDecimalPoint(nodebase->getManualStep(), m_numberOfVisibleDecimal),
	                 numberOfCharWithDecimalPoint(nodebase->getStep(), m_numberOfVisibleDecimal)});
}
