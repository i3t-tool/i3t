#pragma once
#include "WorkspaceMatrix4x4.h"

struct WorkspaceMakeAxisAngleArgs
{
    WorkspaceLevelOfDetail levelOfDetail = WorkspaceLevelOfDetail::Full;
    std::string headerLabel = "default Rotate header";
    std::string nodeLabel = "Rotate";
    Ptr<Core::NodeBase> nodebase = Core::Builder::createNode<ENodeType::MakeAxisAngle>();
};

class WorkspaceMakeAxisAngle : public WorkspaceMatrix4x4
{
public:
	WorkspaceMakeAxisAngle(ImTextureID headerBackground, WorkspaceMakeAxisAngleArgs const& args);
	WorkspaceMakeAxisAngle(ImTextureID headerBackground, std::string headerLabel = "Rotate", std::string nodeLabel = "Rotate");

    void drawDataSetValues(util::NodeBuilder& builder);

};
