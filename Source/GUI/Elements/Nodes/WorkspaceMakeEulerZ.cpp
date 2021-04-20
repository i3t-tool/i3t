#include "WorkspaceMakeEulerZ.h"

WorkspaceMakeEulerZ::WorkspaceMakeEulerZ(ImTextureID headerBackground, WorkspaceMakeEulerZArgs const& args)
    : WorkspaceMatrix4x4(headerBackground, { .levelOfDetail = args.levelOfDetail, .headerLabel = args.headerLabel, .nodeLabel = args.nodeLabel, .nodebase = args.nodebase })
{}

WorkspaceMakeEulerZ::WorkspaceMakeEulerZ(ImTextureID headerBackground, std::string headerLabel, std::string nodeLabel)
    : WorkspaceMatrix4x4(headerBackground, Builder::createNode<ENodeType::MakeEulerY>(), headerLabel, nodeLabel)
{}

void WorkspaceMakeEulerZ::drawDataSetValues(util::NodeBuilder& builder)
{
    drawDataSetValues_builder(builder,
        //SS todo
        { "00", "01", "10", "11" },
        { [this]() {return get00(); }, [this]() {return get01(); }, [this]() {return get10(); } , [this]() {return get11(); } },
        { [this](float v) {return set00(v); }, [this](float v) {return set01(v); }, [this](float v) {return set10(v); } , [this](float v) {return set11(v); }});
}

/* \todo JH underlying functions will be taken from Core */
ValueSetResult WorkspaceMakeEulerZ::set00(float val)
{
    return m_nodebase->setValue(val, glm::ivec2(0, 0));
}

ValueSetResult WorkspaceMakeEulerZ::set01(float val)
{
    return m_nodebase->setValue(val, glm::ivec2(0, 1));
}

ValueSetResult WorkspaceMakeEulerZ::set10(float val)
{
    return m_nodebase->setValue(val, glm::ivec2(1, 0));
}

ValueSetResult WorkspaceMakeEulerZ::set11(float val)
{
    return m_nodebase->setValue(val, glm::ivec2(1, 1));
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