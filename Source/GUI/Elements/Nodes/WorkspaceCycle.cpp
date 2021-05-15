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
    ImVec2 button_sz(32.0f, 32.0f);
    ImTextureID my_tex_id = (ImTextureID)(intptr_t)pgr::createTexture(
        Config::getAbsolutePath("Data/textures/icons2.png"),
        true);

		//   ImGui::ImageButton(ImTextureID, size, uv0, uv1, frame padding, tint color)

		if (ImGui::ImageButton(my_tex_id, button_sz, ImVec2(0*button_sz.x/128.0f,button_sz.y/128.0f), ImVec2(button_sz.x/128.0f,2*button_sz.y/128.0f), -1, I3T::getColor(EColor::NodeBgOperator))){
      m_nodebase->as<Core::Cycle>()->play();
		}
    ImGui::SameLine();
    if (ImGui::ImageButton(my_tex_id, button_sz, ImVec2(2*button_sz.x/128.0f,button_sz.y/128.0f), ImVec2(3*button_sz.x/128.0f,2*button_sz.y/128.0f), -1, I3T::getColor(EColor::NodeBgOperator))){
      m_nodebase->as<Core::Cycle>()->stop();
    }
    ImGui::SameLine();
    if (ImGui::ImageButton(my_tex_id, button_sz, ImVec2(button_sz.x/128.0f,button_sz.y/128.0f), ImVec2(2*button_sz.x/128.0f,2*button_sz.y/128.0f), -1, I3T::getColor(EColor::NodeBgOperator))){
      m_nodebase->as<Core::Cycle>()->resetAndStop();
    }
    ImGui::SameLine();
    if (ImGui::ImageButton(my_tex_id, button_sz, ImVec2(0*button_sz.x/128.0f,2*button_sz.y/128.0f), ImVec2(button_sz.x/128.0f,3*button_sz.y/128.0f), -1, I3T::getColor(EColor::NodeBgOperator))){
      m_nodebase->as<Core::Cycle>()->stepBack();
    }
    ImGui::SameLine();
    if (ImGui::ImageButton(my_tex_id, button_sz, ImVec2(3*button_sz.x/128.0f,button_sz.y/128.0f), ImVec2(4*button_sz.x/128.0f,2*button_sz.y/128.0f), -1, I3T::getColor(EColor::NodeBgOperator))){
      m_nodebase->as<Core::Cycle>()->stepNext();
    }


		/*if(ImGui::Button("play", button_sz)){
      m_nodebase->as<Core::Cycle>()->play();
		}
		ImGui::SameLine();
		if(ImGui::Button("stop", button_sz)){
      m_nodebase->as<Core::Cycle>()->stop();
		}
    ImGui::SameLine();
		if(ImGui::Button("reset and stop", button_sz)){
      m_nodebase->as<Core::Cycle>()->resetAndStop();
		}
    ImGui::SameLine();
    if(ImGui::Button("step back", button_sz)){
      m_nodebase->as<Core::Cycle>()->stepBack();
		}
    ImGui::SameLine();
    if(ImGui::Button("step next", button_sz)){
      m_nodebase->as<Core::Cycle>()->stepNext();
		}*/


		//Mode select
    bool open_popup = false;
    Core::Cycle::EMode core_mode = m_nodebase->as<Core::Cycle>()->getMode();
		const char* mode = "";
		switch(core_mode){
		case Core::Cycle::EMode::Once:
			mode = "Mode: Once                 ";
			break;
    case Core::Cycle::EMode::Repeat:
			mode = "Mode: Repeat             ";
      break;
    case Core::Cycle::EMode::PingPong:
			mode = "Mode: Ping-Pong        ";
      break;
		}

    //ImGui::PushStyleVar(ImGuiStyleVar_ButtonTextAlign, ImVec2(0.5f,10.00f)); // Doesnt work :(
    //open_popup |= ImGui::Button(mode, ImVec2(3*button_sz.x+I3T::getSize(ESize::Nodes_floatPaddingX)*2, button_sz.y/2));

    ImGui::PushItemWidth(3*button_sz.x+I3T::getSize(ESize::Nodes_floatPaddingX)*2);
		//ImGui::
		ImGui::Text("%s", mode);

    if (ImGui::IsMouseReleased(1) && ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenBlockedByPopup))
    {
      fw.showMyPopup = true;
      fw.id = fmt::format("Mode");
      fw.value = 0;
    }

		ImGui::PopItemWidth();
    //ImGui::PopStyleVar();


    /*ImGui::PushItemWidth(3*button_sz.x+I3T::getSize(ESize::Nodes_floatPaddingX)*2);
    static int i = -1;
    if (ImGui::Combo(mode, &i, "Once\0Repeat\0Ping-Pong\0"))
    {
      switch (i)
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
    }
    ImGui::PopItemWidth();*/


    ImGui::SameLine();

		//Multiplier
    const float coreData = m_nodebase->as<Core::Cycle>()->getMultiplier();
    float localData = coreData;
    int const idOfNode = this->m_id.Get();
    bool valueChanged = false;

    ImGui::PushItemWidth(2*button_sz.x+I3T::getSize(ESize::Nodes_floatPaddingX));
    ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, {I3T::getSize(ESize::Nodes_floatPaddingX), button_sz.y/16});

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

		ImGui::PopStyleVar();
    ImGui::PopItemWidth();

  }else if (index == 0){
    const float coreData = m_nodebase->as<Core::Cycle>()->getData().getFloat();
    int const idOfNode = this->m_id.Get();
    const Core::Transform::DataMap& coreMap = m_nodebase->getDataMapRef();

    bool valueChanged = false;
    float localData;

    ImGui::PushItemWidth(m_dataItemsWidth);
    ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, {I3T::getSize(ESize::Nodes_floatPaddingX), I3T::getSize(ESize::Nodes_floatPaddingY)});
    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, { I3T::getSize(ESize::Nodes_ItemsSpacingX), I3T::getSize(ESize::Nodes_ItemsSpacingY) });

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

  builder.Input(pinProp->getId(), WorkspacePinColor[pinProp->getType()]);

  ImGui::PushStyleVar(ImGuiStyleVar_Alpha, alpha);

  ImGui::BeginVertical(pinProp->getId().AsPointer());
  ImGui::Spring(1);

  // color.Value.w = alpha / 255.0f;
  ax::Widgets::Icon(pinProp->getIconSize(),
                    WorkspacePinShape[pinProp->getType()],
                    pinProp->isConnected(), // SS add global variable. User test change or not.
                    WorkspacePinColor[pinProp->getType()],
                    pinProp->getColor());

  ImGui::Spring(1);
  ImGui::EndVertical();

  if (pinProp->getShowLabel()){
    if(pinProp->getLabel().empty()){ //it's never empty :(

      auto label = pinProp->getCorePin().getLabel();
      if(label == "float" || label == "vec3" || label == "vec4" || label == "matrix" || label == "quat" ){
        ImGui::TextUnformatted("");
      }else
      {
        ImGui::Spring(0,I3T::getSize(ESize::Nodes_LabelIndent));
        ImGui::TextUnformatted(label);
      }

    }else{

      auto label = pinProp->getLabel();
      if(label == "float" || label == "vec3" || label == "vec4" || label == "matrix" || label == "quat" ){
        ImGui::TextUnformatted("");
      }else{
        ImGui::Spring(0,I3T::getSize(ESize::Nodes_LabelIndent));
        ImGui::TextUnformatted(label.c_str());
      }

    }
  }


  //assert if operator its imposible to value change (except free operators)
	float coreData = NAN;
  const Core::Transform::DataMap& coreMap = m_nodebase->getDataMapRef();
	switch (pinProp->getIndex()){
	case 0:
    coreData = m_nodebase->as<Core::Cycle>()->getFrom();
    //coreMap = m_nodebase->as<Core::Cycle>()->getDataMapRef();
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

	//m_projection = std::make_shared<WorkspaceSequence>(headerBackground, m_nodebase->as<Core::Camera>()->getProj());

  int const idOfNode = this->m_id.Get();

  bool valueChanged = false;
  float localData;

  ImGui::PushItemWidth(m_dataItemsWidth);
  ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, {I3T::getSize(ESize::Nodes_floatPaddingX), I3T::getSize(ESize::Nodes_floatPaddingY)});
  ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, { I3T::getSize(ESize::Nodes_ItemsSpacingX), I3T::getSize(ESize::Nodes_ItemsSpacingY) });

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
    //ask MH
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
