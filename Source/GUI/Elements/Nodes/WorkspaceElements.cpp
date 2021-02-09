
#include "WorkspaceElements.h"


// \TODO change to core pintypes
WorkspacePin::drawPin(){
      color.Value.w = alpha / 255.0f;
  ax::Widgets::Icon(ImVec2(s_PinIconSize, s_PinIconSize), WorkspacePinShape[Type], connected, WorkspacePinColor[Type], ImColor(32, 32, 32, alpha));

}



WorkspacePin::IsPinConected()
{
return conected;
}


WorkspaceNode::TouchNode(float touchTime)
{
  TouchTime = touchTime;
}


WorkspaceNodeHeader::drawNodeHeader(util::NodeBuilder& builder)
{
      builder.Header(Color);
      ImGui::Spring(0);
      ImGui::TextUnformatted(Name.c_str());
      ImGui::Spring(1);
      ImGui::Dummy(ImVec2(0, 28));
      //ImGui::Spring(0);

      ImGui::BeginVertical("delegates", ImVec2(0, 28));
      ImGui::Spring(1, 0);
      ImGui::Spring(1, 0);
      ImGui::EndVertical();
      ImGui::Spring(0, ImGui::GetStyle().ItemSpacing.x / 2);
      builder.EndHeader();
}


WorkspaceNodeData::drawData(util::NodeBuilder& builder)
{
    glm::mat4& data = nodebase->getInternalData().getMat4();
    bool valueCH = false;
    std::string s = "";
    const char* c = "";

    glm::mat4 f;
    for (int rows = 0; rows < 4; rows++) {
        for (int columns = 0; columns < 4; columns++) {
          f[rows][columns] = data[rows][columns];
      }
    }

    builder.Middle();

    ImGui::PushItemWidth(50.0f);
    for (int rows = 0; rows < 4; rows++)
    {
      for (int columns = 0; columns < 4; columns++)
      {
          s = "##";
          s += std::to_string(rows);
          s += std::to_string(columns);
          c = s.c_str();
          valueCH |= ImGui::InputFloat(c, &f[rows][columns]);
          if (columns < 3)
          {
            ImGui::SameLine();
          }
        }
      }
    ImGui::PopItemWidth();

    if (valueCH)
    {
      nodebase->getInternalData().setValue(f);
    }

    ImGui::Spring(0);

}


WorkspaceNodeInputs::drawInputs(util::NodeBuilder& builder, GUIPin* newLinkPin)
{
    for (auto& input : Inputs)
        {
            float alpha = ImGui::GetStyle().Alpha;
            if (newLinkPin && !CanCreateLink(newLinkPin, input) && input != newLinkPin)
              alpha = alpha * (48.0f / 255.0f);

            builder.Input(input->ID);
            ImGui::PushStyleVar(ImGuiStyleVar_Alpha, alpha);
            DrawPinIcon(*input, IsPinLinked(input->ID), (int)(alpha * 255));
            ImGui::Spring(0);
            if (!input->Name.empty())
            {
              ImGui::TextUnformatted(input->Name.c_str());
              ImGui::Spring(0);
            }

            ImGui::PopStyleVar();
            builder.EndInput();
        }
}


WorkspaceNodeOutputs::drawOutputs(util::NodeBuilder& builder, GUIPin* newLinkPin)
{
    for (auto& output : Outputs)
    {
        float alpha = ImGui::GetStyle().Alpha;

        if (newLinkPin && !CanCreateLink(newLinkPin, output) && output != newLinkPin)
          alpha = alpha * (48.0f / 255.0f);

        ImGui::PushStyleVar(ImGuiStyleVar_Alpha, alpha);
        builder.Output(output->ID);
        if (!output->Name.empty())
        {
          ImGui::TextUnformatted(output->Name.c_str());
          ImGui::Spring(0);
        }
        DrawPinIcon(*output, IsPinLinked(output->ID), (int)(alpha * 255));
        ImGui::PopStyleVar();
        builder.EndOutput();
    }
}



