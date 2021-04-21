#pragma once
#include "WorkspaceMatrix4x4.h"

struct WorkspaceMakeFrustumArgs
{
    WorkspaceLevelOfDetail levelOfDetail = WorkspaceLevelOfDetail::Full;
    std::string headerLabel = "default Frustum header";
    std::string nodeLabel = "Frustum";
    Ptr<Core::NodeBase> nodebase = Core::Builder::createNode<ENodeType::MakeFrustum>();
};

class WorkspaceMakeFrustum : public WorkspaceMatrix4x4
{
public:
    WorkspaceMakeFrustum(ImTextureID headerBackground, WorkspaceMakeFrustumArgs const& args);
    WorkspaceMakeFrustum(ImTextureID headerBackground, std::string headerLabel = "Frustum", std::string nodeLabel = "Frustum");

    void drawDataSetValues(util::NodeBuilder& builder);

};
