#pragma once
#include "WorkspaceMatrix4x4.h"

struct WorkspaceMakeEulerXArgs
{
    WorkspaceLevelOfDetail levelOfDetail = WorkspaceLevelOfDetail::Full;
    std::string headerLabel = "default EulerX header";
    std::string nodeLabel = "Euler X";
    Ptr<Core::NodeBase> nodebase = Core::Builder::createNode<ENodeType::MakeEulerX>();
};

class WorkspaceMakeEulerX : public WorkspaceMatrix4x4
{
public:
    WorkspaceMakeEulerX(ImTextureID headerBackground, WorkspaceMakeEulerXArgs const& args);
    WorkspaceMakeEulerX(ImTextureID headerBackground, std::string headerLabel = "Euler X", std::string nodeLabel = "Euler X");

    void drawDataSetValues(util::NodeBuilder& builder);
};
