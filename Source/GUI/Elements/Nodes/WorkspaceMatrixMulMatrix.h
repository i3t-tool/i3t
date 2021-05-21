#pragma once
#include "WorkspaceMatrix4x4.h"

struct WorkspaceMatrixMulMatrixArgs
{
	WorkspaceLevelOfDetail levelOfDetail = WorkspaceLevelOfDetail::Full;
	std::string headerLabel = "default Matrix Mul Matrix header";
	std::string nodeLabel = "mat * mat";
	Ptr<Core::NodeBase> nodebase = Core::Builder::createNode<ENodeType::MatrixMulMatrix>();
};

class WorkspaceMatrixMulMatrix : public WorkspaceMatrix4x4
{
public:
	WorkspaceMatrixMulMatrix(ImTextureID headerBackground, WorkspaceMatrixMulMatrixArgs const& args);
	WorkspaceMatrixMulMatrix(ImTextureID headerBackground, std::string headerLabel = "mat * mat", std::string nodeLabel = "mat * mat");

	void drawDataSetValues(util::NodeBuilder& builder);

};
