#pragma once
#include "WorkspaceMatrix4x4.h"

struct WorkspaceMatrixInversionArgs
{
	WorkspaceLevelOfDetail levelOfDetail = WorkspaceLevelOfDetail::Full;
	std::string headerLabel = "default Inversion header";
	std::string nodeLabel = "Inversion";
	Ptr<Core::NodeBase> nodebase = Core::Builder::createNode<ENodeType::Inversion>();
};

class WorkspaceMatrixInversion : public WorkspaceMatrix4x4
{
public:
	WorkspaceMatrixInversion(ImTextureID headerBackground, WorkspaceMatrixInversionArgs const& args);
	WorkspaceMatrixInversion(ImTextureID headerBackground, std::string headerLabel = "Inversion", std::string nodeLabel = "Inversion");

	void drawDataSetValues(util::NodeBuilder& builder);
};