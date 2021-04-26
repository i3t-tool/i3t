#pragma once
#include "WorkspaceFloat.h"

struct WorkspaceVector3LengthArgs
{
    WorkspaceLevelOfDetail levelOfDetail = WorkspaceLevelOfDetail::Full;
    std::string headerLabel = "default Vector3Length header";
    std::string nodeLabel = "default Vector3Length label";
    Ptr<Core::NodeBase> nodebase = Core::Builder::createNode<ENodeType::Vector3Length>();
};

class WorkspaceVector3Length : public WorkspaceFloat
{
public:
    WorkspaceVector3Length(ImTextureID headerBackground, WorkspaceVector3LengthArgs const& args);
    WorkspaceVector3Length(ImTextureID headerBackground, std::string headerLabel = "Vector3Length", std::string nodeLabel = "Vector3Length");

    void drawDataSetValues(util::NodeBuilder& builder);

};

