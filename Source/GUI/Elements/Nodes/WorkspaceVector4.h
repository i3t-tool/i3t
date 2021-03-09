#pragma once
#include "WorkspaceNodeWithCoreData.h"

/*! \class WorkspaceMatrix4x4 virtual class for all Nodes with Matrix4x4 Core data*/
class WorkspaceVector4 : public WorkspaceNodeWithCoreData
{
public:
	WorkspaceVector4(ImTextureID headerBackground,
                        std::string headerLabel = "default Vector4 header",
                        Ptr<Core::NodeBase> nodebase = Builder::createNode<ENodeType::Vector4>(),
                        WorkspaceViewScale viewScale = WorkspaceViewScale::Full);

	virtual void drawDataFull(util::NodeBuilder& builder);
};

