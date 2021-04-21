#pragma once
#include "WorkspaceMatrix4x4.h"

struct WorkspaceMakeEulerZArgs
{
    WorkspaceLevelOfDetail levelOfDetail = WorkspaceLevelOfDetail::Full;
    std::string headerLabel = "default EulerZ header";
    std::string nodeLabel = "EulerZ";
    Ptr<Core::NodeBase> nodebase = Core::Builder::createNode<ENodeType::MakeEulerZ>();
};

class WorkspaceMakeEulerZ : public WorkspaceMatrix4x4
{
public:
    WorkspaceMakeEulerZ(ImTextureID headerBackground, WorkspaceMakeEulerZArgs const& args);
    WorkspaceMakeEulerZ(ImTextureID headerBackground, std::string headerLabel = "EulerZ", std::string nodeLabel = "EulerZ");

    void drawDataSetValues(util::NodeBuilder& builder);

    float get00();
    float get01();
    float get10();
    float get11();
};
