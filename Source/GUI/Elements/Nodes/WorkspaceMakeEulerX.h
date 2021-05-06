#pragma once
#include "WorkspaceMatrix4x4.h"

struct WorkspaceMakeEulerXArgs
{
    WorkspaceLevelOfDetail levelOfDetail = WorkspaceLevelOfDetail::Full;
    std::string headerLabel = "default EulerX header";
    std::string nodeLabel = "EulerX";
    Ptr<Core::NodeBase> nodebase = Core::Builder::createNode<ENodeType::MakeEulerX>();
};

class WorkspaceMakeEulerX : public WorkspaceMatrix4x4
{
public:
    WorkspaceMakeEulerX(ImTextureID headerBackground, WorkspaceMakeEulerXArgs const& args);
    WorkspaceMakeEulerX(ImTextureID headerBackground, std::string headerLabel = "EulerX", std::string nodeLabel = "EulerX");

    void drawDataSetValues(util::NodeBuilder& builder);
};
