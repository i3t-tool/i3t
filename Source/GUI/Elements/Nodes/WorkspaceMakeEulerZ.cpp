#include "WorkspaceMakeEulerZ.h"

WorkspaceMakeEulerZ::WorkspaceMakeEulerZ(ImTextureID headerBackground, WorkspaceMakeEulerZArgs const& args)
    : WorkspaceMatrix4x4(headerBackground, { .levelOfDetail = args.levelOfDetail, .headerLabel = args.headerLabel, .nodeLabel = args.nodeLabel, .nodebase = args.nodebase })
{}

WorkspaceMakeEulerZ::WorkspaceMakeEulerZ(ImTextureID headerBackground, std::string headerLabel, std::string nodeLabel)
    : WorkspaceMatrix4x4(headerBackground, Core::Builder::createNode<ENodeType::MakeEulerY>(), headerLabel, nodeLabel)
{}

void WorkspaceMakeEulerZ::drawDataSetValues(util::NodeBuilder& builder)
{
    const Core::Transform::DataMap& coreMap = m_nodebase->getDataMapRef();
    drawDataSetValues_builder(builder,
        //SS todo
        { "00", "01", "10", "11" },
        { [this]() {return get00(); }, [this]() {return get01(); }, [this]() {return get10(); } , [this]() {return get11(); } },
        
        { [this](float v) {return m_nodebase->as<Core::EulerRotZ>()->setValue(v); } },

        { coreMap[0 * 4 + 0], /* \todo JH some better way how determine what element from DataMap should be used? */
            coreMap[0 * 4 + 1],
            coreMap[1 * 4 + 0],
            coreMap[1 * 4 + 1]
        });
}


float WorkspaceMakeEulerZ::get00()
{
    return m_nodebase->getData().getMat4()[0][0];
}

float WorkspaceMakeEulerZ::get01()
{
    return m_nodebase->getData().getMat4()[0][1];
}

float WorkspaceMakeEulerZ::get10()
{
    return m_nodebase->getData().getMat4()[1][0];
}

float WorkspaceMakeEulerZ::get11()
{
    return m_nodebase->getData().getMat4()[1][1];
}