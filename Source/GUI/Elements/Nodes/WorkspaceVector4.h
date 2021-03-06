#pragma once
#include "WorkspaceNodeWithCoreData.h"

/*! \class WorkspaceMatrix4x4 virtual class for all Nodes with Matrix4x4 Core data*/
class WorkspaceVector4 : public WorkspaceNodeWithCoreData
{
public:
	WorkspaceVector4(ImTextureID headerBackground, std::string headerLabel, Ptr<Core::NodeBase> nodebase);

	virtual void drawDataFull(util::NodeBuilder& builder);
};

