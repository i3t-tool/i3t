#pragma once
#include "WorkspaceMatrix4x4.h"

struct WorkspaceMakeEulerZArgs
{
    WorkspaceLevelOfDetail levelOfDetail = WorkspaceLevelOfDetail::Full;
    std::string headerLabel = "default EulerZ header";
    std::string nodeLabel = "EulerZ";
    Ptr<Core::NodeBase> nodebase = Builder::createNode<ENodeType::MakeEulerZ>();
};

class WorkspaceMakeEulerZ : public WorkspaceMatrix4x4
{
public:
    WorkspaceMakeEulerZ(ImTextureID headerBackground, WorkspaceMakeEulerZArgs const& args);
    WorkspaceMakeEulerZ(ImTextureID headerBackground, std::string headerLabel = "EulerZ", std::string nodeLabel = "EulerZ");

    void drawDataSetValues(util::NodeBuilder& builder);

    ValueSetResult set00(float val);
    ValueSetResult set01(float val);
    ValueSetResult set10(float val);
    ValueSetResult set11(float val);
    float get00();
    float get01();
    float get10();
    float get11();
};
