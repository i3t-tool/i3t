#pragma once
#include "WorkspaceVector4.h"

struct WorkspaceVectorFreeArgs
{
    WorkspaceLevelOfDetail levelOfDetail = WorkspaceLevelOfDetail::Full;
    std::string headerLabel = "default FreeVector header";
    std::string nodeLabel = "default FreeVector label";
    Ptr<Core::NodeBase> nodebase = Builder::createNode<ENodeType::Vector4>();
};

class WorkspaceVectorFree : public WorkspaceVector4
{
public:
	WorkspaceVectorFree(ImTextureID headerBackground, WorkspaceVectorFreeArgs const& args);
    WorkspaceVectorFree(ImTextureID headerBackground, std::string headerLabel = "Vector4", std::string nodeLabel = "Vector4");

	void drawDataSetValues(util::NodeBuilder& builder);

};

