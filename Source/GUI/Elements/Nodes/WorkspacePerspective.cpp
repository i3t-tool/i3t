//
// Created by Sofie on 30.04.2021.
//

#include "WorkspacePerspective.h"

WorkspacePerspective::WorkspacePerspective()
    :   WorkspaceMatrix4x4(Core::Builder::createTransform<Core::PerspectiveProj>())
{}


bool WorkspacePerspective::drawDataSetValues(DIWNE::Diwne &diwne)
{
  const Core::Transform::DataMap& coreMap = m_nodebase->getDataMapRef();
  Ptr<Core::PerspectiveProj> nodebase = m_nodebase->as<Core::PerspectiveProj>();
  return drawDataSetValues_builder(diwne,
                            {   "FOW",
                                "Aspect",
                                "ZNear",
                                "ZFar" },
                            {   [nodebase](){return nodebase->getFOW();},
                                [nodebase](){return nodebase->getAspect();},
                                [nodebase](){return nodebase->getZNear();},
                                [nodebase](){return nodebase->getZFar();} },
                            {   [nodebase](float v){return nodebase->setFOW(v);},
                                [nodebase](float v){return nodebase->setAspect(v);},
                                [nodebase](float v){return nodebase->setZNear(v);},
                                [nodebase](float v){return nodebase->setZFar(v);} },
                            {   1, /* \todo JH some better way how determine what element from DataMap should be used? */
                                1,
                                1,
                                1 }
  );
}
