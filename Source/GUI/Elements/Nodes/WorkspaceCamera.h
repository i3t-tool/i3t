#pragma once
#include "WorkspaceNodeWithCoreData.h"
#include "WorkspaceSequence.h"

#include "Core/Nodes/GraphManager.h"


struct WorkspaceCameraArgs
{
    WorkspaceLevelOfDetail levelOfDetail = WorkspaceLevelOfDetail::Full;
    std::string headerLabel = "default Camera header";
    std::string nodeLabel = "Camera";
};

class WorkspaceCamera : public WorkspaceNodeWithCoreData
{
protected:
    ImRect m_dataRect; /* is set in every frame - depend on levelOfDetail and content of inner Sequences */
    Ptr<WorkspaceSequence> m_projection = nullptr;
    Ptr<WorkspaceSequence> m_view = nullptr;
public:
	WorkspaceCamera(ImTextureID headerBackground, WorkspaceCameraArgs const& args);
    WorkspaceCamera(ImTextureID headerBackground, std::string headerLabel = "default Camera header", std::string nodeLabel = "Camera");

    bool isCamera();
    ImVec2 getDataSize();

    Ptr<WorkspaceSequence> const& getProjection() const;
    Ptr<WorkspaceSequence> const& getView() const;

    void drawNode(util::NodeBuilder& builder, Core::Pin* newLinkPin=nullptr, bool withPins=true);
	void drawDataFull(util::NodeBuilder& builder, int index);
    void drawDataSetValues(util::NodeBuilder& builder);

	int maxLenghtOfData();
};


