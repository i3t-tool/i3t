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
}

WorkspaceCycle::WorkspaceCycle(ImTextureID headerBackground,  Ptr<Core::Cycle> nodebase, std::string headerLabel, std::string nodeLabel)
    : WorkspaceNodeWithCoreData(headerBackground, Core::GraphManager::createCycle(), headerLabel, nodeLabel)
{
  fw.showMyPopup = false;
  fw.id = "";
  fw.value = NAN;
  fw.name = "WorkspaceCycle";
}

void WorkspaceCycle::drawDataSetValues(util::NodeBuilder& builder)
{
  drawDataFull(builder,0);
}

void WorkspaceCycle::drawDataFull(util::NodeBuilder& builder, int index)
{

}

void WorkspaceCycle::drawInputPin(util::NodeBuilder& builder, Ptr<WorkspaceCorePinProperties> const & pinProp, Core::Pin* newLinkPin){

  float alpha = ImGui::GetStyle().Alpha;

  builder.Input(pinProp->getId());

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
        ImGui::TextUnformatted(label);
      }

    }else{

      auto label = pinProp->getLabel();
      if(label == "float" || label == "vec3" || label == "vec4" || label == "matrix" || label == "quat" ){
        ImGui::TextUnformatted("");
      }else{
        ImGui::TextUnformatted(label.c_str());
      }

    }
    ImGui::Spring(1);
  }


  //assert if operator its imposible to value change (except free operators)
  const float coreData = m_nodebase->getData(pinProp->getIndex()).getFloat(); //ERROR
  int const idOfNode = this->m_id.Get();
  const Core::Transform::DataMap& coreMap = m_nodebase->getDataMapRef();

  bool valueChanged = false;
  float localData;

  ImGui::PushItemWidth(m_dataItemsWidth);
  ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, {I3T::getSize(ESize::Nodes_floatPaddingX), I3T::getSize(ESize::Nodes_floatPaddingY)});
  ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, { I3T::getSize(ESize::Nodes_ItemsSpacingX), I3T::getSize(ESize::Nodes_ItemsSpacingY) });

  localData = coreData;
  valueChanged |= drawDragFloatWithMap_Inline(&localData, coreMap[0], fmt::format("##{}:{}", idOfNode, pinProp->getIndex())); /* datamap value 1 is changeable */

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
	return 0;
}
