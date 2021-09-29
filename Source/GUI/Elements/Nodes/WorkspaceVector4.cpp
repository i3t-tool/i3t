#include "WorkspaceVector4.h"
WorkspaceVector4::WorkspaceVector4(Ptr<Core::NodeBase> nodebase)
    : WorkspaceNodeWithCoreData(nodebase)
{
	setDataItemsWidth();
}

bool WorkspaceVector4::drawDataFull(DIWNE::Diwne& diwne, int index)
{
	const glm::vec4& coreData = m_nodebase->getData(index).getVec4();
  const Core::Transform::DataMap& coreMap = m_nodebase->getDataMapRef();

	bool valueChanged = false, actualValueChanged = false;
	bool inner_interaction_happen = false;

	glm::vec4 localData;

	ImGui::PushItemWidth(getDataItemsWidth(diwne));
	ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, I3T::getSize(ESizeVec2::Nodes_FloatPadding));
	ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, I3T::getSize(ESizeVec2::Nodes_ItemsSpacing));

	for (int columns = 0; columns < 4; columns++)
	{
		localData[columns] = coreData[columns];
		inner_interaction_happen |= drawDragFloatWithMap_Inline(diwne, actualValueChanged, localData[columns], coreMap[columns], fmt::format("##{}:{}", getId(), columns));
        if(actualValueChanged) valueChanged = true;

	}
  ImGui::PopStyleVar();
	ImGui::PopStyleVar();
	ImGui::PopItemWidth();

	if (valueChanged)
	{
	    m_nodebase->setValue(localData);
	    setDataItemsWidth();
//		Nodebase->setValue(valueOfChange, {columnOfChange});
	}

	// ImGui::Spring(0);
	return inner_interaction_happen;
}

int WorkspaceVector4::maxLenghtOfData(int index)
{
    int act, maximal = 0;
    const glm::vec4& coreData = m_nodebase->getData().getVec4();

    for(int column=0; column < 4; column++)
    {
        act = numberOfCharWithDecimalPoint( coreData[column], m_numberOfVisibleDecimal );
        if(act > maximal)
        {
            maximal = act;
        }
    }

    return maximal;
}
