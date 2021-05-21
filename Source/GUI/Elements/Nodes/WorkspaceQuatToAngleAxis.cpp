//
// Created by Sofie on 05.05.2021.
//

#include "WorkspaceQuatToAngleAxis.h"

WorkspaceQuatToAngleAxis::WorkspaceQuatToAngleAxis(ImTextureID headerBackground, WorkspaceQuatToAngleAxisArgs const& args)
    : WorkspaceFloatVec3(headerBackground, {.levelOfDetail = args.levelOfDetail, .headerLabel = args.headerLabel, .nodeLabel = args.nodeLabel, /*.nodebase = args.nodebase*/ })
{}

WorkspaceQuatToAngleAxis::WorkspaceQuatToAngleAxis(ImTextureID headerBackground, std::string headerLabel, std::string nodeLabel)
    : WorkspaceFloatVec3(headerBackground, Core::Builder::createNode<ENodeType::QuatToAngleAxis>(), headerLabel, nodeLabel)
{}

void WorkspaceQuatToAngleAxis::drawDataSetValues(util::NodeBuilder& builder)
{
  /* \todo JH - is something to do here? function have to exist anyway... */
  drawDataFull(builder, 0);
}

bool WorkspaceQuatToAngleAxis::isQuatToAngleAxis(){
	return true;
}


