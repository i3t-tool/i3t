#pragma once
#include "WorkspaceMatrix4x4.h"

struct WorkspaceMatrixRotateArgs
{
    WorkspaceLevelOfDetail levelOfDetail = WorkspaceLevelOfDetail::Full;
    std::string headerLabel = "default Rotate header";
    std::string nodeLabel = "Rotate";
    Ptr<Core::NodeBase> nodebase = Core::Builder::createNode<ENodeType::MakeAxisAngle>();
};

class WorkspaceMatrixRotate : public WorkspaceMatrix4x4
{
public:
    WorkspaceMatrixRotate(ImTextureID headerBackground, WorkspaceMatrixRotateArgs const& args);
    WorkspaceMatrixRotate(ImTextureID headerBackground, std::string headerLabel = "Rotate", std::string nodeLabel = "Rotate");

    void drawDataSetValues(util::NodeBuilder& builder);

};
