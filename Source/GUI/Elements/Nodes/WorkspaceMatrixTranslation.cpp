#include "WorkspaceMatrixTranslation.h"

WorkspaceMatrixTranslation::WorkspaceMatrixTranslation(ImTextureID headerBackground, std::string headerLabel = "default MatrixTranslation header")
    : WorkspaceMatrix4x4(headerBackground, headerLabel)
{
}

void WorkspaceMatrixTranslation::drawNode(util::NodeBuilder& builder, Core::Pin* newLinkPin)
{
	builder.Begin(Id);

	/* \todo JH here will be switch between different scale of node (all data, just three values, just label etc...) */
    /* \todo JH now its same as matrix4x4 with all cells free -> update it to different scale-possibilities - add other drawData functions - like drawJustLabel etc... */
	drawHeader(builder);
	drawInputs(builder, newLinkPin);
	drawData(builder);
	drawOutputs(builder, newLinkPin);

	builder.End();
}

