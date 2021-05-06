//
// Created by Sofie on 29.04.2021.
//

#include "WorkspaceEulerX.h"

WorkspaceEulerX::WorkspaceEulerX(ImTextureID headerBackground, WorkspaceEulerXArgs const& args)
    : WorkspaceMatrix4x4(headerBackground, {.levelOfDetail=args.levelOfDetail, .headerLabel=args.headerLabel, .nodeLabel=args.nodeLabel, .nodebase=Core::Builder::createTransform<Core::Translation>() })
{}

WorkspaceEulerX::WorkspaceEulerX(ImTextureID headerBackground, std::string headerLabel, std::string nodeLabel)
    : WorkspaceMatrix4x4(headerBackground, Core::Builder::createTransform<Core::EulerRotX>(), headerLabel, nodeLabel)
{}

void WorkspaceEulerX::drawDataSetValues(util::NodeBuilder& builder)
{
  const Core::Transform::DataMap& coreMap = m_nodebase->getDataMapRef();
  drawDataSetValues_builder(builder,
                            { "angle" },
                            { [this]() {return m_nodebase->as<Core::EulerRotX>()->getAngle(); } },
                            { [this](float v) {return m_nodebase->as<Core::EulerRotX>()->setValue(v); } },
	                          {1}
                            );
}