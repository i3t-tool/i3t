#include "WorkspaceMakePerspective.h"

WorkspaceMakePerspective::WorkspaceMakePerspective(ImTextureID headerBackground, WorkspaceMakePerspectiveArgs const& args)
    : WorkspaceMatrix4x4(headerBackground, { .levelOfDetail = args.levelOfDetail, .headerLabel = args.headerLabel, .nodeLabel = args.nodeLabel, .nodebase = args.nodebase })
{}

WorkspaceMakePerspective::WorkspaceMakePerspective(ImTextureID headerBackground, std::string headerLabel, std::string nodeLabel)
    : WorkspaceMatrix4x4(headerBackground, Builder::createNode<ENodeType::MakePerspective>(), headerLabel, nodeLabel)
{}

void WorkspaceMakePerspective::drawDataSetValues(util::NodeBuilder& builder)
{
    drawDataFull(builder);
}