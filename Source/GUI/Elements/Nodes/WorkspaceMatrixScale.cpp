#include "WorkspaceMatrixScale.h"

WorkspaceMatrixScale::WorkspaceMatrixScale(ImTextureID headerBackground, WorkspaceMatrixScaleArgs const& args)
    : WorkspaceMatrix4x4(headerBackground, {.levelOfDetail=args.levelOfDetail, .headerLabel=args.headerLabel, .nodeLabel=args.nodeLabel, .nodebase=args.nodebase})
{}

WorkspaceMatrixScale::WorkspaceMatrixScale(ImTextureID headerBackground, std::string headerLabel, std::string nodeLabel)
    : WorkspaceMatrix4x4(headerBackground, Builder::createTransform<Core::Scale>(), headerLabel, nodeLabel){
}

void WorkspaceMatrixScale::drawData(util::NodeBuilder& builder)
{
	drawDataFull(builder); /* \todo JH here will be switch between different scale of view */
}
