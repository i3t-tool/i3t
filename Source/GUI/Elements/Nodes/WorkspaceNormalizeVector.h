#pragma once
#include "WorkspaceVector4.h"

struct WorkspaceNormalizeVectorArgs
{
    WorkspaceViewScale viewScale = WorkspaceViewScale::Full;
    std::string headerLabel = "default NormalizeVector header";
    std::string nodeLabel = "default NormalizeVector label";
    Ptr<Core::NodeBase> nodebase = Core::Builder::createNode<ENodeType::NormalizeVector>();
};

class WorkspaceNormalizeVector : public WorkspaceVector4
{
public:
	WorkspaceNormalizeVector(ImTextureID headerBackground, WorkspaceNormalizeVectorArgs const& args);
    WorkspaceNormalizeVector(ImTextureID headerBackground, std::string headerLabel = "NormVector", std::string nodeLabel = "NormVector");

	void drawData(util::NodeBuilder& builder);

};
