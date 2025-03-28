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
#include "Cycle.h"
#include "GUI/Workspace/Tools.h"
#include "GUI/Workspace/WorkspaceDiwne.h"

using namespace Workspace;

Cycle::Cycle(DIWNE::NodeEditor& diwne, Ptr<Core::Node> nodebase /*=Core::GraphManager::createCycle()*/,
             bool showDataOnPins /*=true*/)
    : CoreNodeWithPins(diwne, nodebase, showDataOnPins)
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

bool Cycle::isCycle()
{
	return true;
}

void Cycle::drawMenuLevelOfDetail()
{
	drawMenuLevelOfDetail_builder(
	    std::dynamic_pointer_cast<CoreNode>(shared_from_this()),
	    {LevelOfDetail::Full, LevelOfDetail::SetValues, LevelOfDetail::LightCycle, LevelOfDetail::Label});
}

bool Cycle::buttonPlayPause()
{
	DIWNE::IconType iconTypeFg;
	if (m_nodebase->as<Core::Cycle>()->isRunning())
		iconTypeFg = DIWNE::IconType::Pause;
	else
		iconTypeFg = DIWNE::IconType::TriangleRight;

	bool pressed = bigButton("playPause", DIWNE::IconType::Rectangle, iconTypeFg);

	if (pressed)
	{
		if (m_nodebase->as<Core::Cycle>()->isRunning())
			m_nodebase->as<Core::Cycle>()->pause();
		else
			m_nodebase->as<Core::Cycle>()->play();
	}
	return pressed || ImGui::IsItemActive();
}

bool Cycle::buttonStopAndReset()
{
	bool pressed = bigButton("Stop", DIWNE::IconType::Rectangle, DIWNE::IconType::Stop);
	if (pressed)
		m_nodebase->as<Core::Cycle>()->stopAndReset();
	return pressed || ImGui::IsItemActive();
}

bool Cycle::buttonRewind()
{
	bool pressed = bigButton("Rewind", DIWNE::IconType::Rectangle, DIWNE::IconType::Rewind);
	if (pressed)
		m_nodebase->as<Core::Cycle>()->rewind();
	return pressed || ImGui::IsItemActive();
}

bool Cycle::buttonWind()
{
	bool pressed = bigButton("Wind", DIWNE::IconType::Rectangle, DIWNE::IconType::FastForward);
	if (pressed)
		m_nodebase->as<Core::Cycle>()->wind();
	return pressed || ImGui::IsItemActive();
}

bool Cycle::bigButton(const std::string& str, DIWNE::IconType iconTypeBg, DIWNE::IconType iconTypeFg)
{
	// Colors
	const ImColor iconColorBg = I3T::getColor(EColor::Cycle_Button);
	const ImColor iconColorBgHovered = I3T::getColor(EColor::Cycle_ButtonHovered);
	const ImColor iconColorBgActive = I3T::getColor(EColor::Cycle_ButtonActive);
	const ImColor iconColorFg = I3T::getColor(EColor::Cycle_ButtonForeground);
	// Sizes
	const float zoom = diwne.getZoom();
	const ImVec2 iconSize = I3T::getSize(ESizeVec2::Cycle_ButtonSize) * zoom;
	const float rounding = I3T::getSize(ESize::Cycle_ButtonRounding) * zoom;
	const float p = diwne.canvas().diwne2screenSize(6.0f);
	const ImVec4 padding = ImVec4(p, p, p, p); // // (left, bottom, right, top)

	// Pack into styles
	const DIWNE::IconStyle style(iconColorBg, iconColorBg, iconColorFg, iconColorFg, padding, true, ImVec2(1, 1),
	                             rounding);
	const DIWNE::IconStyle styleHovered(iconColorBgHovered, iconColorBgHovered, iconColorFg, iconColorFg, padding, true,
	                                    ImVec2(1, 1), rounding);
	const DIWNE::IconStyle styleActive(iconColorBgActive, iconColorBgActive, iconColorFg, iconColorFg, padding, true,
	                                   ImVec2(1, 1), rounding);

	return diwne.canvas().IconButton2(str.c_str(), iconSize, false, iconTypeBg, iconTypeFg, style, styleHovered,
	                                  styleActive);
}

// TODO: These two button methods below are still an absolute code duplication mess, we need to use general UI methods.
bool Cycle::buttonStepNext()
{
	ImColor iconColorBg = I3T::getColor(EColor::Cycle_Button);
	const ImColor iconColorFg = I3T::getColor(EColor::Cycle_ButtonForeground);

	const float zoom = diwne.getZoom();

	ImGuiContext& g = *GImGui;
	float font_size = g.FontSize;

	// const ImVec2 iconSize = I3T::getSize(ESizeVec2::Cycle_ButtonSize) * zoom;
	const ImVec2 iconSize = ImVec2(font_size * 1.1f, font_size);

	const float rounding = I3T::getSize(ESize::Cycle_RadioButtonRounding) * zoom;

	const float p = diwne.canvas().diwne2screenSize(2.0f);
	const ImVec4 padding = ImVec4(p, p, p, p); // // (left, bottom, right, top)

	// Invisible button
	const ImVec2 position = ImGui::GetCursorPos();

	bool active = false;
	const bool button_pressed = ImGui::InvisibleButton("stepForward", iconSize);
	if (ImGui::IsItemHovered())
		iconColorBg = I3T::getColor(EColor::Cycle_ButtonHovered);
	if (button_pressed || ImGui::IsItemActive())
	{
		active = true;
		iconColorBg = I3T::getColor(EColor::Cycle_ButtonActive);
	}

	const DIWNE::IconType iconTypeBg = DIWNE::IconType::Rectangle;
	DIWNE::IconType iconTypeFg = DIWNE::IconType::SkipForward;

	ImGui::SetCursorPos(position);
	diwne.canvas().DrawIcon(iconTypeBg, iconColorBg, iconColorBg, iconTypeFg, iconColorFg, iconColorFg, iconSize,
	                        padding, true, ImVec2(1, 1), rounding);
	ImGui::Dummy(iconSize);

	if (button_pressed) // if (ImGui::SmallButton("SN"))
	{
		m_nodebase->as<Core::Cycle>()->stepNext();
		return true;
	}
	return active;
}

bool Cycle::buttonStepBack()
{
	ImColor iconColorBg = I3T::getColor(EColor::Cycle_Button);
	const ImColor iconColorFg = I3T::getColor(EColor::Cycle_ButtonForeground);

	const float zoom = diwne.getZoom();

	ImGuiContext& g = *GImGui;
	float font_size = g.FontSize;

	// const ImVec2 iconSize = I3T::getSize(ESizeVec2::Cycle_ButtonSize) * zoom;
	const ImVec2 iconSize = ImVec2(font_size * 1.1f, font_size);

	const float rounding = I3T::getSize(ESize::Cycle_RadioButtonRounding) * zoom;

	const float p = diwne.canvas().diwne2screenSize(2.5f);
	const ImVec4 padding = ImVec4(p, p, p, p); // // (left, bottom, right, top)

	// Invisible button
	const ImVec2 position = ImGui::GetCursorPos();

	bool active = false;
	const bool button_pressed = ImGui::InvisibleButton("stepBack", iconSize);
	if (ImGui::IsItemActive())
		if (ImGui::IsItemHovered())
			iconColorBg = I3T::getColor(EColor::Cycle_ButtonHovered);
	if (button_pressed || ImGui::IsItemActive())
	{
		active = true;
		iconColorBg = I3T::getColor(EColor::Cycle_ButtonActive);
	}
	const DIWNE::IconType iconTypeBg = DIWNE::IconType::Rectangle;
	DIWNE::IconType iconTypeFg = DIWNE::IconType::SkipBack;

	ImGui::SetCursorPos(position);
	diwne.canvas().DrawIcon(iconTypeBg, iconColorBg, iconColorBg, iconTypeFg, iconColorFg, iconColorFg, iconSize,
	                        padding, true, ImVec2(1, 1), rounding);
	ImGui::Dummy(iconSize);

	if (button_pressed) // if (ImGui::SmallButton("SB"))
	{
		m_nodebase->as<Core::Cycle>()->stepBack();
		return true;
	}
	return active;
}

void Cycle::leftContent(DIWNE::DrawInfo& context)
{
	// TODO: The method seems very repetitive, investigate
	// TODO: Review what interactions are input

	// TODO
	// - icons for input pins
	// - no labels for pulse pins - put them to the hover

	bool inner_interaction_happen = false;

	// ImGui::PushStyleVar(ImGuiStyleVar_FramePadding,
	//                     I3T::getSize(ESizeVec2::Nodes_FloatPadding) * diwne.getZoom());
	// ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing,
	//                     I3T::getSize(ESizeVec2::Nodes_ItemsSpacing) * diwne.getZoom());

	if (m_levelOfDetail == LevelOfDetail::LightCycle)
	{
		for (auto const i : {Core::I3T_CYCLE_IN_FROM, Core::I3T_CYCLE_IN_TO, Core::I3T_CYCLE_IN_STEP})
		{
			m_workspaceInputs.at(i)->drawDiwne(context, m_drawMode);
		}

		WorkspaceDiwne& wd = static_cast<WorkspaceDiwne&>(diwne);
		ImRect nodeRect = getRect();
		ImVec2 pinConnectionPoint =
		    ImVec2(nodeRect.Min.x, (diwne.canvas().screen2diwne(ImGui::GetCursorScreenPos()).y + nodeRect.Max.y) / 2);

		for (const auto& pin : m_workspaceInputs)
		{
			pin->setRendered(false);
		}
		for (auto const i : {Core::I3T_CYCLE_IN_PLAY, Core::I3T_CYCLE_IN_PAUSE, Core::I3T_CYCLE_IN_STOP,
		                     Core::I3T_CYCLE_IN_PREV, Core::I3T_CYCLE_IN_NEXT})
		{
			Ptr<CorePin> const pin = m_workspaceInputs.at(i);
			pin->setConnectionPointDiwne(pinConnectionPoint);
			pin->setRendered(true);
		}
	}
	else if (m_levelOfDetail == LevelOfDetail::Label)
	{
		// inner_interaction_happen |= WorkspaceNode::leftContent();
		CoreNodeWithPins::leftContent(context);
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
		m_workspaceInputs.at(Core::I3T_CYCLE_IN_FROM)->drawDiwne(context, m_drawMode); // icon and label
		ImGui::EndGroup();
		const float fromWidth = ImGui::GetItemRectSize().x; // length of the icon + label (from is the longest text)
		// ImGui::DebugDrawItemRect(ImColor(255, 127, 0, 127));

		// value
		float localData = m_nodebase->as<Core::Cycle>()->getFrom();
		bool valueChanged = false;

		ImGui::PushStyleVar(ImGuiStyleVar_FramePadding,
		                    I3T::getSize(ESizeVec2::Nodes_FloatPadding) *
		                        diwne.getZoom()); // height of the float widget
		ImGui::SameLine();
		ImGui::Text(""); // probably adds the item separator
		ImGui::SameLine();
		ImGui::SetNextItemWidth(getDataItemsWidth()); // width of the drag float widget
		inner_interaction_happen |= DataRenderer::drawDragFloatWithMap_Inline(
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

			context.update(true, true, true);
		}

		//----------------
		// To: -------- v| > | from | value |
		//----------------
		ImGui::BeginGroup();
		m_workspaceInputs.at(Core::I3T_CYCLE_IN_TO)->drawDiwne(context, m_drawMode); // icon and label
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
		ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, I3T::getSize(ESizeVec2::Nodes_FloatPadding) * diwne.getZoom());
		inner_interaction_happen |= DataRenderer::drawDragFloatWithMap_Inline(
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
		m_workspaceInputs.at(Core::I3T_CYCLE_IN_STEP)->drawDiwne(context, m_drawMode); // icon and label
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
		ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, I3T::getSize(ESizeVec2::Nodes_FloatPadding) * diwne.getZoom());
		// TODO: drawDragFloat should take the context parameter
		bool interaction = DataRenderer::drawDragFloatWithMap_Inline(
		    diwne, getNumberOfVisibleDecimal(), m_floatPopupMode, fmt::format("##{}step", getId()), localData,
		    m_workspaceInputs.at(Core::I3T_CYCLE_IN_STEP)->isConnected() ? Core::EValueState::Locked
		                                                                 : Core::EValueState::Editable,
		    valueChanged);
		if (interaction)
			context.update(true, true, true);

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
		if (m_levelOfDetail == LevelOfDetail::Full)
		{
			// the remaining pins
			for (auto const i : {Core::I3T_CYCLE_IN_PLAY, Core::I3T_CYCLE_IN_PAUSE, Core::I3T_CYCLE_IN_STOP,
			                     Core::I3T_CYCLE_IN_PREV, Core::I3T_CYCLE_IN_NEXT})
			{
				// inner_interaction_happen |= m_workspaceInputs.at(i)->m_iconType
				m_workspaceInputs.at(i)->drawDiwne(context, m_drawMode);
			}
		}
		else if (m_levelOfDetail == LevelOfDetail::SetValues)
		{
			// the remaining pins are hidden, draw the wires only
			WorkspaceDiwne& wd = static_cast<WorkspaceDiwne&>(diwne);
			const ImRect nodeRect = getRect();
			const ImVec2 pinConnectionPoint =
			    ImVec2(nodeRect.Min.x, (nodeRect.Min.y + nodeRect.Max.y) / 2.0f + 30.0f); // (PF) The Piscvejc constant
			for (auto const i : {Core::I3T_CYCLE_IN_PLAY, Core::I3T_CYCLE_IN_PAUSE, Core::I3T_CYCLE_IN_STOP,
			                     Core::I3T_CYCLE_IN_PREV, Core::I3T_CYCLE_IN_NEXT})
			{
				auto const& pin = m_workspaceInputs.at(i);
				// if (!pin->getCorePin().isRendered()) // todo (PF) Label did not draw the wires!
				{
					pin->setConnectionPointDiwne(pinConnectionPoint);
				}
			}
		}
	}
	// ImGui::PopStyleVar(1);
}

void Cycle::rightContent(DIWNE::DrawInfo& context)
{
	// TODO
	// - icons for output pins DONE
	// - no labels for pulse pins DONE
	// - put labels to the hover - postponed - long hover text overlaps the box and distracts

	bool inner_interaction_happen = false;
	CoreNodeWithPins::rightContent(context);
}

bool Cycle::myRadioButton(const char* label, int* v, int v_button)
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
	return pressed || ImGui::IsItemActive();
}

void DebugDrawItemRect(ImU32 col = IM_COL32(255, 0, 0, 255))
{
	ImGuiContext& g = *GImGui;
	ImGuiWindow* window = g.CurrentWindow;
	ImGui::GetForegroundDrawList(window)->AddRect(g.LastItemData.Rect.Min, g.LastItemData.Rect.Max, col);
}

void Cycle::centerContent(DIWNE::DrawInfo& context)
{
	// \todo Add icons to buttons
	// std::u8string string = u8"ěščřžýáíé";
	// std::string s(string.cbegin(), string.cend());

	// fmt::print("{}\n", "ф");
	// std::cout << fmt::format("{}\n", "⏩") << std::endl;

	bool inner_interaction_happen = false;

	// ImVec2 button_sz =
	// I3T::getSize(ESizeVec2::Nodes_FloatCycleButtonSize)*diwne.getZoom();
	float localData;
	bool valueChanged;

	ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, I3T::getSize(ESizeVec2::Nodes_FloatPadding) * diwne.getZoom());
	ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, I3T::getSize(ESizeVec2::Nodes_ItemsSpacing) * diwne.getZoom());

	switch (m_levelOfDetail)
	{
	case LevelOfDetail::Label:
		break;
	case LevelOfDetail::Full:
	case LevelOfDetail::SetValues:
	case LevelOfDetail::LightCycle:
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
			                    I3T::getUI()->getTheme().get(ESize::StartWindow_FrameRounding) * diwne.getZoom());

			inner_interaction_happen |= buttonPlayPause();

			ImGui::SameLine();
			inner_interaction_happen |= buttonStopAndReset();

			if (m_levelOfDetail == LevelOfDetail::LightCycle) // draw play and stop buttons only
			{
				ImGui::PopStyleVar(2); // ImGuiStyleVar_ItemSpacing, ImGuiStyleVar_FrameRounding
				break;
			}

			ImGui::SameLine();
			inner_interaction_happen |= buttonRewind();
			ImGui::SameLine();
			inner_interaction_happen |= buttonWind();

			ImGui::Dummy(ImVec2(0, diwne.canvas().diwne2screenSize(4.0f)));

			// -------------------------------------------------------------
			// line 2: MODE select - radiobuttons: once | repeat | ping-pong
			// -------------------------------------------------------------
			int mode = static_cast<int>(m_nodebase->as<Core::Cycle>()->getMode());

			const ImVec2 borderSize(diwne.canvas().diwne2screenSize(2.5f), diwne.canvas().diwne2screenSize(2.5f));
			static ImVec2 radioButtonBgSize; // defined in the first frame

			const ImVec2 position = ImGui::GetCursorPos();

			// darker radio buttons background
			if (radioButtonBgSize.x > 0.0f) // area behind
			{
				ImGui::SetCursorPos(position - borderSize);
				diwne.canvas().EmptyButton(diwne.canvas().diwne2screenSize(radioButtonBgSize),
				                           (ImVec4) I3T::getColor(EColor::Cycle_RadioButtonBackground),
				                           diwne.canvas().diwne2screenSize(5.0f));
				ImGui::SetCursorPos(position + borderSize);
			}

			ImGui::BeginGroup(); // radio buttons
			{
				ImGui::PushStyleColor(ImGuiCol_Text, I3T::getColor(EColor::Cycle_ButtonForeground));
				ImGui::Text(" ");
				ImGui::SameLine();
				inner_interaction_happen |= myRadioButton(_t("   once   "), &mode, 0);
				ImGui::SameLine();
				ImGui::Text("  |  ");
				ImGui::SameLine();
				inner_interaction_happen |= myRadioButton(_t("   repeat   "), &mode, 1);
				ImGui::SameLine();
				ImGui::Text("  |  ");
				ImGui::SameLine();
				inner_interaction_happen |= myRadioButton(_t("   ping-pong   "), &mode, 2);
				ImGui::SameLine();
				ImGui::Text(" ");
				ImGui::PopStyleColor();
			}
			ImGui::EndGroup();
			// ImGui::DebugDrawItemRect(ImColor(255, 127, 0, 180));

			radioButtonBgSize = diwne.canvas().screen2diwneSize(
			    ImGui::GetCurrentContext()->LastItemData.Rect.GetSize() + borderSize * 4);

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
			ImGui::Text(_t("  smooth step: "));

			//            GetColorU32((held && hovered) ? ImGuiCol_FrameBgActive
			//                        : hovered         ? ImGuiCol_FrameBgHovered
			//                                          : ImGuiCol_FrameBg),
			//            true, style.FrameRounding);
			// ImU32 check_col = GetColorU32(ImGuiCol_CheckMark);
			ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding,
			                    I3T::getSize(ESize::Cycle_ButtonRounding) * diwne.getZoom());
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
			if (ImGui::IsItemActive())
				context.consumeInput();
			ImGui::PopStyleColor(5);
			ImGui::PopStyleVar();

			// step duration
			ImGui::SameLine();
			ImGui::Text(_t("     step duration: "));

			ImGui::SameLine();
			localData = m_nodebase->as<Core::Cycle>()->getStepDuration();
			valueChanged = false;
			ImGui::SetNextItemWidth(getDataItemsWidth()); // \todo according to the number of visible decimals
			inner_interaction_happen |= DataRenderer::drawDragFloatWithMap_Inline(
			    diwne, getNumberOfVisibleDecimal(), m_floatPopupMode, fmt::format("##{}stepduration", getId()),
			    localData, Core::EValueState::Editable, valueChanged);
			if (ImGui::IsItemHovered())
			{
				ImGui::SetTooltip(_t("Automatic step duration"));
			}
			if (valueChanged)
			{
				m_nodebase->as<Core::Cycle>()->setStepDuration(localData);
				updateDataItemsWidth();
			}


			// -----------------------------------------------------
			// line 4: Single step line
			// -----------------------------------------------------
			ImGui::Text(_t("             single step: "));

			ImGui::SameLine();

			localData = m_nodebase->as<Core::Cycle>()->getManualStep();
			valueChanged = false;
			ImGui::SetNextItemWidth(getDataItemsWidth());
			inner_interaction_happen |= DataRenderer::drawDragFloatWithMap_Inline(
			    diwne, getNumberOfVisibleDecimal(), m_floatPopupMode, fmt::format("##{}manstep", getId()), localData,
			    Core::EValueState::Editable, valueChanged);
			if (ImGui::IsItemHovered())
			{
				ImGui::SetTooltip(_t("Step for manual Next/Prev"));
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

	if (inner_interaction_happen)
		context.consumeInput();
}

int Cycle::maxLengthOfData()
{
	Ptr<Core::Cycle> nodebase = m_nodebase->as<Core::Cycle>();
	return std::max({Tools::numberOfCharWithDecimalPoint(nodebase->getFrom(), m_numberOfVisibleDecimal),
	                 Tools::numberOfCharWithDecimalPoint(nodebase->getTo(), m_numberOfVisibleDecimal),
	                 Tools::numberOfCharWithDecimalPoint(nodebase->getManualStep(), m_numberOfVisibleDecimal),
	                 Tools::numberOfCharWithDecimalPoint(nodebase->getStep(), m_numberOfVisibleDecimal)});
}
