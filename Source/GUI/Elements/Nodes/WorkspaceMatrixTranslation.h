#pragma once
#include "WorkspaceMatrix4x4.h"

class WorkspaceMatrixTranslation : public WorkspaceMatrix4x4
{
public:
	WorkspaceMatrixTranslation(ImTextureID headerBackground, std::string headerLabel);

	void drawNode(util::NodeBuilder& builder, Core::Pin* newLinkPin);

};

