#pragma once
#include "WorkspaceMatrix4x4.h"

class WorkspaceMatrixScale : public WorkspaceMatrix4x4
{
public:
	WorkspaceMatrixScale(ImTextureID headerBackground, std::string headerLabel = "default MatrixScale header", WorkspaceViewScale viewScale = WorkspaceViewScale::Full);

	void drawData(util::NodeBuilder& builder);

};


