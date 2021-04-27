#include "WorkspaceMakeEulerX.h"

WorkspaceMakeEulerX::WorkspaceMakeEulerX(ImTextureID headerBackground, WorkspaceMakeEulerXArgs const& args)
    : WorkspaceMatrix4x4(headerBackground, { .levelOfDetail = args.levelOfDetail, .headerLabel = args.headerLabel, .nodeLabel = args.nodeLabel, .nodebase = args.nodebase })
{}

WorkspaceMakeEulerX::WorkspaceMakeEulerX(ImTextureID headerBackground, std::string headerLabel, std::string nodeLabel)
    : WorkspaceMatrix4x4(headerBackground, Core::Builder::createNode<ENodeType::MakeEulerX>(), headerLabel, nodeLabel)
{}

void WorkspaceMakeEulerX::drawDataSetValues(util::NodeBuilder& builder)
{
    const Core::Transform::DataMap& coreMap = m_nodebase->getDataMapRef();
    drawDataSetValues_builder(builder,
        //SS todo
        { "11", "12", "21", "22" },
        //SS better get?
        { [this]() {return get11(); }, [this]() {return get12(); }, [this]() {return get21(); } , [this]() {return get22(); } },

        { [this](float v) {return m_nodebase->as<Core::EulerRotX>()->setValue(v); } }, /* \todo JH here have to be same number of setters as getters */


        {   coreMap[1 * 4 + 1], /* \todo JH some better way how determine what element from DataMap should be used? */
            coreMap[1 * 4 + 2],
            coreMap[2 * 4 + 1],
            coreMap[2 * 4 + 2]
        });
}

float WorkspaceMakeEulerX::get11()
{
    return m_nodebase->getData().getMat4()[1][1];
}

float WorkspaceMakeEulerX::get12()
{
    return m_nodebase->getData().getMat4()[1][2];
}

float WorkspaceMakeEulerX::get21()
{
    return m_nodebase->getData().getMat4()[2][1];
}

float WorkspaceMakeEulerX::get22()
{
    return m_nodebase->getData().getMat4()[2][2];
}
