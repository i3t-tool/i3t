#include "WorkspaceMatrixScale.h"

WorkspaceMatrixScale::WorkspaceMatrixScale(ImTextureID headerBackground, WorkspaceMatrixScaleArgs const& args)
    : WorkspaceMatrix4x4(headerBackground, {.levelOfDetail=args.levelOfDetail, .headerLabel=args.headerLabel, .nodeLabel=args.nodeLabel, .nodebase=args.nodebase})
{}

WorkspaceMatrixScale::WorkspaceMatrixScale(ImTextureID headerBackground, std::string headerLabel, std::string nodeLabel)
    : WorkspaceMatrix4x4(headerBackground, Builder::createTransform<Core::Scale>(), headerLabel, nodeLabel){
}

void WorkspaceMatrixScale::drawDataSetValues(util::NodeBuilder& builder)
{
    drawDataSetValues_builder(builder,
                              {"x", "y", "z"},
                              { [this](){return getValueX();}, [this](){return getValueY();}, [this](){return getValueZ();} },
                              { [this](float v){return setValueX(v);}, [this](float v){return setValueY(v);}, [this](float v){return setValueZ(v);} });
}

/* \todo JH underlying functions will be taken from Core */
ValueSetResult WorkspaceMatrixScale::setValueX(float val)
{
    return m_nodebase->setValue(val, glm::ivec2(0, 0));
}

ValueSetResult WorkspaceMatrixScale::setValueY(float val)
{
    return m_nodebase->setValue(val, glm::ivec2(1, 1));
}

ValueSetResult WorkspaceMatrixScale::setValueZ(float val)
{
    return m_nodebase->setValue(val, glm::ivec2(2, 2));
}

float WorkspaceMatrixScale::getValueX()
{
    return m_nodebase->getData().getMat4()[0][0];
}

float WorkspaceMatrixScale::getValueY()
{
    return m_nodebase->getData().getMat4()[1][1];
}

float WorkspaceMatrixScale::getValueZ()
{
    return m_nodebase->getData().getMat4()[2][2];
}
