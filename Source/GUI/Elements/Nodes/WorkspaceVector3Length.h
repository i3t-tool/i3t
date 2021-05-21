#pragma once
#include "WorkspaceFloat.h"

struct WorkspaceVector3LengthArgs
{
    WorkspaceLevelOfDetail levelOfDetail = WorkspaceLevelOfDetail::Full;
    std::string headerLabel = "default length(vec3) header";
    std::string nodeLabel = "default length(vec3) label";
    Ptr<Core::NodeBase> nodebase = Core::Builder::createNode<ENodeType::Vector3Length>();
};

class WorkspaceVector3Length : public WorkspaceFloat
{
public:
    WorkspaceVector3Length(ImTextureID headerBackground, WorkspaceVector3LengthArgs const& args);
    WorkspaceVector3Length(ImTextureID headerBackground, std::string headerLabel = "length(vec3)", std::string nodeLabel = "length(vec3)");

    void drawDataSetValues(util::NodeBuilder& builder);

};

