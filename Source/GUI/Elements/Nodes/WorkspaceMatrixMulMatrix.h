#pragma once
#include "WorkspaceMatrix4x4.h"

struct WorkspaceMatrixMulMatrixArgs
{
	WorkspaceLevelOfDetail levelOfDetail = WorkspaceLevelOfDetail::Full;
	std::string headerLabel = "default Matrix Mul Matrix header";
	std::string nodeLabel = "MatrixMulMatrix";
	Ptr<Core::NodeBase> nodebase = Builder::createNode<ENodeType::MatrixMulMatrix>();
};

class WorkspaceMatrixMulMatrix : public WorkspaceMatrix4x4
{
public:
	WorkspaceMatrixMulMatrix(ImTextureID headerBackground, WorkspaceMatrixMulMatrixArgs const& args);
	WorkspaceMatrixMulMatrix(ImTextureID headerBackground, std::string headerLabel = "MatrixMulMatrix", std::string nodeLabel = "MatrixMulMatrix");

	void drawDataSetValues(util::NodeBuilder& builder);

};