//
// Created by Sofie on 29.04.2021.
//

#include "WorkspaceEulerX.h"
WorkspaceEulerX::WorkspaceEulerX()
    :   WorkspaceMatrix4x4(Core::Builder::createTransform<Core::EulerRotX>())
{}

bool WorkspaceEulerX::drawDataSetValues(DIWNE::Diwne &diwne)
{
  const Core::Transform::DataMap& coreMap = m_nodebase->getDataMapRef();
  Ptr<Core::EulerRotX> nodebase = m_nodebase->as<Core::EulerRotX>();

  return drawDataSetValues_builder(diwne,
                            { "angle" },
                            { [nodebase]() {return nodebase->getAngle(); } },
                            { [nodebase](float v) {return nodebase->setValue(v); } },
                            {1} /* \todo JH use coreMap */
                            );
}
