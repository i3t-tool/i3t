//
// Created by Sofie on 30.04.2021.
//

#include "WorkspaceOrtho.h"

WorkspaceOrtho::WorkspaceOrtho(ImTextureID headerBackground, WorkspaceOrthoArgs const& args)
    : WorkspaceMatrix4x4(headerBackground, {.levelOfDetail=args.levelOfDetail, .headerLabel=args.headerLabel, .nodeLabel=args.nodeLabel, .nodebase=Core::Builder::createTransform<Core::Translation>() })
{}

WorkspaceOrtho::WorkspaceOrtho(ImTextureID headerBackground, std::string headerLabel, std::string nodeLabel)
    : WorkspaceMatrix4x4(headerBackground, Core::Builder::createTransform<Core::OrthoProj>(), headerLabel, nodeLabel)
{}

void WorkspaceOrtho::drawDataSetValues(util::NodeBuilder& builder)
{
  const Core::Transform::DataMap& coreMap = m_nodebase->getDataMapRef();
  drawDataSetValues_builder(builder,
                            {   "left",
                                "right",
                                "bottom",
                                "top",
                                "near",
                                "far"},
                            {   [this](){return m_nodebase->as<Core::OrthoProj>()->getLeft();},
                                [this](){return m_nodebase->as<Core::OrthoProj>()->getRight();},
                                [this](){return m_nodebase->as<Core::OrthoProj>()->getBottom();},
                                [this](){return m_nodebase->as<Core::OrthoProj>()->getTop();},
                                [this](){return m_nodebase->as<Core::OrthoProj>()->getNear();},
                                [this](){return m_nodebase->as<Core::OrthoProj>()->getFar();} },
                            {   [this](float v){return m_nodebase->as<Core::OrthoProj>()->setLeft(v);},
                                [this](float v){return m_nodebase->as<Core::OrthoProj>()->setRight(v);},
                                [this](float v){return m_nodebase->as<Core::OrthoProj>()->setBottom(v);},
                                [this](float v){return m_nodebase->as<Core::OrthoProj>()->setTop(v);},
                                [this](float v){return m_nodebase->as<Core::OrthoProj>()->setNear(v);},
                                [this](float v){return m_nodebase->as<Core::OrthoProj>()->setFar(v);}},
                            {   1, /* \todo JH some better way how determine what element from DataMap should be used? */
                                1,
                                1,
                                1,
                                1,
                                1}
  );
}
