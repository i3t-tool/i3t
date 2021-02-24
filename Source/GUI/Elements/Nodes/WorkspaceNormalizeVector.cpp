#include "WorkspaceNormalizeVector.h"

WorkspaceNormalizeVector::WorkspaceNormalizeVector(ImTextureID headerBackground)
        : WorkspaceNodeBaseData( Builder::createNode<ENodeType::NormalizeVector>() )
        , WorkspaceNode( Nodebase->getId(), "default NormalizeVector label", headerBackground ) /* \todo take default label from Const.h */
{}




void WorkspaceNormalizeVector::drawWorkspaceNodeData(util::NodeBuilder& builder)
{
    const glm::vec4& coreData = Nodebase->getInternalData().getVec4();
    bool valueCH = false;

    glm::vec4 localData = coreData;
    float localDataArray[4] = {0, 0, 0, 0};

    fromVecToArray4(localData, localDataArray);

    builder.Middle();

    ImGui::PushItemWidth(200.0f);
    valueCH = ImGui::InputFloat4("x", localDataArray, 3); /* \todo /"x"/ as some property of class? or from Const.h? */
    ImGui::PopItemWidth();

    if (valueCH)
    {
      fromArrayToVec4(localData, localDataArray);
      Nodebase->getInternalData().setValue(localData);
    }

    ImGui::Spring(0);

}


void WorkspaceNormalizeVector::drawWorkspaceNode(util::NodeBuilder& builder, Core::Pin* newLinkPin)
{
    builder.Begin(Id);

    drawWorkspaceNodeHeader(builder);
    drawWorkspaceInputs(builder, newLinkPin);
    drawWorkspaceNodeData(builder);
    drawWorkspaceOutputs(builder, newLinkPin);

    builder.End();
}




