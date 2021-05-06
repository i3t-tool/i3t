//
// Created by Sofie on 29.04.2021.
//

#include "WorkspaceEulerY.h"

WorkspaceEulerY::WorkspaceEulerY(ImTextureID headerBackground, WorkspaceEulerYArgs const& args)
    : WorkspaceMatrix4x4(headerBackground, {.levelOfDetail=args.levelOfDetail, .headerLabel=args.headerLabel, .nodeLabel=args.nodeLabel, .nodebase=Core::Builder::createTransform<Core::Translation>() })
{}

WorkspaceEulerY::WorkspaceEulerY(ImTextureID headerBackground, std::string headerLabel, std::string nodeLabel)
    : WorkspaceMatrix4x4(headerBackground, Core::Builder::createTransform<Core::EulerRotY>(), headerLabel, nodeLabel)
{}

void WorkspaceEulerY::drawDataSetValues(util::NodeBuilder& builder)
{
  const Core::Transform::DataMap& coreMap = m_nodebase->getDataMapRef();
  drawDataSetValues_builder(builder,
                            { "angle" },
                            { [this]() {return m_nodebase->as<Core::EulerRotY>()->getAngle(); } },
                            { [this](float v) {return m_nodebase->as<Core::EulerRotY>()->setValue(v); } },
                            {1}
  );
}