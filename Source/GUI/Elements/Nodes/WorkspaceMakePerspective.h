#pragma once
#include "WorkspaceMatrix4x4.h"

struct WorkspaceMakePerspectiveArgs
{
    WorkspaceLevelOfDetail levelOfDetail = WorkspaceLevelOfDetail::Full;
    std::string headerLabel = "default Perspective header";
    std::string nodeLabel = "Perspective";
    Ptr<Core::NodeBase> nodebase = Core::Builder::createNode<ENodeType::MakePerspective>();
};

class WorkspaceMakePerspective : public WorkspaceMatrix4x4
{
public:
    WorkspaceMakePerspective(ImTextureID headerBackground, WorkspaceMakePerspectiveArgs const& args);
    WorkspaceMakePerspective(ImTextureID headerBackground, std::string headerLabel = "Perspective", std::string nodeLabel = "Perspective");

    void drawDataSetValues(util::NodeBuilder& builder);

};
