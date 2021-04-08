#pragma once
#include "WorkspaceNodeWithCoreData.h"

struct WorkspaceFloatArgs
{
    WorkspaceViewScale viewScale = WorkspaceViewScale::Full;
    std::string headerLabel = "default Float header";
    std::string nodeLabel = "Float";
    Ptr<Core::NodeBase> nodebase = Builder::createNode<ENodeType::Float>();
};

/*! \class WorkspaceFloat virtual class for all Nodes with Float Core data*/
class WorkspaceFloat : public WorkspaceNodeWithCoreData
{
public:
    WorkspaceFloat(ImTextureID headerBackground, WorkspaceFloatArgs const& args);
    WorkspaceFloat(ImTextureID headerBackground, Ptr<Core::NodeBase> nodebase, std::string headerLabel, std::string nodeLabel);

    virtual void drawData(util::NodeBuilder& builder);
    virtual void drawDataFull(util::NodeBuilder& builder);
};
