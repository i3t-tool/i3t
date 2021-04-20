#pragma once
#include "WorkspaceVector4.h"

struct WorkspaceVectorFreeArgs
{
    WorkspaceLevelOfDetail levelOfDetail = WorkspaceLevelOfDetail::Full;
    std::string headerLabel = "default FreeVector header";
    std::string nodeLabel = "default FreeVector label";
    Ptr<Core::NodeBase> nodebase = Core::Builder::createNode<ENodeType::Vector4>();
};

class WorkspaceVectorFree : public WorkspaceVector4
{
public:
	WorkspaceVectorFree(ImTextureID headerBackground, WorkspaceVectorFreeArgs const& args);
    WorkspaceVectorFree(ImTextureID headerBackground, std::string headerLabel = "Free", std::string nodeLabel = "Free");

	void drawDataSetValues(util::NodeBuilder& builder);

};

