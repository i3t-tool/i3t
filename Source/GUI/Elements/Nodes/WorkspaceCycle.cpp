//
// Created by Sofie on 07.05.2021.
//

#include "WorkspaceCycle.h"

WorkspaceCycle::WorkspaceCycle(ImTextureID headerBackground, WorkspaceCycleArgs const& args)
    : WorkspaceNodeWithCoreData(headerBackground, {.levelOfDetail = args.levelOfDetail, .headerLabel = args.headerLabel, .nodeLabel = args.nodeLabel, .nodebase = args.nodebase })
{
  fw.showMyPopup = false;
  fw.id = "";
  fw.value = NAN;
  fw.name = "WorkspaceCycle";
  setDataItemsWidth();
}

WorkspaceCycle::WorkspaceCycle(ImTextureID headerBackground,  Ptr<Core::Cycle> nodebase, std::string headerLabel, std::string nodeLabel)
    : WorkspaceNodeWithCoreData(headerBackground, nodebase == nullptr ? Core::GraphManager::createCycle() : nodebase, headerLabel, nodeLabel)
{
  fw.showMyPopup = false;
  fw.id = "";
  fw.value = NAN;
  fw.name = "WorkspaceCycle";
  setDataItemsWidth();
}

bool WorkspaceCycle::isCycle()
{
  return true;
}

void WorkspaceCycle::drawDataSetValues(util::NodeBuilder& builder)
{
  drawDataFull(builder,0);
}

void WorkspaceCycle::drawDataFull(util::NodeBuilder& builder, int index)
{

  if(index == -1){  // -> draw middle
    //BUTTONS
    ImVec2 button_sz = I3T::getSize(ESizeVec2::Nodes_FloatCycleButtonSize);

		// "⯈/❙❙" "◼" "❙⯇" "⯈❙"
		//std::u8string string = u8"⯈";
		//std::u8string string = u8"ěščřžýáíé";
		//std::string s(string.cbegin(), string.cend());

		if(ImGui::Button("P/P", button_sz)){
			if(m_nodebase->as<Core::Cycle>()->isRunning()){
				m_nodebase->as<Core::Cycle>()->stop();
			}else{
				m_nodebase->as<Core::Cycle>()->play();
			}
		}
    ImGui::SameLine();
		if(ImGui::Button("SaR", button_sz)){
      m_nodebase->as<Core::Cycle>()->resetAndStop();
		}
    ImGui::SameLine();
    if(ImGui::Button("SB", button_sz)){
      m_nodebase->as<Core::Cycle>()->stepBack();
		}
    ImGui::SameLine();
    if(ImGui::Button("SN", button_sz)){
      m_nodebase->as<Core::Cycle>()->stepNext();
		}




		//Mode select
		static int mode = 0;

		//TODO is there a way to calculate how much space take radio button with text?

		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(button_sz.x-10.0f, 0.0f));

		ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, {I3T::getSize(ESizeVec2::Nodes_FloatPadding).x, button_sz.y/16});

		ImGui::RadioButton("Once", &mode, 0);
		ImGui::SameLine();

		//Multiplier
		const float coreData = m_nodebase->as<Core::Cycle>()->getMultiplier();
		float localData = coreData;
		int const idOfNode = this->m_id.Get();
		bool valueChanged = false;

		ImGui::PushItemWidth(2*button_sz.x + 4* I3T::getSize(ESizeVec2::Nodes_ItemsSpacing).x + 1.0f);


		valueChanged |= drawDragFloatWithMap_Inline(&localData, 1, fmt::format("##{}:{}", idOfNode, index));

		if (ImGui::IsMouseReleased(1) && ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenBlockedByPopup))
		{
			fw.showMyPopup = true;
			fw.id = fmt::format("##{}", idOfNode);
			fw.value = localData;
		}

		if (valueChanged)
		{
			m_nodebase->as<Core::Cycle>()->setMultiplier(localData);
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
		}
		ImGui::PopStyleVar();
		ImGui::PopStyleVar();




  }else if (index == 0){
    const float coreData = m_nodebase->as<Core::Cycle>()->getData().getFloat();
    int const idOfNode = this->m_id.Get();
    const Core::Transform::DataMap& coreMap = m_nodebase->getDataMapRef();

    bool valueChanged = false;
    float localData;

    ImGui::PushItemWidth(m_dataItemsWidth);
		ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, I3T::getSize(ESizeVec2::Nodes_FloatPadding));
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, I3T::getSize(ESizeVec2::Nodes_ItemsSpacing));

    localData = coreData;
    valueChanged |= drawDragFloatWithMap_Inline(&localData, coreMap[0], fmt::format("##{}:{}", idOfNode, index)); /* datamap value 1 is changeable */

    if (ImGui::IsMouseReleased(1) && ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenBlockedByPopup))
    {
      fw.showMyPopup = true;
      fw.id = fmt::format("##{}", idOfNode);
      fw.value = localData;
    }


    ImGui::PopStyleVar();
    ImGui::PopStyleVar();
    ImGui::PopItemWidth();

    if (valueChanged)
    {
      //ask MH
      m_nodebase->setValue(localData);
      setDataItemsWidth();
    }



	}
}

void WorkspaceCycle::drawInputPin(util::NodeBuilder& builder, Ptr<WorkspaceCorePinProperties> const & pinProp, Core::Pin* newLinkPin){

  float alpha = ImGui::GetStyle().Alpha;

  builder.Input(pinProp->getId(), I3T::getColor(WorkspacePinColor[pinProp->getType()]));

  ImGui::PushStyleVar(ImGuiStyleVar_Alpha, alpha);

  ImGui::BeginVertical(pinProp->getId().AsPointer());
  ImGui::Spring(1);

  // color.Value.w = alpha / 255.0f;
  ax::Widgets::Icon(pinProp->getIconSize(),
                    WorkspacePinShape[pinProp->getType()],
                    pinProp->isConnected(), // SS add global variable. User test change or not.
										I3T::getColor(WorkspacePinColor[pinProp->getType()]),
										I3T::getColor(WorkspaceInnerPinColor[pinProp->getType()]));

  ImGui::Spring(1);
  ImGui::EndVertical();

  if (pinProp->getShowLabel()){
    if(pinProp->getLabel().empty()){ //it's never empty :(

      auto label = pinProp->getCorePin().getLabel();
      if(label == "float" || label == "vec3" || label == "vec4" || label == "matrix" || label == "quat" || label == "pulse"){
        ImGui::TextUnformatted("");
      }else
      {
        ImGui::Spring(0,I3T::getSize(ESize::Nodes_LabelIndent));
        ImGui::TextUnformatted(label);
      }

    }else{

      auto label = pinProp->getLabel();
      if(label == "float" || label == "vec3" || label == "vec4" || label == "matrix" || label == "quat" || label == "pulse"){
        ImGui::TextUnformatted("");
      }else{
        ImGui::Spring(0,I3T::getSize(ESize::Nodes_LabelIndent));
        ImGui::TextUnformatted(label.c_str());
      }

    }
  }

	float coreData = NAN;
  const Core::Transform::DataMap& coreMap = m_nodebase->getDataMapRef();
	switch (pinProp->getIndex()){
	case 0:
    coreData = m_nodebase->as<Core::Cycle>()->getFrom();
		break;
	case 1:
    coreData = m_nodebase->as<Core::Cycle>()->getTo();
    break;
	case 2:
    coreData = m_nodebase->as<Core::Cycle>()->getStep();
    break;
	default:
    break;
	}

  int const idOfNode = this->m_id.Get();

  bool valueChanged = false;
  float localData;

  ImGui::PushItemWidth(m_dataItemsWidth);
	ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, I3T::getSize(ESizeVec2::Nodes_FloatPadding));
	ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, I3T::getSize(ESizeVec2::Nodes_ItemsSpacing));

  localData = coreData;

	if(!isnan(localData)){
    valueChanged |= drawDragFloatWithMap_Inline(&localData, coreMap[0], fmt::format("##{}:{}", idOfNode, pinProp->getIndex())); /* datamap value 1 is changeable */
  }

  if (ImGui::IsMouseReleased(1) && ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenBlockedByPopup))
  {
    fw.showMyPopup = true;
    fw.id = fmt::format("##{}", idOfNode);
    fw.value = localData;
  }


  ImGui::PopStyleVar();
  ImGui::PopStyleVar();
  ImGui::PopItemWidth();

  if (valueChanged)
  {
    m_nodebase->setValue(localData);
    setDataItemsWidth();
  }



  ImGui::PopStyleVar();
  builder.EndInput();
}

int WorkspaceCycle::maxLenghtOfData()
{
  float m = std::max({m_nodebase->as<Core::Cycle>()->getFrom(), m_nodebase->as<Core::Cycle>()->getTo(), m_nodebase->as<Core::Cycle>()->getStep(), m_nodebase->as<Core::Cycle>()->getMultiplier()});
  return numberOfCharWithDecimalPoint( m, m_numberOfVisibleDecimal );
}
