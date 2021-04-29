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
                            { "cos(a)", "-sin(a)", "sin(a)", "cos(a)" },
                            { [this]() {return get11(); }, [this]() {return get12(); }, [this]() {return get21(); } , [this]() {return get22(); } },

                            { [this](float v) {return m_nodebase->as<Core::EulerRotX>()->setValue(v); } }, /* \todo JH here have to be same number of setters as getters */

	                          {   coreMap[1 * 4 + 1], /* \todo JH some better way how determine what element from DataMap should be used? */
                                coreMap[1 * 4 + 2],
                                coreMap[2 * 4 + 1],
                                coreMap[2 * 4 + 2]
                            });
}

float WorkspaceEulerX::get11()
{
  return m_nodebase->getData().getMat4()[1][1];
}

float WorkspaceEulerX::get12()
{
  return m_nodebase->getData().getMat4()[1][2];
}

float WorkspaceEulerX::get21()
{
  return m_nodebase->getData().getMat4()[2][1];
}

float WorkspaceEulerX::get22()
{
  return m_nodebase->getData().getMat4()[2][2];
}
