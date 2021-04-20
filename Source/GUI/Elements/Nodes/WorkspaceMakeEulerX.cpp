#include "WorkspaceMakeEulerX.h"

WorkspaceMakeEulerX::WorkspaceMakeEulerX(ImTextureID headerBackground, WorkspaceMakeEulerXArgs const& args)
    : WorkspaceMatrix4x4(headerBackground, { .levelOfDetail = args.levelOfDetail, .headerLabel = args.headerLabel, .nodeLabel = args.nodeLabel, .nodebase = args.nodebase })
{}

WorkspaceMakeEulerX::WorkspaceMakeEulerX(ImTextureID headerBackground, std::string headerLabel, std::string nodeLabel)
    : WorkspaceMatrix4x4(headerBackground, Builder::createNode<ENodeType::MakeEulerX>(), headerLabel, nodeLabel)
{}

void WorkspaceMakeEulerX::drawDataSetValues(util::NodeBuilder& builder)
{
    drawDataSetValues_builder(builder,
        //SS todo
        { "11", "12", "21", "22" },
        { [this]() {return get11(); }, [this]() {return get12(); }, [this]() {return get21(); } , [this]() {return get22(); } },
        { [this](float v) {return set11(v); }, [this](float v) {return set12(v); }, [this](float v) {return set21(v); }, [this](float v) {return set22(v); } });
}

/* \todo JH underlying functions will be taken from Core */
ValueSetResult WorkspaceMakeEulerX::set11(float val)
{
    return m_nodebase->setValue(val, glm::ivec2(1, 1));
}

ValueSetResult WorkspaceMakeEulerX::set12(float val)
{
    return m_nodebase->setValue(val, glm::ivec2(1, 2));
}

ValueSetResult WorkspaceMakeEulerX::set21(float val)
{
    return m_nodebase->setValue(val, glm::ivec2(2, 1));
}

ValueSetResult WorkspaceMakeEulerX::set22(float val)
{
    return m_nodebase->setValue(val, glm::ivec2(2, 2));
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