#include "WorkspaceMakeEulerY.h"

WorkspaceMakeEulerY::WorkspaceMakeEulerY()
    :   WorkspaceMatrix4x4(Core::Builder::createNode<ENodeType::MakeEulerY>())
{}

bool WorkspaceMakeEulerY::drawDataSetValues(DIWNE::Diwne &diwne)
{
    const Core::Transform::DataMap& coreMap = m_nodebase->getDataMapRef();
    return drawDataSetValues_builder(diwne,
        //SS todo
        { "00", "02", "20", "22" },
        { [this]() {return get00(); }, [this]() {return get02(); }, [this]() {return get20(); } , [this]() {return get22(); } },
        { [this](float v) {return m_nodebase->as<Core::EulerRotY>()->setValue(v); } },


        { coreMap[0 * 4 + 0], /* \todo JH some better way how determine what element from DataMap should be used? */
            coreMap[0 * 4 + 2],
            coreMap[2 * 4 + 0],
            coreMap[2 * 4 + 2]
        });
}


/* \todo Move those to core and create setters */
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
