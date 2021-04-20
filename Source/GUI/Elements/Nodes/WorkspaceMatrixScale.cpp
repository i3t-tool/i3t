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
                                {   "x",
                                    "y",
                                    "z" },
                                {   [this](){return m_nodebase->as<Core::Scale>()->getX();},
                                    [this](){return m_nodebase->as<Core::Scale>()->getY();},
                                    [this](){return m_nodebase->as<Core::Scale>()->getZ();} },
                                {   [this](float v){return m_nodebase->as<Core::Scale>()->setX(v);},
                                    [this](float v){return m_nodebase->as<Core::Scale>()->setY(v);},
                                    [this](float v){return m_nodebase->as<Core::Scale>()->setZ(v);} },
                                {   coreMap[0*4+0], /* \todo JH some better way how determine what element from DataMap should be used? */
                                    coreMap[1*4+1],
                                    coreMap[2*4+2] }
                            );
}
