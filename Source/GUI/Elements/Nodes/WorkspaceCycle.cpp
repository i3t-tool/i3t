/**
 * \authors Sofie Sasorina, Martin Herich
 */

#include "WorkspaceCycle.h"
#include "../Windows/WorkspaceWindow.h"

WorkspaceCycle::WorkspaceCycle(DIWNE::Diwne& diwne, Ptr<Core::NodeBase> nodebase/*=Core::GraphManager::createCycle()*/, bool drawPins/*=true*/)
    :   WorkspaceNodeWithCoreDataWithPins(diwne, nodebase, drawPins)
{
	setDataItemsWidth(); /* \todo Jh make "processinfirstframe" function in Node and run settings data width in it */
}

bool WorkspaceCycle::isCycle() { return true; }

void WorkspaceCycle::drawMenuLevelOfDetail()
{
	drawMenuLevelOfDetail_builder(std::dynamic_pointer_cast<WorkspaceNodeWithCoreData>(shared_from_this()), {WorkspaceLevelOfDetail::Full, WorkspaceLevelOfDetail::LightCycle, WorkspaceLevelOfDetail::Label});
}

bool WorkspaceCycle::buttonPlayPause()
{
    if (ImGui::SmallButton("P/P"))
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

bool WorkspaceCycle::leftContent()
{
    bool inner_interaction_happen = false;

    if (m_levelOfDetail ==  WorkspaceLevelOfDetail::LightCycle)
    {
        for (auto const i : {Core::I3T_CYCLE_IN_FROM, Core::I3T_CYCLE_IN_TO, Core::I3T_CYCLE_IN_MULT})
        {
            m_workspaceInputs.at(i)->drawDiwne();
        }

        WorkspaceDiwne& wd = static_cast<WorkspaceDiwne&>(diwne);
        ImRect nodeRect = getNodeRectDiwne();
        ImVec2 pinConnectionPoint = ImVec2(nodeRect.Min.x, (diwne.screen2diwne(ImGui::GetCursorScreenPos()).y + nodeRect.Max.y)/2);

        for (auto const i : {Core::I3T_CYCLE_IN_PLAY, Core::I3T_CYCLE_IN_PAUSE, Core::I3T_CYCLE_IN_STOP, Core::I3T_CYCLE_IN_PREV, Core::I3T_CYCLE_IN_NEXT})
        {
            Ptr<WorkspaceCoreInputPin> const pin = m_workspaceInputs.at(i);
            pin->setConnectionPointDiwne(pinConnectionPoint);
            if (pin->isConnected()){wd.m_linksToDraw.push_back(&pin->getLink());}
        }
    }
    else
    {
       inner_interaction_happen |= WorkspaceNodeWithCoreDataWithPins::leftContent();
    }

    return inner_interaction_happen;
}

bool WorkspaceCycle::rightContent()
{
    bool inner_interaction_happen = false;
    if (m_levelOfDetail ==  WorkspaceLevelOfDetail::LightCycle)
    {
        for (auto const i : {Core::I3T_CYCLE_OUT_VAL})
        {
            m_workspaceOutputs.at(i)->drawDiwne();
        }

        WorkspaceDiwne& wd = static_cast<WorkspaceDiwne&>(diwne);
        ImRect nodeRect = getNodeRectDiwne();
        ImVec2 pinConnectionPoint = ImVec2(nodeRect.Min.x, (diwne.screen2diwne(ImGui::GetCursorScreenPos()).y + nodeRect.Max.y)/2);

        for (auto const i : {Core::I3T_CYCLE_OUT_PLAY, Core::I3T_CYCLE_OUT_PAUSE, Core::I3T_CYCLE_OUT_STOP, Core::I3T_CYCLE_OUT_PREV, Core::I3T_CYCLE_OUT_NEXT, Core::I3T_CYCLE_OUT_END})
        {
            m_workspaceOutputs.at(i)->setConnectionPointDiwne(pinConnectionPoint);
        }
    }
    else
    {
        inner_interaction_happen |= WorkspaceNodeWithCoreDataWithPins::rightContent();
    }

    return inner_interaction_happen;
}

bool WorkspaceCycle::middleContent()
{
    // \todo Add icons to buttons
    // "⯈/❙❙" "◼" "❙⯇" "⯈❙"
    //std::u8string string = u8"⯈";
    //std::u8string string = u8"ěščřžýáíé";
    //std::string s(string.cbegin(), string.cend());


    bool inner_interaction_happen = false;
    //ImVec2 button_sz = I3T::getSize(ESizeVec2::Nodes_FloatCycleButtonSize)*diwne.getWorkAreaZoom();
    float localData;
    bool valueChanged;

    ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, I3T::getSize(ESizeVec2::Nodes_FloatPadding));
    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, I3T::getSize(ESizeVec2::Nodes_ItemsSpacing));

    switch (m_levelOfDetail)
    {
    case WorkspaceLevelOfDetail::LightCycle:

        inner_interaction_happen |= buttonPlayPause(); ImGui::SameLine();
        inner_interaction_happen |= buttonStopAndReset();

        valueChanged = false;
        localData = m_nodebase->as<Core::Cycle>()->getData().getFloat();
        ImGui::SetNextItemWidth(getDataItemsWidth());
        inner_interaction_happen |= drawDragFloatWithMap_Inline(diwne, getNumberOfVisibleDecimal(), m_floatPopupMode, fmt::format("##{}Value", getId()),
                                    localData, /* m_nodebase->as<Core::Cycle>()->getValueState({0, 0}) - not work */ Core::EValueState::Editable, valueChanged); /* \todo MH PF Always editable?*/
        if (ImGui::IsItemHovered()){ImGui::SetTooltip("Value");}
        if (valueChanged)
        {
            //ask MH \\ \todo SetValue does not call setDirectionMultiplier()
            m_nodebase->setValue(localData);
            setDataItemsWidth();
        }

        break;
    case WorkspaceLevelOfDetail::Full:

        inner_interaction_happen |= buttonPlayPause(); ImGui::SameLine();
		inner_interaction_happen |= buttonStopAndReset(); ImGui::SameLine();
		inner_interaction_happen |= buttonStepBack(); ImGui::SameLine();
		inner_interaction_happen |= buttonStepNext();

		valueChanged = false;
        localData = m_nodebase->as<Core::Cycle>()->getData().getFloat();
        ImGui::SetNextItemWidth(getDataItemsWidth());
        inner_interaction_happen |= drawDragFloatWithMap_Inline(diwne, getNumberOfVisibleDecimal(), m_floatPopupMode, fmt::format("##{}Value", getId()),
                                    localData, /* m_nodebase->as<Core::Cycle>()->getValueState({0, 0}) - not work */ Core::EValueState::Editable, valueChanged); /* \todo MH PF Always editable?*/
        if (ImGui::IsItemHovered()){ImGui::SetTooltip("Value");}
        if (valueChanged)
        {
            //ask MH \\ \todo SetValue does not call setDirectionMultiplier()
            m_nodebase->setValue(localData);
            setDataItemsWidth();
        }

		/* =================================== */
		// Mode select
		int mode = static_cast<int>(m_nodebase->as<Core::Cycle>()->getMode());

		//TODO is there a way to calculate how much space take radio button with text?

		ImGui::BeginTable(fmt::format("{}mode",getId()).c_str(), 4, ImGuiTableFlags_NoHostExtendX | ImGuiTableFlags_SizingFixedFit );
            ImGui::TableNextRow();
//                ImGui::TableNextColumn();
//                    ImGui::TextUnformatted("One");
//                ImGui::TableNextColumn();
//                    ImGui::TextUnformatted("Rep");
//                ImGui::TableNextColumn();
//                    ImGui::TextUnformatted("Ping");
//  //              ImGui::TableNextColumn();
//  //                  ImGui::TextUnformatted("Val");
            ImGui::TableNextRow();
                ImGui::TableNextColumn();
                    ImGui::RadioButton("##Once", &mode, 0); if (ImGui::IsItemHovered()){ImGui::SetTooltip("Once");}
                ImGui::TableNextColumn();
                    ImGui::RadioButton("##Repeat", &mode, 1); if (ImGui::IsItemHovered()){ImGui::SetTooltip("Repeat");}
                ImGui::TableNextColumn();
                    ImGui::RadioButton("##Ping", &mode, 2); if (ImGui::IsItemHovered()){ImGui::SetTooltip("Ping-pong");}
        ImGui::EndTable();
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

		ImGui::BeginTable(fmt::format("{}controlValues",getId()).c_str(), 2, ImGuiTableFlags_NoHostExtendX | ImGuiTableFlags_SizingFixedFit );
//            ImGui::TableNextRow();
//                ImGui::TableNextColumn();
//                    ImGui::TextUnformatted("From");
//                ImGui::TableNextColumn();
//                    ImGui::TextUnformatted("To");
//                ImGui::TableNextColumn();
//                    ImGui::TextUnformatted("Step");
//                ImGui::TableNextColumn();
//                    ImGui::TextUnformatted("Man.");
            ImGui::TableNextRow();
                ImGui::TableNextColumn();
                    localData = m_nodebase->as<Core::Cycle>()->getFrom();
                    valueChanged = false;
                    ImGui::SetNextItemWidth(getDataItemsWidth());
                    inner_interaction_happen |= drawDragFloatWithMap_Inline(diwne, getNumberOfVisibleDecimal(), m_floatPopupMode, fmt::format("##{}from", getId()),
                                                localData, m_workspaceInputs.at(Core::I3T_CYCLE_IN_FROM)->isConnected() ? Core::EValueState::Locked : Core::EValueState::Editable , valueChanged);
                    if (ImGui::IsItemHovered()){ImGui::SetTooltip("From");}
                    if (valueChanged)
                    {
                        m_nodebase->as<Core::Cycle>()->setFrom(localData);
                        setDataItemsWidth();
                    }
                ImGui::TableNextColumn();
                    localData   = m_nodebase->as<Core::Cycle>()->getTo();
                    valueChanged = false;
                    ImGui::SetNextItemWidth(getDataItemsWidth());
                    inner_interaction_happen |= drawDragFloatWithMap_Inline(diwne, getNumberOfVisibleDecimal(), m_floatPopupMode, fmt::format("##{}to", getId()),
                                                localData, m_workspaceInputs.at(Core::I3T_CYCLE_IN_TO)->isConnected() ? Core::EValueState::Locked : Core::EValueState::Editable , valueChanged);
                    if (ImGui::IsItemHovered()){ImGui::SetTooltip("To");}
                    if (valueChanged)
                    {
                        m_nodebase->as<Core::Cycle>()->setTo(localData);
                        setDataItemsWidth();
                    }
            ImGui::TableNextRow();
                ImGui::TableNextColumn();
                    localData   = m_nodebase->as<Core::Cycle>()->getMultiplier();
                    valueChanged = false;
                    ImGui::SetNextItemWidth(getDataItemsWidth());
                    inner_interaction_happen |= drawDragFloatWithMap_Inline(diwne, getNumberOfVisibleDecimal(), m_floatPopupMode, fmt::format("##{}step", getId()),
                                                localData, m_workspaceInputs.at(Core::I3T_CYCLE_IN_MULT)->isConnected() ? Core::EValueState::Locked : Core::EValueState::Editable , valueChanged);
                    if (ImGui::IsItemHovered()){ImGui::SetTooltip("Step");}
                    if (valueChanged)
                    {
                        m_nodebase->as<Core::Cycle>()->setMultiplier(localData);
                        setDataItemsWidth();
                    }
                ImGui::TableNextColumn();
                    localData   = m_nodebase->as<Core::Cycle>()->getManualStep();
                    valueChanged = false;
                    ImGui::SetNextItemWidth(getDataItemsWidth());
                    inner_interaction_happen |= drawDragFloatWithMap_Inline(diwne, getNumberOfVisibleDecimal(), m_floatPopupMode, fmt::format("##{}manstep", getId()),
                                                localData, Core::EValueState::Editable, valueChanged);
                    if (ImGui::IsItemHovered()){ImGui::SetTooltip("Step for manual Next/Prev");}
                    if (valueChanged)
                    {
                        m_nodebase->as<Core::Cycle>()->setManualStep(localData);
                        setDataItemsWidth();
                    }
        ImGui::EndTable();
        break;
    }

    ImGui::PopStyleVar(2);
    return inner_interaction_happen;
}


int WorkspaceCycle::maxLenghtOfData()
{
    Ptr<Core::Cycle> nodebase = m_nodebase->as<Core::Cycle>();
	return std::max({numberOfCharWithDecimalPoint(nodebase->getFrom(), m_numberOfVisibleDecimal)
                    , numberOfCharWithDecimalPoint(nodebase->getTo(), m_numberOfVisibleDecimal)
                    , numberOfCharWithDecimalPoint(nodebase->getManualStep(), m_numberOfVisibleDecimal)
                    , numberOfCharWithDecimalPoint(nodebase->getMultiplier(), m_numberOfVisibleDecimal)});
}