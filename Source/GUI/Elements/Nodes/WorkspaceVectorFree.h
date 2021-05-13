#pragma once
#include "WorkspaceVector4.h"

struct WorkspaceVectorFreeArgs
{
    WorkspaceLevelOfDetail levelOfDetail = WorkspaceLevelOfDetail::Full;
    std::string headerLabel = "default Vector header";
    std::string nodeLabel = "default Vector label";
    Ptr<Core::NodeBase> nodebase = Core::Builder::createNode<ENodeType::Vector4>();
};

class WorkspaceVectorFree : public WorkspaceVector4
{
public:
	WorkspaceVectorFree(ImTextureID headerBackground, WorkspaceVectorFreeArgs const& args);
    WorkspaceVectorFree(ImTextureID headerBackground, std::string headerLabel = "Vector 4", std::string nodeLabel = "Vector 4");

	void drawDataSetValues(util::NodeBuilder& builder);

};

