#include "WorkspaceMatrixTranslation.h"

WorkspaceMatrixTranslation::WorkspaceMatrixTranslation(ImTextureID headerBackground, WorkspaceMatrixTranslationArgs const& args)
    : WorkspaceMatrix4x4(headerBackground, {.levelOfDetail=args.levelOfDetail, .headerLabel=args.headerLabel, .nodeLabel=args.nodeLabel, .nodebase=Core::Builder::createTransform<Core::Translation>() })
{}

WorkspaceMatrixTranslation::WorkspaceMatrixTranslation(ImTextureID headerBackground, std::string headerLabel, std::string nodeLabel)
    : WorkspaceMatrix4x4(headerBackground, Core::Builder::createTransform<Core::Translation>(), headerLabel, nodeLabel)
{}

void WorkspaceMatrixTranslation::drawDataSetValues(util::NodeBuilder& builder)
{
    const Core::Transform::DataMap& coreMap = m_nodebase->getDataMapRef();
    drawDataSetValues_builder(builder,
                                {   "x",
                                    "y",
                                    "z" },
                                {   [this](){return m_nodebase->as<Core::Translation>()->getX();},
                                    [this](){return m_nodebase->as<Core::Translation>()->getY();},
                                    [this](){return m_nodebase->as<Core::Translation>()->getZ();} },
                                {   [this](float v){return m_nodebase->as<Core::Translation>()->setX(v);},
                                    [this](float v){return m_nodebase->as<Core::Translation>()->setY(v);},
                                    [this](float v){return m_nodebase->as<Core::Translation>()->setZ(v);} },
                                {   coreMap[3*4+0], /* \todo JH some better way how determine what element from DataMap should be used? */
                                    coreMap[3*4+1],
                                    coreMap[3*4+2] }
                            );
}
