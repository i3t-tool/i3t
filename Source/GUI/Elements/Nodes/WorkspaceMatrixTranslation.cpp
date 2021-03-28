#include "WorkspaceMatrixTranslation.h"

WorkspaceMatrixTranslation::WorkspaceMatrixTranslation(ImTextureID headerBackground, WorkspaceMatrixTranslationArgs const& args)
    : WorkspaceMatrix4x4(headerBackground, {.levelOfDetail=args.levelOfDetail, .headerLabel=args.headerLabel, .nodeLabel=args.nodeLabel, .nodebase=args.nodebase})
{}

WorkspaceMatrixTranslation::WorkspaceMatrixTranslation(ImTextureID headerBackground, std::string headerLabel, std::string nodeLabel)
    : WorkspaceMatrix4x4(headerBackground, Builder::createTransform<Core::Translation>(), headerLabel, nodeLabel)
{}

void WorkspaceMatrixTranslation::drawDataSetValues(util::NodeBuilder& builder)
{
    drawDataSetValues_builder(builder,
                              {"x", "y", "z"},
                              { [this](){return getValueX();}, [this](){return getValueY();}, [this](){return getValueZ();} },
                              { [this](float v){return setValueX(v);}, [this](float v){return setValueY(v);}, [this](float v){return setValueZ(v);} });
}

/* \todo JH underlying functions will be taken from Core */
ValueSetResult WorkspaceMatrixTranslation::setValueX(float val)
{
    return m_nodebase->setValue(val, glm::ivec2(3, 0));
}

ValueSetResult WorkspaceMatrixTranslation::setValueY(float val)
{
    return m_nodebase->setValue(val, glm::ivec2(3, 1));
}

ValueSetResult WorkspaceMatrixTranslation::setValueZ(float val)
{
    return m_nodebase->setValue(val, glm::ivec2(3, 2));
}

float WorkspaceMatrixTranslation::getValueX()
{
    return m_nodebase->getData().getMat4()[3][0];
}

float WorkspaceMatrixTranslation::getValueY()
{
    return m_nodebase->getData().getMat4()[3][1];
}

float WorkspaceMatrixTranslation::getValueZ()
{
    return m_nodebase->getData().getMat4()[3][2];
}

