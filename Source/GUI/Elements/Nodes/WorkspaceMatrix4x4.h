#pragma once
#include "WorkspaceNodeWithCoreData.h"
/*! \class WorkspaceMatrix4x4 virtual class for all Nodes with Matrix4x4 Core data*/
class WorkspaceMatrix4x4 : public WorkspaceNodeWithCoreData

{
public:
	WorkspaceMatrix4x4(ImTextureID headerBackground, std::string headerLabel);

	void drawData(util::NodeBuilder& builder);
};
