#pragma once
#include "WorkspaceMatrix4x4.h"

struct WorkspaceMatrixMulMatrixArgs
{
	WorkspaceViewScale viewScale = WorkspaceViewScale::Full;
	std::string headerLabel = "default Matrix Mul Matrix header";
	std::string nodeLabel = "MatrixMulMatrix";
	Ptr<Core::NodeBase> nodebase = Builder::createNode<ENodeType::MatrixMulMatrix>();
};

class WorkspaceMatrixMulMatrix : public WorkspaceMatrix4x4
{
public:
	WorkspaceMatrixMulMatrix(ImTextureID headerBackground, WorkspaceMatrixMulMatrixArgs const& args);

	void drawData(util::NodeBuilder& builder);

	void drawDataSetValues(util::NodeBuilder& builder);
	void drawDataLabel(util::NodeBuilder& builder);

};