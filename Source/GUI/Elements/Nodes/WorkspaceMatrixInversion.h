#pragma once
#include "WorkspaceMatrix4x4.h"

struct WorkspaceMatrixInversionArgs
{
	WorkspaceViewScale viewScale = WorkspaceViewScale::Full;
	std::string headerLabel = "default Inversion header";
	std::string nodeLabel = "Inversion";
	Ptr<Core::NodeBase> nodebase = Builder::createNode<ENodeType::Inversion>();
};

class WorkspaceMatrixInversion : public WorkspaceMatrix4x4
{
public:
	WorkspaceMatrixInversion(ImTextureID headerBackground, WorkspaceMatrixInversionArgs const& args);

	void drawData(util::NodeBuilder& builder);

	void drawDataSetValues(util::NodeBuilder& builder);
	void drawDataLabel(util::NodeBuilder& builder);

};