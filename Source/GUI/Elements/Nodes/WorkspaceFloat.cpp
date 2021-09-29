#include "WorkspaceFloat.h"
WorkspaceFloat::WorkspaceFloat(Ptr<Core::NodeBase> nodebase)
	:   WorkspaceNodeWithCoreData(nodebase)
{
	setDataItemsWidth();
}

bool WorkspaceFloat::drawDataFull(DIWNE::Diwne &diwne, int index)
{
	//assert if operator its imposible to value change (except free operators)
	const float coreData = m_nodebase->getData(index).getFloat();
    const Core::Transform::DataMap& coreMap = m_nodebase->getDataMapRef();

	bool valueChanged = false, inner_interaction_happen = false;
	float localData;

	ImGui::PushItemWidth(getDataItemsWidth(diwne));
	ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, I3T::getSize(ESizeVec2::Nodes_FloatPadding));
	ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, I3T::getSize(ESizeVec2::Nodes_ItemsSpacing));

	localData = coreData;
	inner_interaction_happen |= drawDragFloatWithMap_Inline(diwne, valueChanged, localData, coreMap[0], fmt::format("##{}:{}", getId(), index)); /* datamap value 1 is changeable */


	ImGui::PopStyleVar();
    ImGui::PopStyleVar();
	ImGui::PopItemWidth();

	if (valueChanged)
	{
		m_nodebase->setValue(localData);
		setDataItemsWidth();
	}
	return inner_interaction_happen;
}

int WorkspaceFloat::maxLenghtOfData(int index)
{
    return numberOfCharWithDecimalPoint( m_nodebase->getData(index).getFloat(), m_numberOfVisibleDecimal );
}
