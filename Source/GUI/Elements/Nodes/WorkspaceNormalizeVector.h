#pragma once
#include "WorkspaceVector4.h"

struct WorkspaceNormalizeVectorArgs
{
    WorkspaceLevelOfDetail levelOfDetail = WorkspaceLevelOfDetail::Full;
    std::string headerLabel = "default NormalizeVector header";
    std::string nodeLabel = "default NormalizeVector label";
    Ptr<Core::NodeBase> nodebase = Builder::createNode<ENodeType::NormalizeVector>();
};

class WorkspaceNormalizeVector : public WorkspaceVector4
{
public:
	WorkspaceNormalizeVector(ImTextureID headerBackground, WorkspaceNormalizeVectorArgs const& args);
    WorkspaceNormalizeVector(ImTextureID headerBackground, std::string headerLabel = "NormVector", std::string nodeLabel = "NormVector");

	void drawDataSetValues(util::NodeBuilder& builder);

};
