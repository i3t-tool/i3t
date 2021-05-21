#pragma once
#include "WorkspaceVector4.h"

struct WorkspaceMixVectorArgs
{
    WorkspaceLevelOfDetail levelOfDetail = WorkspaceLevelOfDetail::Full;
    std::string headerLabel = "default mix vec4 header";
    std::string nodeLabel = "default mix vec4 label";
    Ptr<Core::NodeBase> nodebase = Core::Builder::createNode<ENodeType::MixVector>();
};

class WorkspaceMixVector : public WorkspaceVector4
{
public:
    WorkspaceMixVector(ImTextureID headerBackground, WorkspaceMixVectorArgs const& args);
    WorkspaceMixVector(ImTextureID headerBackground, std::string headerLabel = "Mix vec4", std::string nodeLabel = "Mix vec4");

    void drawDataSetValues(util::NodeBuilder& builder);

};

