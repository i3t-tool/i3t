#pragma once
#include "WorkspaceVector4.h"

struct WorkspaceNormalizeVectorArgs
{
    WorkspaceLevelOfDetail levelOfDetail = WorkspaceLevelOfDetail::Full;
    std::string headerLabel = "default Normalize vec4 header";
    std::string nodeLabel = "default Normalize vec4 label";
    Ptr<Core::NodeBase> nodebase = Core::Builder::createNode<ENodeType::NormalizeVector>();
};

class WorkspaceNormalizeVector : public WorkspaceVector4
{
public:
	WorkspaceNormalizeVector(ImTextureID headerBackground, WorkspaceNormalizeVectorArgs const& args);
    WorkspaceNormalizeVector(ImTextureID headerBackground, std::string headerLabel = "Normalize vec4", std::string nodeLabel = "Normalize vec4");

	void drawDataSetValues(util::NodeBuilder& builder);

};
