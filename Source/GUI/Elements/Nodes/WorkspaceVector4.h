#pragma once
#include "WorkspaceNodeWithCoreData.h"

struct WorkspaceVector4Args
{
    WorkspaceLevelOfDetail levelOfDetail = WorkspaceLevelOfDetail::Full;
    std::string headerLabel = "default Vector4 header";
    std::string nodeLabel = "Vec4";
    Ptr<Core::NodeBase> nodebase = Builder::createNode<ENodeType::Vector4>();
};

/*! \class WorkspaceMatrix4x4 virtual class for all Nodes with Matrix4x4 Core data*/
class WorkspaceVector4 : public WorkspaceNodeWithCoreData
{
public:
	WorkspaceVector4( ImTextureID headerBackground, WorkspaceVector4Args const& args);
    WorkspaceVector4(ImTextureID headerBackground, Ptr<Core::NodeBase> nodebase, std::string headerLabel, std::string nodeLabel);

	virtual void drawDataFull(util::NodeBuilder& builder);
};
