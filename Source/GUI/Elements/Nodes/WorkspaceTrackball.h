//
// Created by Sofie on 16.05.2021.
//

#pragma once
#include "WorkspaceMatrix4x4.h"

struct WorkspaceTrackballArgs
{
	WorkspaceLevelOfDetail levelOfDetail = WorkspaceLevelOfDetail::Full;
	std::string headerLabel = "default Trackball header";
	std::string nodeLabel = "Trackball";
	//TODO uncomment when MH make trackball in Core
	//Ptr<Core::NodeBase> nodebase = Core::Builder::createNode<ENodeType::Trackball>();
	Ptr<Core::NodeBase> nodebase = Core::Builder::createNode<ENodeType::Matrix>();
};

class WorkspaceTrackball : public WorkspaceMatrix4x4
{
public:
	WorkspaceTrackball(ImTextureID headerBackground, WorkspaceTrackballArgs const& args);
	WorkspaceTrackball(ImTextureID headerBackground, std::string headerLabel = "Trackball", std::string nodeLabel = "Trackball");

	bool isTrackball();

	void drawDataSetValues(util::NodeBuilder& builder);
	void drawDataFull(util::NodeBuilder& builder, int index);
};
