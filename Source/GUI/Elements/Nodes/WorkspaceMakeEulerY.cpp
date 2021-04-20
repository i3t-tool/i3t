#include "WorkspaceMakeEulerY.h"

WorkspaceMakeEulerY::WorkspaceMakeEulerY(ImTextureID headerBackground, WorkspaceMakeEulerYArgs const& args)
    : WorkspaceMatrix4x4(headerBackground, { .levelOfDetail = args.levelOfDetail, .headerLabel = args.headerLabel, .nodeLabel = args.nodeLabel, .nodebase = args.nodebase })
{}

WorkspaceMakeEulerY::WorkspaceMakeEulerY(ImTextureID headerBackground, std::string headerLabel, std::string nodeLabel)
    : WorkspaceMatrix4x4(headerBackground, Builder::createNode<ENodeType::MakeEulerY>(), headerLabel, nodeLabel)
{}

void WorkspaceMakeEulerY::drawDataSetValues(util::NodeBuilder& builder)
{
    drawDataSetValues_builder(builder,
        //SS todo
        { "00", "02", "20", "22" },
        { [this]() {return get00(); }, [this]() {return get02(); }, [this]() {return get20(); } , [this]() {return get22(); } },
        { [this](float v) {return set00(v); }, [this](float v) {return set02(v); }, [this](float v) {return set20(v); } , [this](float v) {return set22(v); } });
}

/* \todo JH underlying functions will be taken from Core */
ValueSetResult WorkspaceMakeEulerY::set00(float val)
{
    return m_nodebase->setValue(val, glm::ivec2(0, 0));
}

ValueSetResult WorkspaceMakeEulerY::set02(float val)
{
    return m_nodebase->setValue(val, glm::ivec2(0, 2));
}

ValueSetResult WorkspaceMakeEulerY::set20(float val)
{
    return m_nodebase->setValue(val, glm::ivec2(2, 0));
}

ValueSetResult WorkspaceMakeEulerY::set22(float val)
{
    return m_nodebase->setValue(val, glm::ivec2(2, 2));
}

float WorkspaceMakeEulerY::get00()
{
    return m_nodebase->getData().getMat4()[0][0];
}

float WorkspaceMakeEulerY::get02()
{
    return m_nodebase->getData().getMat4()[0][2];
}

float WorkspaceMakeEulerY::get20()
{
    return m_nodebase->getData().getMat4()[2][0];
}

float WorkspaceMakeEulerY::get22()
{
    return m_nodebase->getData().getMat4()[2][2];
}