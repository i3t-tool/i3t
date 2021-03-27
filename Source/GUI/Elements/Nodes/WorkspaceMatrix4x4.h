#pragma once
#include "WorkspaceNodeWithCoreData.h"

struct WorkspaceMatrix4x4Args
{
    WorkspaceLevelOfDetail levelOfDetail = WorkspaceLevelOfDetail::Full;
    std::string headerLabel = "default Matrix4x4 header";
    std::string nodeLabel = "Free";
    Ptr<Core::NodeBase> nodebase = Builder::createNode<ENodeType::Matrix>();
};

/*! \class WorkspaceMatrix4x4 virtual class for all Nodes with Matrix4x4 Core data*/
class WorkspaceMatrix4x4 : public WorkspaceNodeWithCoreData
{
public:
	WorkspaceMatrix4x4( ImTextureID headerBackground, WorkspaceMatrix4x4Args const& args);
    WorkspaceMatrix4x4(ImTextureID headerBackground, Ptr<Core::NodeBase> nodebase, std::string headerLabel, std::string nodeLabel);


	virtual void drawData(util::NodeBuilder& builder);

	virtual void drawDataFull(util::NodeBuilder& builder);
};
