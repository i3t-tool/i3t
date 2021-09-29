//
// Created by Sofie on 29.04.2021.
//

#include "WorkspaceEulerY.h"

WorkspaceEulerY::WorkspaceEulerY()
    :   WorkspaceMatrix4x4(Core::Builder::createTransform<Core::EulerRotY>())
{}

bool WorkspaceEulerY::drawDataSetValues(DIWNE::Diwne &diwne)
{
  const Core::Transform::DataMap& coreMap = m_nodebase->getDataMapRef();
  Ptr<Core::EulerRotY> nodebase = m_nodebase->as<Core::EulerRotY>();

  return drawDataSetValues_builder(diwne,
                            { "angle" },
                            { [nodebase]() {return nodebase->getAngle(); } },
                            { [nodebase](float v) {return nodebase->setValue(v); } },
                            {1} /* \todo JH use coreMap */
  );
}
