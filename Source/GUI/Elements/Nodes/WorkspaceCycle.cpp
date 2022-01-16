/**
 * \authors Sofie Sasorina, Martin Herich
 */

#include "WorkspaceCycle.h"

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

bool WorkspaceCycle::buttonPlayPause(ImVec2 const &  button_sz)
{
    if (ImGui::Button("P/P", button_sz))
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

bool WorkspaceCycle::buttonStopAndReset(ImVec2 const &  button_sz)
{
    if (ImGui::Button("SaR", button_sz))
    {
        m_nodebase->as<Core::Cycle>()->stopAndReset();
        return true;
    }
    return false;
}

bool WorkspaceCycle::buttonStepBack(ImVec2 const &  button_sz)
{
    if (ImGui::Button("SB", button_sz))
    {
        m_nodebase->as<Core::Cycle>()->stepBack();
        return true;
    }
    return false;
}

bool WorkspaceCycle::buttonStepNext(ImVec2 const &  button_sz)
{
    if (ImGui::Button("SN", button_sz))
    {
        m_nodebase->as<Core::Cycle>()->stepNext();
        return true;
    }
    return false;
}

bool WorkspaceCycle::leftContent()
{
    bool inner_interaction_happen = false;

    ImRect nodeRect; /* just declaration before switch */
    ImVec2 pinConnectionPoint;

    switch (m_levelOfDetail)
    {
    case WorkspaceLevelOfDetail::Label:
        nodeRect = getNodeRectDiwne();
        pinConnectionPoint = ImVec2(nodeRect.Min.x, (nodeRect.Min.y + nodeRect.Max.y)/2);
        for (auto const& pin : m_workspaceInputs) {
            pin->setConnectionPointDiwne(pinConnectionPoint);
            if (pin->isConnected())
            {
                inner_interaction_happen |= pin->getLink().drawDiwne();
            }
        }

        break;
    case WorkspaceLevelOfDetail::LightCycle:


        for (auto const i : {Core::I3T_CYCLE_IN_FROM, Core::I3T_CYCLE_IN_TO, Core::I3T_CYCLE_IN_MULT})
        {
            m_workspaceInputs.at(i)->drawDiwne();
        }

        for (auto const i : {Core::I3T_CYCLE_IN_PLAY, Core::I3T_CYCLE_IN_PAUSE, Core::I3T_CYCLE_IN_STOP, Core::I3T_CYCLE_IN_PREV, Core::I3T_CYCLE_IN_NEXT})
        {
            Ptr<WorkspaceCoreInputPin> const pin = m_workspaceInputs.at(i);
            pin->setConnectionPointDiwne(pinConnectionPoint);
            if (pin->isConnected())
            {
                inner_interaction_happen |= pin->getLink().drawDiwne();
            }
        }

        break;
    case WorkspaceLevelOfDetail::Full:
        inner_interaction_happen |= WorkspaceNodeWithCoreDataWithPins::leftContent();
        break;
    }

    return inner_interaction_happen;
}

bool WorkspaceCycle::rightContent()
{
    bool inner_interaction_happen = false;

    ImRect nodeRect; /* just declaration before switch */
    ImVec2 pinConnectionPoint;

    switch (m_levelOfDetail)
    {
    case WorkspaceLevelOfDetail::Label:
        nodeRect = getNodeRectDiwne();
        pinConnectionPoint = ImVec2(nodeRect.Max.x, (nodeRect.Min.y + nodeRect.Max.y)/2);
        for (auto const& pin : m_workspaceOutputs) {
            pin->setConnectionPointDiwne(pinConnectionPoint);
        }

        break;
    case WorkspaceLevelOfDetail::LightCycle:

        for (auto const i : {Core::I3T_CYCLE_OUT_VAL})
        {
            m_workspaceOutputs.at(i)->drawDiwne();
        }

        for (auto const i : {Core::I3T_CYCLE_OUT_PLAY, Core::I3T_CYCLE_OUT_PAUSE, Core::I3T_CYCLE_OUT_STOP, Core::I3T_CYCLE_OUT_PREV, Core::I3T_CYCLE_OUT_NEXT, Core::I3T_CYCLE_OUT_END})
        {
            m_workspaceOutputs.at(i)->setConnectionPointDiwne(pinConnectionPoint);
        }

        break;
    case WorkspaceLevelOfDetail::Full:
        inner_interaction_happen |= WorkspaceNodeWithCoreDataWithPins::rightContent();
        break;
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
    ImVec2 button_sz = I3T::getSize(ESizeVec2::Nodes_FloatCycleButtonSize)*diwne.getWorkAreaZoom();

    switch (m_levelOfDetail)
    {
    case WorkspaceLevelOfDetail::Label:

        ImGui::TextUnformatted(m_middleLabel.c_str());

        break;
    case WorkspaceLevelOfDetail::LightCycle:

        inner_interaction_happen |= buttonPlayPause(button_sz);
        ImGui::SameLine(); inner_interaction_happen |= buttonStopAndReset(button_sz);

        break;
    case WorkspaceLevelOfDetail::Full:

        inner_interaction_happen |= buttonPlayPause(button_sz); ImGui::SameLine();
		inner_interaction_happen |= buttonStopAndReset(button_sz); ImGui::SameLine();
		inner_interaction_happen |= buttonStepBack(button_sz); ImGui::SameLine();
		inner_interaction_happen |= buttonStepNext(button_sz);

		/* =================================== */
		// Mode select
		int mode = static_cast<int>(m_nodebase->as<Core::Cycle>()->getMode());

		//TODO is there a way to calculate how much space take radio button with text?

		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(button_sz.x - 10.0f, 0.0f));

		ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, {I3T::getSize(ESizeVec2::Nodes_FloatPadding).x, button_sz.y / 16});

		ImGui::RadioButton("Once", &mode, 0);
		ImGui::SameLine();

		// Multiplier = step for a single tick
		const float coreData_manStep		 = m_nodebase->as<Core::Cycle>()->getManualStep();
		float				localData		 = coreData_manStep;
		bool				valueChanged = false;

		ImGui::PushItemWidth(2 * button_sz.x + 4 * I3T::getSize(ESizeVec2::Nodes_ItemsSpacing).x + 1.0f);


		inner_interaction_happen |= drawDragFloatWithMap_Inline(diwne, getNumberOfVisibleDecimal(), m_floatPopupMode, fmt::format("##{}:{}", getId(), 0),
                                    localData, 1, valueChanged);

		if (valueChanged)
		{
			//m_nodebase->as<Core::Cycle>()->setMultiplier(localData); //wrong
			m_nodebase->as<Core::Cycle>()->setManualStep(localData); //PF
			setDataItemsWidth();
		}

		ImGui::PopItemWidth();

		ImGui::RadioButton("Repeat", &mode, 1);
		ImGui::RadioButton("Ping-Pong", &mode, 2);

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
		ImGui::PopStyleVar();
		ImGui::PopStyleVar();

		const float coreData_float = m_nodebase->as<Core::Cycle>()->getData().getFloat();
		const Core::Transform::DataMap& coreMap	 = m_nodebase->getDataMapRef();

//		bool	valueChanged = false;
//		float localData;
        valueChanged = false;

		ImGui::PushItemWidth(getDataItemsWidth());
		ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, I3T::getSize(ESizeVec2::Nodes_FloatPadding));
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, I3T::getSize(ESizeVec2::Nodes_ItemsSpacing));

		localData = coreData_float;
		inner_interaction_happen |= drawDragFloatWithMap_Inline(diwne, getNumberOfVisibleDecimal(), m_floatPopupMode, fmt::format("##{}:{}", getId(), 1),
                                    localData, coreMap[0], valueChanged);


		ImGui::PopStyleVar();
		ImGui::PopStyleVar();
		ImGui::PopItemWidth();

		if (valueChanged)
		{
			//ask MH \\ \todo SetValue does not call setDirectionMultiplier()
			m_nodebase->setValue(localData);
			setDataItemsWidth();
		}

        break;
    }
    return inner_interaction_happen;
}

//void WorkspaceCycle::drawInputPin(util::NodeBuilder& builder, Ptr<WorkspaceCorePinProperties> const& pinProp,
//																	Core::Pin* newLinkPin)
//{
//
//	float alpha = ImGui::GetStyle().Alpha;
//
//	builder.Input(pinProp->getId(), I3T::getColor(WorkspacePinColor[pinProp->getType()]));
//
//	ImGui::PushStyleVar(ImGuiStyleVar_Alpha, alpha);
//
//	ImGui::BeginVertical(pinProp->getId().AsPointer());
//	ImGui::Spring(1);
//
//	// color.Value.w = alpha / 255.0f;
//	ax::Widgets::Icon(pinProp->getIconSize(), WorkspacePinShape[pinProp->getType()],
//										pinProp->isConnected(), // SS add global variable. User test change or not.
//										I3T::getColor(WorkspacePinColor[pinProp->getType()]),
//										I3T::getColor(WorkspaceInnerPinColor[pinProp->getType()]));
//
//	ImGui::Spring(1);
//	ImGui::EndVertical();
//
//	if (pinProp->getShowLabel())
//	{
//		if (pinProp->getLabel().empty())
//		{ //it's never empty :(
//
//			auto label = pinProp->getCorePin().getLabel();
//			if (label == "float" || label == "vec3" || label == "vec4" || label == "matrix" || label == "quat" ||
//					label == "pulse")
//			{
//				ImGui::TextUnformatted("");
//			}
//			else
//			{
//				ImGui::Spring(0, I3T::getSize(ESize::Nodes_LabelIndent));
//				ImGui::TextUnformatted(label);
//			}
//		}
//		else
//		{
//
//			auto label = pinProp->getLabel();
//			if (label == "float" || label == "vec3" || label == "vec4" || label == "matrix" || label == "quat" ||
//					label == "pulse")
//			{
//				ImGui::TextUnformatted("");
//			}
//			else
//			{
//				ImGui::Spring(0, I3T::getSize(ESize::Nodes_LabelIndent));
//				ImGui::TextUnformatted(label.c_str());
//			}
//		}
//	}
//
//	float														coreData = NAN;
//	const Core::Transform::DataMap& coreMap	 = m_nodebase->getDataMapRef();
//	switch (pinProp->getIndex())
//	{
//	case 0:
//		coreData = m_nodebase->as<Core::Cycle>()->getFrom();
//		break;
//	case 1:
//		coreData = m_nodebase->as<Core::Cycle>()->getTo();
//		break;
//	case 2:
//		//coreData = m_nodebase->as<Core::Cycle>()->getManualStep();
//		coreData = m_nodebase->as<Core::Cycle>()->getMultiplier();
//		break;
//	default:
//		break;
//	}
//
//	int const idOfNode = this->m_id.Get();
//
//	bool	valueChanged = false;
//	float localData;
//
//	ImGui::PushItemWidth(getDataItemsWidth(diwne));
//	ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, I3T::getSize(ESizeVec2::Nodes_FloatPadding));
//	ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, I3T::getSize(ESizeVec2::Nodes_ItemsSpacing));
//
//	localData = coreData;
//
//	if (!isnan(localData))
//	{
//		drawDragFloatWithMap_Inline(valueChanged,
//				localData, coreMap[0],
//				fmt::format("##{}:{}", idOfNode, pinProp->getIndex())); /* datamap value 1 is changeable */
//	}
//
//	if (ImGui::IsMouseReleased(1) && ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenBlockedByPopup))
//	{
//		fw.showMyPopup = true;
//		fw.id					 = fmt::format("##{}", idOfNode);
//		fw.value			 = localData;
//	}
//
//
//	ImGui::PopStyleVar();
//	ImGui::PopStyleVar();
//	ImGui::PopItemWidth();
//
//	if (valueChanged)
//	{
//		m_nodebase->setValue(localData);
//		setDataItemsWidth();
//	}
//
//
//	ImGui::PopStyleVar();
//	builder.EndInput();
//}

int WorkspaceCycle::maxLenghtOfData()
{
    Ptr<Core::Cycle> nodebase = m_nodebase->as<Core::Cycle>();
	return std::max({numberOfCharWithDecimalPoint(nodebase->getFrom(), m_numberOfVisibleDecimal)
                    , numberOfCharWithDecimalPoint(nodebase->getTo(), m_numberOfVisibleDecimal)
                    , numberOfCharWithDecimalPoint(nodebase->getManualStep(), m_numberOfVisibleDecimal)
                    , numberOfCharWithDecimalPoint(nodebase->getMultiplier(), m_numberOfVisibleDecimal)});
}
