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

//

bool WorkspaceCycle::buttonPlayPause()
{
	// setup - todo get from themes

	// I3T::getColor(EColor::Cycle_ButtonHovered) I3T::getColor(EColor::Cycle_ButtonActive)


	// const ImColor iconColorBg = ImColor(127, 127, 127, 255); // todo I3T::getColor(EColor::Cycle_ButtonBg);
	// const ImColor iconColorFg = IM_COL32_BLACK;              // todo I3T::getColor(EColor::Cycle_ButtonFg);
	// const ImVec2 iconSize = ImVec2(40.0f, 40.0f) * zoom; // todo use themes:
	// const float rounding = 3.0f;

	ImColor iconColorBg = I3T::getColor(EColor::Cycle_Button);
	const ImColor iconColorFg = I3T::getColor(EColor::Cycle_ButtonForeground);

	const float zoom = diwne.getWorkAreaZoom();
	const ImVec2 iconSize = I3T::getSize(ESizeVec2::Cycle_ButtonSize) * zoom;

	const float rounding = I3T::getSize(ESize::Cycle_ButtonRounding);
	ImVec4 padding;

	// Invisible button
	ImVec2 position = ImGui::GetCursorPos();

	const bool button_pressed = ImGui::InvisibleButton("playPause1", iconSize);
	// bool button_hovered = ImGui::IsItemHovered();
	// ImGui::Text("Button 1: %s%s", button_hovered ? "hovered " : "", button_pressed ? "PRESSED " : "");
	if (ImGui::IsItemHovered())
		iconColorBg = I3T::getColor(EColor::Cycle_ButtonHovered);
	if (button_pressed)
		iconColorBg = I3T::getColor(EColor::Cycle_ButtonActive);

	const DIWNE::IconType iconTypeBg = DIWNE::IconType::Rectangle;
	DIWNE::IconType iconTypeFg = DIWNE::IconType::TriangleRight;

	ImGui::SetCursorPos(position);
	if (m_nodebase->as<Core::Cycle>()->isRunning())
	{
		iconTypeFg = DIWNE::IconType::Pause;
		padding = ImVec4(5 * zoom, 6 * zoom, 5 * zoom, 6 * zoom); // todo I3T::getSize(ESizeVec2::Cycle_ButtonPadding)
	}
	else
	{
		iconTypeFg = DIWNE::IconType::TriangleRight;
		// (left, bottom, right, top)
		padding = ImVec4(9 * zoom, 6 * zoom, 7 * zoom, 6 * zoom); // todo I3T::getSize(ESizeVec2::Cycle_ButtonPadding)
	}
	diwne.DrawIcon(iconTypeBg, iconColorBg, iconColorBg, iconTypeFg, iconColorFg, iconColorFg, iconSize, padding, true,
	               ImVec2(1, 1), rounding);

	if (button_pressed)
	{
		if (m_nodebase->as<Core::Cycle>()->isRunning())
		{
			m_nodebase->as<Core::Cycle>()->pause();
		}
		else
		{
			m_nodebase->as<Core::Cycle>()->play();
		}
		return true;
	}
	return false;


	//-------------------------------------

	// diwne.DrawIcon(iconTypeBg, iconColorBg, iconColorBg, iconTypeFg, iconColorFg, iconColorFg, iconSize,
	//               ImVec4(padding, padding, padding, padding), true);


	// m_iconRectDiwne = ImRect(diwne.screen2diwne(ImGui::GetItemRectMin()),
	// diwne.screen2diwne(ImGui::GetItemRectMax()));


	// if (ImGui::Button(fmt::format("{}", "⏩").c_str()))
	//{
	//	if (m_nodebase->as<Core::Cycle>()->isRunning())
	//	{
	//		m_nodebase->as<Core::Cycle>()->pause();
	//	}
	//	else
	//	{
	//		m_nodebase->as<Core::Cycle>()->play();
	//	}
	//	return true;
	// }
	//  ImGui::PushFont(I3T::getUI()->getTheme().get(EFont::TutorialTitle));
	//// std::u8string playCharacter = u8"⯈";
	//// if (ImGui::SmallButton(reinterpret_cast<const char*>(playCharacter.c_str())))
	//// if (ImGui::SmallButton("\u2BC8"))

	// }
	// ImGui::PopFont();
	// return false;
}

bool WorkspaceCycle::buttonStopAndReset()
{
	// if (ImGui::SmallButton("SaR"))
	// {
	//	m_nodebase->as<Core::Cycle>()->stopAndReset();
	//	return true;
	// }
	// return false;
	//  setup - todo get from themes

	// const ImColor iconColorBg = ImColor(127, 127, 127, 255); // todo I3T::getColor(EColor::Cycle_ButtonBg);
	// const ImColor iconColorFg = IM_COL32_BLACK;              // todo I3T::getColor(EColor::Cycle_ButtonFg);

	// const float zoom = diwne.getWorkAreaZoom();
	// const ImVec2 iconSize = ImVec2(40.0f, 40.0f) * zoom; // todo use themes:
	// I3T::getSize(ESizeVec2::Cycle_ButtonSize)

	// constexpr float rounding = 3.0f; //	ESize::Cycle_ButtonRounding


	ImColor iconColorBg = I3T::getColor(EColor::Cycle_Button);
	const ImColor iconColorFg = I3T::getColor(EColor::Cycle_ButtonForeground);

	const float zoom = diwne.getWorkAreaZoom();
	const ImVec2 iconSize = I3T::getSize(ESizeVec2::Cycle_ButtonSize) * zoom;

	const float rounding = I3T::getSize(ESize::Cycle_ButtonRounding);


	const float p = 6.0f * zoom;
	const ImVec4 padding = ImVec4(p, p, p, p); // // (left, bottom, right, top)

	// Invisible button
	const ImVec2 position = ImGui::GetCursorPos();

	const bool button_pressed = ImGui::InvisibleButton("stop", iconSize);
	// bool button_hovered = ImGui::IsItemHovered();
	// ImGui::Text("Button 1: %s%s", button_hovered ? "hovered " : "", button_pressed ? "PRESSED " : "");
	if (ImGui::IsItemHovered())
		iconColorBg = I3T::getColor(EColor::Cycle_ButtonHovered);
	if (button_pressed)
		iconColorBg = I3T::getColor(EColor::Cycle_ButtonActive);

	const DIWNE::IconType iconTypeBg = DIWNE::IconType::Rectangle;
	DIWNE::IconType iconTypeFg = DIWNE::IconType::Stop;

	ImGui::SetCursorPos(position);

	diwne.DrawIcon(iconTypeBg, iconColorBg, iconColorBg, iconTypeFg, iconColorFg, iconColorFg, iconSize, padding, true,
	               ImVec2(1, 1), rounding);

	if (button_pressed)
	{
		m_nodebase->as<Core::Cycle>()->stopAndReset();
		return true;
	}
	return false;
}

bool WorkspaceCycle::buttonStepBack()
{
	ImColor iconColorBg = I3T::getColor(EColor::Cycle_Button);
	const ImColor iconColorFg = I3T::getColor(EColor::Cycle_ButtonForeground);

	const float zoom = diwne.getWorkAreaZoom();


	ImGuiContext& g = *GImGui;
	float font_size = g.FontSize;

	// const ImVec2 iconSize = I3T::getSize(ESizeVec2::Cycle_ButtonSize) * zoom;
	const ImVec2 iconSize = ImVec2(font_size * 1.1f, font_size);


	const float rounding = I3T::getSize(ESize::Cycle_RadioButtonRounding);


	const float p = 2.5f * zoom;
	const ImVec4 padding = ImVec4(p, p, p, p); // // (left, bottom, right, top)

	// Invisible button
	const ImVec2 position = ImGui::GetCursorPos();

	const bool button_pressed = ImGui::InvisibleButton("stepBack", iconSize);

	if (ImGui::IsItemHovered())
		iconColorBg = I3T::getColor(EColor::Cycle_ButtonHovered);
	if (button_pressed)
		iconColorBg = I3T::getColor(EColor::Cycle_ButtonActive);

	const DIWNE::IconType iconTypeBg = DIWNE::IconType::Rectangle;
	DIWNE::IconType iconTypeFg = DIWNE::IconType::SkipBack;

	ImGui::SetCursorPos(position);

	diwne.DrawIcon(iconTypeBg, iconColorBg, iconColorBg, iconTypeFg, iconColorFg, iconColorFg, iconSize, padding, true,
	               ImVec2(1, 1), rounding);

	if (button_pressed) // if (ImGui::SmallButton("SB"))
	{
		m_nodebase->as<Core::Cycle>()->stepBack();
		return true;
	}
	return false;
}

//// Small buttons fits within text without additional vertical spacing.
// bool ImGui::SmallButton(const char* label)
//{
//	ImGuiContext& g = *GImGui;
//	float backup_padding_y = g.Style.FramePadding.y;
//	g.Style.FramePadding.y = 0.0f;
//	bool pressed = ButtonEx(label, ImVec2(0, 0), ImGuiButtonFlags_AlignTextBaseLine);
//	g.Style.FramePadding.y = backup_padding_y;
//	return pressed;
// }


bool WorkspaceCycle::buttonStepNext()
{
	ImColor iconColorBg = I3T::getColor(EColor::Cycle_Button);
	const ImColor iconColorFg = I3T::getColor(EColor::Cycle_ButtonForeground);

	const float zoom = diwne.getWorkAreaZoom();


	ImGuiContext& g = *GImGui;
	float font_size = g.FontSize;

	// const ImVec2 iconSize = I3T::getSize(ESizeVec2::Cycle_ButtonSize) * zoom;
	const ImVec2 iconSize = ImVec2(font_size * 1.1f, font_size);


	const float rounding = I3T::getSize(ESize::Cycle_RadioButtonRounding);


	const float p = 2.0f * zoom;
	const ImVec4 padding = ImVec4(p, p, p, p); // // (left, bottom, right, top)

	// Invisible button
	const ImVec2 position = ImGui::GetCursorPos();

	const bool button_pressed = ImGui::InvisibleButton("stepForward", iconSize);
	if (ImGui::IsItemHovered())
		iconColorBg = I3T::getColor(EColor::Cycle_ButtonHovered);
	if (button_pressed)
		iconColorBg = I3T::getColor(EColor::Cycle_ButtonActive);

	const DIWNE::IconType iconTypeBg = DIWNE::IconType::Rectangle;
	DIWNE::IconType iconTypeFg = DIWNE::IconType::SkipForward;

	ImGui::SetCursorPos(position);

	diwne.DrawIcon(iconTypeBg, iconColorBg, iconColorBg, iconTypeFg, iconColorFg, iconColorFg, iconSize, padding, true,
	               ImVec2(1, 1), rounding);

	if (button_pressed) // if (ImGui::SmallButton("SN"))
	{
		m_nodebase->as<Core::Cycle>()->stepNext();
		return true;
	}
	return false;
}


bool WorkspaceCycle::buttonRewind()
{
	ImColor iconColorBg = I3T::getColor(EColor::Cycle_Button);
	const ImColor iconColorFg = I3T::getColor(EColor::Cycle_ButtonForeground);

	const float zoom = diwne.getWorkAreaZoom();
	const ImVec2 iconSize = I3T::getSize(ESizeVec2::Cycle_ButtonSize) * zoom;

	const float rounding = I3T::getSize(ESize::Cycle_ButtonRounding);


	const float p = 6.0f * zoom;
	const ImVec4 padding = ImVec4(p, p, p, p); // // (left, bottom, right, top)

	// Invisible button
	const ImVec2 position = ImGui::GetCursorPos();

	const bool button_pressed = ImGui::InvisibleButton("rewind", iconSize);
	if (ImGui::IsItemHovered())
		iconColorBg = I3T::getColor(EColor::Cycle_ButtonHovered);
	if (button_pressed)
		iconColorBg = I3T::getColor(EColor::Cycle_ButtonActive);

	const DIWNE::IconType iconTypeBg = DIWNE::IconType::Rectangle;
	DIWNE::IconType iconTypeFg = DIWNE::IconType::Rewind;

	ImGui::SetCursorPos(position);

	diwne.DrawIcon(iconTypeBg, iconColorBg, iconColorBg, iconTypeFg, iconColorFg, iconColorFg, iconSize, padding, true,
	               ImVec2(1, 1), rounding);

	if (button_pressed) // if (ImGui::SmallButton("RR"))
	{
		m_nodebase->as<Core::Cycle>()->rewind();
		return true;
	}
	return false;
}

bool WorkspaceCycle::buttonWind()
{
	ImColor iconColorBg = I3T::getColor(EColor::Cycle_Button);
	const ImColor iconColorFg = I3T::getColor(EColor::Cycle_ButtonForeground);

	const float zoom = diwne.getWorkAreaZoom();
	const ImVec2 iconSize = I3T::getSize(ESizeVec2::Cycle_ButtonSize) * zoom;

	const float rounding = I3T::getSize(ESize::Cycle_ButtonRounding);


	const float p = 6.0f * zoom;
	const ImVec4 padding = ImVec4(p, p, p, p); // // (left, bottom, right, top)

	// Invisible button
	const ImVec2 position = ImGui::GetCursorPos();

	const bool button_pressed = ImGui::InvisibleButton("Wind", iconSize);
	if (ImGui::IsItemHovered())
		iconColorBg = I3T::getColor(EColor::Cycle_ButtonHovered);
	if (button_pressed)
		iconColorBg = I3T::getColor(EColor::Cycle_ButtonActive);

	const DIWNE::IconType iconTypeBg = DIWNE::IconType::Rectangle;
	DIWNE::IconType iconTypeFg = DIWNE::IconType::FastForward;

	ImGui::SetCursorPos(position);

	diwne.DrawIcon(iconTypeBg, iconColorBg, iconColorBg, iconTypeFg, iconColorFg, iconColorFg, iconSize, padding, true,
	               ImVec2(1, 1), rounding);

	if (button_pressed) // if (ImGui::SmallButton("WW"))
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

	// ImGui::PushStyleVar(ImGuiStyleVar_FramePadding,
	//                     I3T::getSize(ESizeVec2::Nodes_FloatPadding) * diwne.getWorkAreaZoom());
	// ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing,
	//                     I3T::getSize(ESizeVec2::Nodes_ItemsSpacing) * diwne.getWorkAreaZoom());

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

		//-------------------------------------------------------------------
		// PINS From, To, and Step
		//-------------------------------------------------------------------

		//----------------
		// From: -------- | > | from | value |
		//----------------
		ImGui::BeginGroup();
		inner_interaction_happen |= m_workspaceInputs.at(Core::I3T_CYCLE_IN_FROM)->drawDiwne(); // icon and label
		ImGui::EndGroup();
		const float fromWidth = ImGui::GetItemRectSize().x; // length of the icon + label (from is the longest text)
		// ImGui::DebugDrawItemRect(ImColor(255, 127, 0, 127));

		// value
		float localData = m_nodebase->as<Core::Cycle>()->getFrom();
		bool valueChanged = false;

		ImGui::PushStyleVar(ImGuiStyleVar_FramePadding,
		                    I3T::getSize(ESizeVec2::Nodes_FloatPadding) *
		                        diwne.getWorkAreaZoom()); // height of the float widget
		ImGui::SameLine();
		ImGui::Text(""); // probably adds the item separator
		ImGui::SameLine();
		ImGui::SetNextItemWidth(getDataItemsWidth()); // width of the drag float widget
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


		//----------------
		// To: -------- v| > | from | value |
		//----------------
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

		//----------------
		// Step: -------- | > | from | value |
		//----------------
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

		//-------------------------------------------------------------------
		// Pulse PINs
		//-------------------------------------------------------------------
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
	// ImGui::PopStyleVar(1);

	return inner_interaction_happen;
}

bool WorkspaceCycle::rightContent()
{
	// TODO
	// - icons for output pins DONE
	// - no labels for pulse pins DONE
	// - put labels to the hover - postponed - long hover text overlaps the box and distracts

	bool inner_interaction_happen = false;
	inner_interaction_happen |= WorkspaceNodeWithCoreDataWithPins::rightContent();

	return inner_interaction_happen;
}

bool WorkspaceCycle::topContent()
{
	diwne.AddRectFilledDiwne(m_topRectDiwne.Min, m_topRectDiwne.Max, I3T::getTheme().get(EColor::NodeHeader),
	                         I3T::getSize(ESize::Nodes_Rounding), ImDrawFlags_RoundCornersTop);

	return WorkspaceNodeWithCoreData::topContent();
}

bool WorkspaceCycle::myRadioButton(const char* label, int* v, int v_button)
{
	// const float radioBtnWidth = I3T::getUI()->getTheme().get(ESize::StartWindow_LoadButtonWidth);
	// const float buttonHeight = I3T::getUI()->getTheme().get(ESize::StartWindow_ButtonHeight);

	if (*v == v_button) // selected mode (once / repeat / ping-pong)
	{
		// todo - set the colors in the theme file
		// ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4) ImColor::HSV(*v / 7.0f, 0.6f, 0.6f));
		// ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4) ImColor::HSV(*v / 7.0f, 0.7f, 0.7f));
		// ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4) ImColor::HSV(*v / 7.0f, 0.8f, 0.8f));

		ImGui::PushStyleColor(ImGuiCol_Button, I3T::getColor(EColor::Cycle_RadioButtonSelected));
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, I3T::getColor(EColor::Cycle_RadioButtonSelectedHovered));
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, I3T::getColor(EColor::Cycle_ButtonActive));
		ImGui::PushStyleColor(ImGuiCol_Text, I3T::getColor(EColor::Cycle_RadioButtonSelectedText));
	}
	else // not selected
	{
		// ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4) ImColor::HSV(*v / 7.0f, 0.6f / 4.0f, 0.6f));
		// ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4) ImColor::HSV(*v / 7.0f, 0.7f / 4.0f, 0.7f));
		// ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4) ImColor::HSV(*v / 7.0f, 0.8f / 4.0f, 0.8f));

		ImGui::PushStyleColor(ImGuiCol_Button, I3T::getColor(EColor::Cycle_RadioButton));
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, I3T::getColor(EColor::Cycle_RadioButtonHovered));
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, I3T::getColor(EColor::Cycle_RadioButtonActive));
		ImGui::PushStyleColor(ImGuiCol_Text, I3T::getColor(EColor::Cycle_RadioButtonText));
	}

	bool pressed = false;
	// pressed = ImGui::Button(fmt::format("{}##{}",label, v_button).c_str());
	pressed = ImGui::Button(label);
	if (pressed)
	{
		*v = v_button;
		// std::cout << "button " << v_button << ", diwneID " << getId() << ", label >" << label << "<" << std::endl;
	}
	ImGui::PopStyleColor(4);
	return pressed;
}

void DebugDrawItemRect(ImU32 col = IM_COL32(255, 0, 0, 255))
{
	ImGuiContext& g = *GImGui;
	ImGuiWindow* window = g.CurrentWindow;
	ImGui::GetForegroundDrawList(window)->AddRect(g.LastItemData.Rect.Min, g.LastItemData.Rect.Max, col);
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
	case WorkspaceLevelOfDetail::Full:
	case WorkspaceLevelOfDetail::SetValues:
	case WorkspaceLevelOfDetail::LightCycle:
		// button height
		// CalcTextSize("") should return ImVec2(0.0f, g.FontSize)
		// const ImVec2 label_size = ImGui::CalcTextSize("") * 1.2;

		// static float scale = 0.0;
		// ImGui::SetNextItemWidth(30.0f);
		// ImGui::DragFloat("Scale ", &scale, 0.1f, -0.5f, 2.0f);

		ImGui::BeginGroup(); // complete middle part
		{
			ImGui::NewLine();

			// ImGui::Dummy(ImVec2(42.0f, 10.0f));
			// ImGui::SameLine(42.0f);
			ImGui::Indent();
			ImGui::Indent();

			ImVec2 size = ImGui::GetItemRectSize();


			// -----------------------------------------------------
			// LINE SPACING
			// -----------------------------------------------------

			const ImVec2 itemSpacing = ImGui::GetStyle().ItemSpacing;
			ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, (ImVec2(2 * itemSpacing.x, 2.5f * itemSpacing.y)));


			// -----------------------------------------------------
			// line 1: BUTTONS - play, stop, rewind, wind
			// -----------------------------------------------------

			ImGui::TextUnformatted("         ");
			ImGui::SameLine();

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

			ImGui::Dummy(ImVec2(0, 4));


			// -------------------------------------------------------------
			// line 2: MODE select - radiobuttons: once | repeat | ping-pong
			// -------------------------------------------------------------
			int mode = static_cast<int>(m_nodebase->as<Core::Cycle>()->getMode());

			const ImVec2 borderWidth(0.0f, 2.5f);

			static ImRect radio_buttons_rect; // defined in the first frame

			const ImVec2 position = ImGui::GetCursorPos();

			// darker radio buttons background
			if (radio_buttons_rect.GetWidth() > 0.0f) // area behind
			{
				ImGui::SetCursorPos(position - borderWidth);
				diwne.EmptyButton(radio_buttons_rect.GetSize(),
				                  (ImVec4) I3T::getColor(EColor::Cycle_RadioButtonBackground), 5.0f);
				ImGui::SetCursorPos(position + borderWidth);
			}

			ImGui::BeginGroup(); // radio buttons
			{
				ImGui::PushStyleColor(ImGuiCol_Text, I3T::getColor(EColor::Cycle_ButtonForeground));
				ImGui::Text(" ");
				ImGui::SameLine();
				inner_interaction_happen |= myRadioButton("   once   ", &mode, 0);
				ImGui::SameLine();
				ImGui::Text("  |  ");
				ImGui::SameLine();
				inner_interaction_happen |= myRadioButton("   repeat   ", &mode, 1);
				ImGui::SameLine();
				ImGui::Text("  |  ");
				ImGui::SameLine();
				inner_interaction_happen |= myRadioButton("   ping-pong   ", &mode, 2);
				ImGui::SameLine();
				ImGui::Text(" ");
				ImGui::PopStyleColor();
			}
			ImGui::EndGroup();

			// ImGui::DebugDrawItemRect(ImColor(255, 127, 0, 180));

			// radioButtonsSize
			ImGuiContext& g = *GImGui;

			radio_buttons_rect = g.LastItemData.Rect;
			radio_buttons_rect.Expand(borderWidth + borderWidth);

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
			// line 3: smooth step line
			// -----------------------------------------------------

			// smooth checkbox
			ImGui::Text("  smooth step: ");

			//            GetColorU32((held && hovered) ? ImGuiCol_FrameBgActive
			//                        : hovered         ? ImGuiCol_FrameBgHovered
			//                                          : ImGuiCol_FrameBg),
			//            true, style.FrameRounding);
			// ImU32 check_col = GetColorU32(ImGuiCol_CheckMark);
			ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, I3T::getSize(ESize::Cycle_ButtonRounding));
			ImGui::PushStyleColor(ImGuiCol_FrameBg, I3T::getColor(EColor::Cycle_RadioButtonSelected));
			ImGui::PushStyleColor(ImGuiCol_FrameBgActive, I3T::getColor(EColor::Cycle_RadioButtonSelected));
			ImGui::PushStyleColor(ImGuiCol_FrameBgHovered, I3T::getColor(EColor::Cycle_RadioButtonSelected));
			ImGui::PushStyleColor(ImGuiCol_FrameBgActive, I3T::getColor(EColor::Cycle_RadioButtonSelected));
			ImGui::PushStyleColor(ImGuiCol_CheckMark, I3T::getColor(EColor::Cycle_RadioButtonSelectedText));
			ImGui::SameLine();
			auto smooth = m_nodebase->as<Core::Cycle>()->getSmoothStep();
			if (ImGui::Checkbox(fmt::format("##{}smooth", getId()).c_str(), &smooth))
			{
				m_nodebase->as<Core::Cycle>()->setSmoothStep(smooth);
			}
			ImGui::PopStyleColor(5);
			ImGui::PopStyleVar();

			// step duration
			ImGui::SameLine();
			ImGui::Text("     step duration: ");

			ImGui::SameLine();
			localData = m_nodebase->as<Core::Cycle>()->getStepDuration();
			valueChanged = false;
			ImGui::SetNextItemWidth(getDataItemsWidth()); // \todo according to the number of visible decimals
			inner_interaction_happen |= drawDragFloatWithMap_Inline(
			    diwne, getNumberOfVisibleDecimal(), m_floatPopupMode, fmt::format("##{}stepduration", getId()),
			    localData, Core::EValueState::Editable, valueChanged);
			if (ImGui::IsItemHovered())
			{
				ImGui::SetTooltip("Automatic step duration");
			}
			if (valueChanged)
			{
				m_nodebase->as<Core::Cycle>()->setStepDuration(localData);
				updateDataItemsWidth();
			}


			// -----------------------------------------------------
			// line 4: Single step line
			// -----------------------------------------------------
			ImGui::Text("             single step: ");

			ImGui::SameLine();

			localData = m_nodebase->as<Core::Cycle>()->getManualStep();
			valueChanged = false;
			ImGui::SetNextItemWidth(getDataItemsWidth());
			inner_interaction_happen |= drawDragFloatWithMap_Inline(
			    diwne, getNumberOfVisibleDecimal(), m_floatPopupMode, fmt::format("##{}manstep", getId()), localData,
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

			ImGui::SameLine();
			ImGui::Text("   ");

			//---------------------------
			ImGui::SameLine();
			inner_interaction_happen |= buttonStepBack();
			ImGui::SameLine();
			inner_interaction_happen |= buttonStepNext();
			//---------------------------
			ImGui::PopStyleVar(1); // ItemSpacing

			ImGui::NewLine(); // bottom space
		}
		ImGui::EndGroup(); // all middle buttons

		//---------------------------
		ImGui::SameLine();
		ImGui::Text("          "); // right side indentation
		//---------------------------
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
