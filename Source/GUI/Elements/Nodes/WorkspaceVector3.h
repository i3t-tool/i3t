#pragma once
#include "WorkspaceNodeWithCoreData.h"

struct WorkspaceVector3Args
{
    WorkspaceLevelOfDetail levelOfDetail = WorkspaceLevelOfDetail::Full;
    std::string headerLabel = "default Vector3 header";
    std::string nodeLabel = "Vec3";
    Ptr<Core::NodeBase> nodebase = Core::Builder::createNode<ENodeType::Vector3ToVector3>();
};

class WorkspaceVector3 : public WorkspaceNodeWithCoreData
{
public:
	WorkspaceVector3( ImTextureID headerBackground, WorkspaceVector3Args const& args);
    WorkspaceVector3(ImTextureID headerBackground, Ptr<Core::NodeBase> nodebase, std::string headerLabel, std::string nodeLabel);

	virtual void drawDataFull(util::NodeBuilder& builder, int index);
	int maxLenghtOfData();
};

