//
// Created by Sofie on 18.05.2021.
//
#pragma once
#include "WorkspaceFloat.h"

struct WorkspaceScreenArgs
{
	WorkspaceLevelOfDetail levelOfDetail = WorkspaceLevelOfDetail::Full;
	std::string headerLabel = "default Screen header";
	std::string nodeLabel = "Screen";
	Ptr<Core::NodeBase> nodebase = Core::Builder::createNode<ENodeType::Screen>();
};

class WorkspaceScreen : public WorkspaceFloat
{
public:
	WorkspaceScreen(ImTextureID headerBackground, WorkspaceScreenArgs const& args);
	WorkspaceScreen(ImTextureID headerBackground, std::string headerLabel = "Screen", std::string nodeLabel = "Screen");

	void drawDataSetValues(util::NodeBuilder& builder);
	void drawDataFull(util::NodeBuilder& builder, int index);
};
