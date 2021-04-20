#pragma once
#include "WorkspaceMatrix4x4.h"

struct WorkspaceMakeEulerXArgs
{
    WorkspaceLevelOfDetail levelOfDetail = WorkspaceLevelOfDetail::Full;
    std::string headerLabel = "default EulerX header";
    std::string nodeLabel = "EulerX";
    Ptr<Core::NodeBase> nodebase = Builder::createNode<ENodeType::MakeEulerX>();
};

class WorkspaceMakeEulerX : public WorkspaceMatrix4x4
{
public:
    WorkspaceMakeEulerX(ImTextureID headerBackground, WorkspaceMakeEulerXArgs const& args);
    WorkspaceMakeEulerX(ImTextureID headerBackground, std::string headerLabel = "EulerX", std::string nodeLabel = "EulerX");

    void drawDataSetValues(util::NodeBuilder& builder);
    
    ValueSetResult set11(float val);
    ValueSetResult set12(float val);
    ValueSetResult set21(float val);
    ValueSetResult set22(float val);
    float get11();
    float get12();
    float get21();
    float get22();
};
