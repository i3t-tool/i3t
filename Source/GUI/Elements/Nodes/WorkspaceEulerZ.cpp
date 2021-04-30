//
// Created by Sofie on 29.04.2021.
//

#include "WorkspaceEulerZ.h"

WorkspaceEulerZ::WorkspaceEulerZ(ImTextureID headerBackground, WorkspaceEulerZArgs const& args)
    : WorkspaceMatrix4x4(headerBackground, {.levelOfDetail=args.levelOfDetail, .headerLabel=args.headerLabel, .nodeLabel=args.nodeLabel, .nodebase=Core::Builder::createTransform<Core::Translation>() })
{}

WorkspaceEulerZ::WorkspaceEulerZ(ImTextureID headerBackground, std::string headerLabel, std::string nodeLabel)
    : WorkspaceMatrix4x4(headerBackground, Core::Builder::createTransform<Core::EulerRotZ>(), headerLabel, nodeLabel)
{}

void WorkspaceEulerZ::drawDataSetValues(util::NodeBuilder& builder)
{
  const Core::Transform::DataMap& coreMap = m_nodebase->getDataMapRef();
  drawDataSetValues_builder(builder,
                            { "angle" },
                            { [this]() {return m_nodebase->as<Core::EulerRotZ>()->getAngle(); } },
                            { [this](float v) {return m_nodebase->as<Core::EulerRotZ>()->setValue(v); } },
                            {1}
  );
}