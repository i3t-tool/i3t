#pragma once
#include "WorkspaceMatrix4x4.h"

struct WorkspaceMakeEulerYArgs
{
    WorkspaceLevelOfDetail levelOfDetail = WorkspaceLevelOfDetail::Full;
    std::string headerLabel = "default EulerY header";
    std::string nodeLabel = "Euler Y";
    Ptr<Core::NodeBase> nodebase = Core::Builder::createNode<ENodeType::MakeEulerY>();
};

class WorkspaceMakeEulerY : public WorkspaceMatrix4x4
{
public:
    WorkspaceMakeEulerY(ImTextureID headerBackground, WorkspaceMakeEulerYArgs const& args);
    WorkspaceMakeEulerY(ImTextureID headerBackground, std::string headerLabel = "Euler Y", std::string nodeLabel = "Euler Y");

    void drawDataSetValues(util::NodeBuilder& builder);

    float get00();
    float get02();
    float get20();
    float get22();
};
