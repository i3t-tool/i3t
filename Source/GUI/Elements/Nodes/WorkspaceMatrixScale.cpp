#include "WorkspaceMatrixScale.h"

WorkspaceMatrixScale::WorkspaceMatrixScale(ImTextureID headerBackground, WorkspaceMatrixScaleArgs const& args)
    : WorkspaceMatrix4x4(headerBackground, {.levelOfDetail=args.levelOfDetail, .headerLabel=args.headerLabel, .nodeLabel=args.nodeLabel, .nodebase=args.nodebase})
{}

WorkspaceMatrixScale::WorkspaceMatrixScale(ImTextureID headerBackground, std::string headerLabel, std::string nodeLabel)
    : WorkspaceMatrix4x4(headerBackground, Core::Builder::createTransform<Core::Scale>(), headerLabel, nodeLabel){
}

void WorkspaceMatrixScale::drawDataSetValues(util::NodeBuilder& builder)
{
    const Core::Transform::DataMap& coreMap = m_nodebase->getDataMapRef();
    drawDataSetValues_builder(builder,
                                {   "scale" },
                                {   [this](){return m_nodebase->as<Core::Scale>()->getX();} },
                                {   [this](float v){return m_nodebase->as<Core::Scale>()->setX(v);}},
                                {   coreMap[0*4+0] /* \todo JH some better way how determine what element from DataMap should be used? */}
                            );
}
