#include "WorkspaceMakeOrtho.h"

WorkspaceMakeOrtho::WorkspaceMakeOrtho(ImTextureID headerBackground, WorkspaceMakeOrthoArgs const& args)
    : WorkspaceMatrix4x4(headerBackground, { .levelOfDetail = args.levelOfDetail, .headerLabel = args.headerLabel, .nodeLabel = args.nodeLabel, .nodebase = args.nodebase })
{}

WorkspaceMakeOrtho::WorkspaceMakeOrtho(ImTextureID headerBackground, std::string headerLabel, std::string nodeLabel)
    : WorkspaceMatrix4x4(headerBackground, Builder::createNode<ENodeType::MakeOrtho>(), headerLabel, nodeLabel)
{}

void WorkspaceMakeOrtho::drawDataSetValues(util::NodeBuilder& builder)
{
    drawDataFull(builder);
}