//
// Created by Sofie on 30.04.2021.
//

#include "WorkspaceFrustrum.h"

WorkspaceFrustum::WorkspaceFrustum(ImTextureID headerBackground, WorkspaceFrustumArgs const& args)
    : WorkspaceMatrix4x4(headerBackground, {.levelOfDetail=args.levelOfDetail, .headerLabel=args.headerLabel, .nodeLabel=args.nodeLabel, .nodebase=Core::Builder::createTransform<Core::Translation>() })
{}

WorkspaceFrustum::WorkspaceFrustum(ImTextureID headerBackground, std::string headerLabel, std::string nodeLabel)
    : WorkspaceMatrix4x4(headerBackground, Core::Builder::createTransform<Core::Translation>(), headerLabel, nodeLabel)
{}

void WorkspaceFrustum::drawDataSetValues(util::NodeBuilder& builder)
{
  const Core::Transform::DataMap& coreMap = m_nodebase->getDataMapRef();
  drawDataSetValues_builder(builder,
                            {   "left",
                                "right",
                                "bottom",
                                "top",
                                "near",
                                "far"},
                            {   [this](){return m_nodebase->as<Core::Frustum>()->getLeft();},
                                [this](){return m_nodebase->as<Core::Frustum>()->getRight();},
                                [this](){return m_nodebase->as<Core::Frustum>()->getBottom();},
                                [this](){return m_nodebase->as<Core::Frustum>()->getTop();},
                                [this](){return m_nodebase->as<Core::Frustum>()->getNear();},
                                [this](){return m_nodebase->as<Core::Frustum>()->getFar();} },
                            {   [this](float v){return m_nodebase->as<Core::Frustum>()->setLeft(v);},
                                [this](float v){return m_nodebase->as<Core::Frustum>()->setRight(v);},
                                [this](float v){return m_nodebase->as<Core::Frustum>()->setBottom(v);},
                                [this](float v){return m_nodebase->as<Core::Frustum>()->setTop(v);},
                                [this](float v){return m_nodebase->as<Core::Frustum>()->setNear(v);},
                                [this](float v){return m_nodebase->as<Core::Frustum>()->setFar(v);}},
                            {   1, /* \todo JH some better way how determine what element from DataMap should be used? */
                                1,
                                1,
                                1,
                                1,
                                1}
  );
}
