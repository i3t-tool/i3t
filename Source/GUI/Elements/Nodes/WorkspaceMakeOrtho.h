#pragma once
#include "WorkspaceMatrix4x4.h"

struct WorkspaceMakeOrthoArgs
{
    WorkspaceLevelOfDetail levelOfDetail = WorkspaceLevelOfDetail::Full;
    std::string headerLabel = "default Ortho header";
    std::string nodeLabel = "Ortho";
    Ptr<Core::NodeBase> nodebase = Builder::createNode<ENodeType::MakeOrtho>();
};

class WorkspaceMakeOrtho : public WorkspaceMatrix4x4
{
public:
    WorkspaceMakeOrtho(ImTextureID headerBackground, WorkspaceMakeOrthoArgs const& args);
    WorkspaceMakeOrtho(ImTextureID headerBackground, std::string headerLabel = "Ortho", std::string nodeLabel = "Ortho");

    void drawDataSetValues(util::NodeBuilder& builder);

};
