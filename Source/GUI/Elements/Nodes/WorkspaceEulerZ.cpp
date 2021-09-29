//
// Created by Sofie on 29.04.2021.
//

#include "WorkspaceEulerZ.h"

WorkspaceEulerZ::WorkspaceEulerZ()
    :   WorkspaceMatrix4x4(Core::Builder::createTransform<Core::EulerRotZ>())
{}


bool WorkspaceEulerZ::drawDataSetValues(DIWNE::Diwne &diwne)
{
  const Core::Transform::DataMap& coreMap = m_nodebase->getDataMapRef();
  Ptr<Core::EulerRotZ> nodebase = m_nodebase->as<Core::EulerRotZ>();

  return drawDataSetValues_builder(diwne,
                            { "angle" },
                            { [nodebase]() {return nodebase->getAngle(); } },
                            { [nodebase](float v) {return nodebase->setValue(v); } },
                            {1} /* \todo JH use coreMap */
  );
}
