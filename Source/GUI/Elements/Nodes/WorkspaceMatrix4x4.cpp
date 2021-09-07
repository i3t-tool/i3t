#include "WorkspaceMatrix4x4.h"

WorkspaceMatrix4x4::WorkspaceMatrix4x4(Ptr<Core::NodeBase> nodebase)
    :   WorkspaceNodeWithCoreData(nodebase)
{
	setDataItemsWidth();
}

bool WorkspaceMatrix4x4::drawDataFull(DIWNE::Diwne &diwne, int index)
{
    bool inner_interaction_happen = false;
    bool valueChanged = false, actualValueChanged = false;
    int rowOfChange, columnOfChange;
    float valueOfChange, localData; /* user can change just one value at the moment */
    const glm::mat4& coreData = m_nodebase->getData(index).getMat4();
    const Core::Transform::DataMap& dataMap = m_nodebase->getDataMapRef();

    ImGui::PushItemWidth(getDataItemsWidth(diwne));
    ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, I3T::getSize(ESizeVec2::Nodes_FloatPadding));
    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, I3T::getSize(ESizeVec2::Nodes_ItemsSpacing));

    /* Drawing is row-wise */
    for (int rows = 0; rows < 4; rows++)
    {
      for (int columns = 0; columns < 4; columns++)
      {

        localData = coreData[columns][rows]; /* Data are column-wise */
        inner_interaction_happen |= drawDragFloatWithMap_Inline(diwne, actualValueChanged, localData, dataMap[columns * 4 + rows],
                                        fmt::format("##{}:r{}c{}", getId(), rows, columns));
        if (actualValueChanged)
        {
            valueChanged = true;
            columnOfChange = columns;
            rowOfChange = rows;
            valueOfChange = localData;
        }
        if(columns != 3)
        {
            ImGui::SameLine();
        }
      }
    }

    ImGui::PopStyleVar();
    ImGui::PopStyleVar();
    ImGui::PopItemWidth();

    if (valueChanged)
    {
      m_nodebase->setValue(valueOfChange, {columnOfChange, rowOfChange});
      setDataItemsWidth(); /* \todo JH maybe somehow wrap setValue to Core and set Items Width */
    }

    return inner_interaction_happen;
}

int WorkspaceMatrix4x4::maxLenghtOfData(int index)
{
    return maxLenghtOfData4x4( m_nodebase->getData(index).getMat4(), m_numberOfVisibleDecimal);
}


int maxLenghtOfData4x4(const glm::mat4& data, int numberOfVisibleDecimal)
{
    int act, maximal = 0;
    for(int column = 0; column < 4; column++)
    {
        for(int row = 0; row < 4; row++)
        {
            act = numberOfCharWithDecimalPoint( data[column][row], numberOfVisibleDecimal);
            if(act > maximal)
            {
                maximal = act;
            }
        }
    }
    return maximal;
}




