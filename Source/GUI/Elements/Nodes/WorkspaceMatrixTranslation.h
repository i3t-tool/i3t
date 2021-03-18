#pragma once
#include "WorkspaceMatrix4x4.h"

class WorkspaceMatrixTranslation : public WorkspaceMatrix4x4
{
public:
	WorkspaceMatrixTranslation(ImTextureID headerBackground,
	                           std::string headerLabel = "default MatrixTranslation header");

	void drawData(util::NodeBuilder& builder);
};
