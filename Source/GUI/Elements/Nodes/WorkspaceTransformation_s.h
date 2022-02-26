#pragma once

#include "WorkspaceTransformation.h"

template<ETransformType T>
class WorkspaceTransformation_s : public WorkspaceTransformation
{
public:
WorkspaceTransformation_s(DIWNE::Diwne& diwne)
    :    WorkspaceTransformation(diwne, Core::Builder::createTransform<T>())
    {
        setDataItemsWidth();
    }

virtual Core::ETransformState dataAreValid() override
{
    return m_nodebase->as<Core::TransformImpl<T>>()->isValid();
}

int maxLenghtOfData()
{
    if (ETransformType::Quat == T)
        return maxLenghtOfDataQuaternion( m_nodebase->getData().getQuat(), m_numberOfVisibleDecimal);
    else
        return maxLenghtOfData4x4( m_nodebase->getData().getMat4(), m_numberOfVisibleDecimal);

}

bool drawDataSetValues()
{
    bool inner_interaction_happen = false, value_changed = false;
    auto nodebase = m_nodebase->as<Core::TransformImpl<T>>();

    for (auto& [key, valueStore] : nodebase->getDefaultValues())
    {
        switch (valueStore.opValueType)
        {
            case EValueType::Vec3:
            {
                auto localData = valueStore.getVec3();

                inner_interaction_happen |= drawDataSetValues_builder({fmt::format("{} X",key.c_str()), fmt::format("{} Y",key.c_str()), fmt::format("{} Z",key.c_str()) } /* \todo MH all/whole labels from core? */
                                                            ,   {&localData[0], &localData[1], &localData[2]}
                                                            ,   value_changed);
                if (value_changed)
                {
                    nodebase->setDefaultValue(key, localData);
                    setDataItemsWidth();
                }
            break;
            }
            case EValueType::Vec4:
            {
                auto localData = valueStore.getVec4();

                inner_interaction_happen |= drawDataSetValues_builder({fmt::format("{} X",key.c_str()), fmt::format("{} Y",key.c_str()), fmt::format("{} Z",key.c_str()), fmt::format("{} W",key.c_str()) } /* \todo MH all/whole labels from core? */
                                                            ,   {&localData[0], &localData[1], &localData[2], &localData[3]}
                                                            ,   value_changed);
                if (value_changed)
                {
                    nodebase->setDefaultValue(key, localData);
                    setDataItemsWidth();
                }
            break;
            }
            case EValueType::Quat:
            {
                auto localData = valueStore.getQuat();

                inner_interaction_happen |= drawDataSetValues_builder({fmt::format("{} X",key.c_str()), fmt::format("{} Y",key.c_str()), fmt::format("{} Z",key.c_str()), fmt::format("{} W",key.c_str()) } /* \todo MH all/whole labels from core? */
                                                            ,   {&localData[0], &localData[1], &localData[2], &localData[3]}
                                                            ,   value_changed);
                if (value_changed)
                {
                    nodebase->setDefaultValue(key, localData);
                    setDataItemsWidth();
                }
            break;
            }
            case EValueType::Matrix:
            {
                inner_interaction_happen |= drawDataFull();
            break;
            }
            case EValueType::Float:
            {
                auto localData = valueStore.getFloat();

                inner_interaction_happen |= drawDataSetValues_builder({fmt::format("{}",key.c_str()) }
                                                            ,   {&localData}
                                                            ,   value_changed);
                if (value_changed)
                {
                    nodebase->setDefaultValue(key, localData);
                    setDataItemsWidth();
                }
            break;
            }
            default:
                inner_interaction_happen |= drawDataFull();
        }
    }
    return inner_interaction_happen;
}

};
