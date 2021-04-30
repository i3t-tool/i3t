//
// Created by Sofie on 30.04.2021.
//

#include "WorkspacePerspective.h"

WorkspacePerspective::WorkspacePerspective(ImTextureID headerBackground, WorkspacePerspectiveArgs const& args)
    : WorkspaceMatrix4x4(headerBackground, {.levelOfDetail=args.levelOfDetail, .headerLabel=args.headerLabel, .nodeLabel=args.nodeLabel, .nodebase=Core::Builder::createTransform<Core::Translation>() })
{}

WorkspacePerspective::WorkspacePerspective(ImTextureID headerBackground, std::string headerLabel, std::string nodeLabel)
    : WorkspaceMatrix4x4(headerBackground, Core::Builder::createTransform<Core::PerspectiveProj>(), headerLabel, nodeLabel)
{}

void WorkspacePerspective::drawDataSetValues(util::NodeBuilder& builder)
{
  const Core::Transform::DataMap& coreMap = m_nodebase->getDataMapRef();
  drawDataSetValues_builder(builder,
                            {   "FOW",
                                "Aspect",
                                "ZNear",
                                "ZFar" },
                            {   [this](){return m_nodebase->as<Core::PerspectiveProj>()->getFOW();},
                                [this](){return m_nodebase->as<Core::PerspectiveProj>()->getAspect();},
                                [this](){return m_nodebase->as<Core::PerspectiveProj>()->getZNear();},
                                [this](){return m_nodebase->as<Core::PerspectiveProj>()->getZFar();} },
                            {   [this](float v){return m_nodebase->as<Core::PerspectiveProj>()->setFOW(v);},
                                [this](float v){return m_nodebase->as<Core::PerspectiveProj>()->setAspect(v);},
                                [this](float v){return m_nodebase->as<Core::PerspectiveProj>()->setZNear(v);},
                                [this](float v){return m_nodebase->as<Core::PerspectiveProj>()->setZFar(v);} },
                            {   1, /* \todo JH some better way how determine what element from DataMap should be used? */
                                1,
	                              1,
                                1 }
  );
}
