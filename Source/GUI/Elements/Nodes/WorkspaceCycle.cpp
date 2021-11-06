/**
 * \authors Sofie Sasorina, Martin Herich
 */

#include "WorkspaceCycle.h"

WorkspaceCycle::WorkspaceCycle() :
		WorkspaceNodeWithCoreData(Core::GraphManager::createCycle())
{
	setDataItemsWidth();
}

bool WorkspaceCycle::isCycle() { return true; }

void WorkspaceCycle::drawMenuLevelOfDetail()
{
	drawMenuLevelOfDetail_builder(std::dynamic_pointer_cast<WorkspaceNodeWithCoreData>(shared_from_this()), {WorkspaceLevelOfDetail::Full, WorkspaceLevelOfDetail::Label});
}

bool WorkspaceCycle::drawDataFull(DIWNE::Diwne& diwne, int index)
{
	if (index == -1)
	{ // -> draw middle
		//BUTTONS
		ImVec2 button_sz = I3T::getSize(ESizeVec2::Nodes_FloatCycleButtonSize);

		auto cycle = m_nodebase->as<Core::Cycle>();

		// \todo Add icons to buttons
		// "⯈/❙❙" "◼" "❙⯇" "⯈❙"
		//std::u8string string = u8"⯈";
		//std::u8string string = u8"ěščřžýáíé";
		//std::string s(string.cbegin(), string.cend());

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
		}
		ImGui::SameLine();
		if (ImGui::Button("SaR", button_sz)) { m_nodebase->as<Core::Cycle>()->stopAndReset(); }
		ImGui::SameLine();
		if (ImGui::Button("SB", button_sz)) { m_nodebase->as<Core::Cycle>()->stepBack(); }
		ImGui::SameLine();
		if (ImGui::Button("SN", button_sz)) { m_nodebase->as<Core::Cycle>()->stepNext(); }

		// Mode select
		int mode = static_cast<int>(cycle->getMode());

		//TODO is there a way to calculate how much space take radio button with text?

		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(button_sz.x - 10.0f, 0.0f));

		ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, {I3T::getSize(ESizeVec2::Nodes_FloatPadding).x, button_sz.y / 16});

		ImGui::RadioButton("Once", &mode, 0);
		ImGui::SameLine();

		// Multiplier = step for a single tick
		//const float coreData		 = m_nodebase->as<Core::Cycle>()->getMultiplier(); // wrong
		const float coreData		 = m_nodebase->as<Core::Cycle>()->getManualStep();
		float				localData		 = coreData;
		bool				valueChanged = false;

		ImGui::PushItemWidth(2 * button_sz.x + 4 * I3T::getSize(ESizeVec2::Nodes_ItemsSpacing).x + 1.0f);


		drawDragFloatWithMap_Inline(diwne, getNumberOfVisibleDecimal(), m_floatPopupMode, fmt::format("##{}:{}", getId(), index),
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
	}
	else if (index == 0)
	{
		const float coreData = m_nodebase->as<Core::Cycle>()->getData().getFloat();
		const Core::Transform::DataMap& coreMap	 = m_nodebase->getDataMapRef();

		bool	valueChanged = false;
		float localData;

		ImGui::PushItemWidth(getDataItemsWidth(diwne));
		ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, I3T::getSize(ESizeVec2::Nodes_FloatPadding));
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, I3T::getSize(ESizeVec2::Nodes_ItemsSpacing));

		localData = coreData;
		drawDragFloatWithMap_Inline(diwne, getNumberOfVisibleDecimal(), m_floatPopupMode, fmt::format("##{}:{}", getId(), index),
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
	}
	return false;
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

int WorkspaceCycle::maxLenghtOfData(int index)
{
    Ptr<Core::Cycle> nodebase = m_nodebase->as<Core::Cycle>();
	float m = std::max({nodebase->getFrom(), nodebase->getTo(), nodebase->getManualStep(), nodebase->getMultiplier()});
	return numberOfCharWithDecimalPoint(m, m_numberOfVisibleDecimal);
}
