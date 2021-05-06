//
// Created by Sofie on 05.05.2021.
//

#include "WorkspaceQuatToFloatVec.h"

WorkspaceQuatToFloatVec::WorkspaceQuatToFloatVec(ImTextureID headerBackground, WorkspaceQuatToFloatVecArgs const& args)
    : WorkspaceFloatVec3(headerBackground, {.levelOfDetail = args.levelOfDetail, .headerLabel = args.headerLabel, .nodeLabel = args.nodeLabel, /*.nodebase = args.nodebase*/ })
{}

WorkspaceQuatToFloatVec::WorkspaceQuatToFloatVec(ImTextureID headerBackground, std::string headerLabel, std::string nodeLabel)
    : WorkspaceFloatVec3(headerBackground, Core::Builder::createNode<ENodeType::QuatToFloatVec>(), headerLabel, nodeLabel)
{}

void WorkspaceQuatToFloatVec::drawDataSetValues(util::NodeBuilder& builder)
{
  /* \todo JH - is something to do here? function have to exist anyway... */
  drawDataFull(builder, 0);
}


