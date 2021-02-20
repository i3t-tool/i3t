#include "WorkspaceMatrix4x4.h"

WorkspaceMatrix4x4::WorkspaceMatrix4x4(ImTextureID headerBackground)
        : WorkspaceNodeBaseData( Builder::createNode<ENodeType::Matrix>() )
        , WorkspaceNode( Nodebase->getId(), "default Matrix4x4 label", headerBackground ) /* \todo take default label from Const.h*/
{}


void WorkspaceMatrix4x4::drawWorkspaceNodeData(util::NodeBuilder& builder)
{
    const glm::mat4& coreData = Nodebase->getInternalData().getMat4();
    bool valueCH = false;
    std::string s = "";
    const char* c = "";

    glm::mat4 localData;
    for (int rows = 0; rows < 4; rows++) {
        for (int columns = 0; columns < 4; columns++) {
          localData[rows][columns] = coreData[rows][columns];
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
          valueCH |= ImGui::InputFloat(c, &localData[rows][columns]);
          if (columns < 3)
          {
            ImGui::SameLine();
          }
        }
      }
    ImGui::PopItemWidth();

    if (valueCH)
    {
      Nodebase->getInternalData().setValue(localData);
    }

    ImGui::Spring(0);

}


void WorkspaceMatrix4x4::drawWorkspaceNode(util::NodeBuilder& builder, Core::Pin* newLinkPin)
{
    builder.Begin(Id);

    drawWorkspaceNodeHeader(builder);
    drawWorkspaceInputLinks(builder);
    drawWorkspaceInputs(builder, newLinkPin);
    drawWorkspaceNodeData(builder);
    drawWorkspaceOutputs(builder, newLinkPin);

    builder.End();
}



