#pragma once
#include "WorkspaceVector4.h"

struct WorkspaceMixVectorArgs
{
    WorkspaceLevelOfDetail levelOfDetail = WorkspaceLevelOfDetail::Full;
    std::string headerLabel = "default MixVector header";
    std::string nodeLabel = "default MixVector label";
    Ptr<Core::NodeBase> nodebase = Core::Builder::createNode<ENodeType::MixVector>();
};

class WorkspaceMixVector : public WorkspaceVector4
{
public:
    WorkspaceMixVector(ImTextureID headerBackground, WorkspaceMixVectorArgs const& args);
    WorkspaceMixVector(ImTextureID headerBackground, std::string headerLabel = "MixVector", std::string nodeLabel = "MixVector");

    void drawDataSetValues(util::NodeBuilder& builder);

};

