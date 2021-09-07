//
// Created by Sofie on 30.04.2021.
//

#include "WorkspaceFrustum.h"
WorkspaceFrustum::WorkspaceFrustum()
    :   WorkspaceMatrix4x4(Core::Builder::createTransform<Core::Frustum>())
{}

bool WorkspaceFrustum::drawDataSetValues(DIWNE::Diwne &diwne)
{
  const Core::Transform::DataMap& coreMap = m_nodebase->getDataMapRef();
  Ptr<Core::Frustum> nodebase = m_nodebase->as<Core::Frustum>();

  return drawDataSetValues_builder(diwne,
                            {   "left",
                                "right",
                                "bottom",
                                "top",
                                "near",
                                "far"},
                            {   [nodebase](){return nodebase->getLeft();},
                                [nodebase](){return nodebase->getRight();},
                                [nodebase](){return nodebase->getBottom();},
                                [nodebase](){return nodebase->getTop();},
                                [nodebase](){return nodebase->getNear();},
                                [nodebase](){return nodebase->getFar();} },
                            {   [nodebase](float v){return nodebase->setLeft(v);},
                                [nodebase](float v){return nodebase->setRight(v);},
                                [nodebase](float v){return nodebase->setBottom(v);},
                                [nodebase](float v){return nodebase->setTop(v);},
                                [nodebase](float v){return nodebase->setNear(v);},
                                [nodebase](float v){return nodebase->setFar(v);}},
                            {   1, /* \todo JH use CoreMap */
                                1,
                                1,
                                1,
                                1,
                                1}
  );

}
