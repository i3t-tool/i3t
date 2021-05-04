//
// Created by Sofie on 04.05.2021.
//

#include "WorkspaceQuatToEuler.h"

WorkspaceQuatToEuler::WorkspaceQuatToEuler(ImTextureID headerBackground, WorkspaceQuatToEulerArgs const& args)
    : WorkspaceFloat(headerBackground, {.levelOfDetail = args.levelOfDetail, .headerLabel = args.headerLabel, .nodeLabel = args.nodeLabel, .nodebase = args.nodebase })
{}

WorkspaceQuatToEuler::WorkspaceQuatToEuler(ImTextureID headerBackground, std::string headerLabel, std::string nodeLabel)
    : WorkspaceFloat(headerBackground, Core::Builder::createNode<ENodeType::QuatToEuler>(), headerLabel, nodeLabel)
{}

void WorkspaceQuatToEuler::drawDataSetValues(util::NodeBuilder& builder)
{
	drawDataFull(builder);
}
