#include "WorkspaceVector4.h"

WorkspaceVector4::WorkspaceVector4(ImTextureID headerBackground, WorkspaceVector4Args const& args)
    : WorkspaceNodeWithCoreData(headerBackground, {.levelOfDetail=args.levelOfDetail, .headerLabel=args.headerLabel, .nodeLabel=args.nodeLabel, .nodebase=args.nodebase})
{
	fw.showMyPopup = false;
	fw.id = "";
	fw.value = NAN; 
	fw.name = "vector4";
	fw.rows = 0;
	setDataItemsWidth();
}

WorkspaceVector4::WorkspaceVector4(ImTextureID headerBackground, Ptr<Core::NodeBase> nodebase, std::string headerLabel, std::string nodeLabel)
    : WorkspaceNodeWithCoreData(headerBackground, nodebase, headerLabel, nodeLabel)
{
	fw.showMyPopup = false;
	fw.id = "";
	fw.value = NAN; 
	fw.name = "vector4";
	fw.rows = 0;

    setDataItemsWidth();
}

void WorkspaceVector4::drawDataFull(util::NodeBuilder& builder, int index)
{
	const glm::vec4& coreData = m_nodebase->getData(index).getVec4();
  const Core::Transform::DataMap& coreMap = m_nodebase->getDataMapRef();
	int const idOfNode = this->m_id.Get();

	bool valueChanged = false;

	glm::vec4 localData;

	ImGui::PushItemWidth(m_dataItemsWidth);
	ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, I3T::getSizeVec2(ESizeVec2::Nodes_FloatPadding));
	ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, I3T::getSizeVec2(ESizeVec2::Nodes_ItemsSpacing));

	for (int columns = 0; columns < 4; columns++)
	{
		localData[columns] = coreData[columns];
		valueChanged |=
				drawDragFloatWithMap_Inline(&localData[columns], coreMap[columns], fmt::format("##{}:{}", idOfNode, columns));

				if (ImGui::IsMouseReleased(1) && ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenBlockedByPopup))
				{
					fw.showMyPopup = true;
					fw.id = fmt::format("##{}:{}", idOfNode, columns);
					fw.value = localData[columns];
					fw.columns = columns;
				}

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

	ImGui::Spring(0);
}

int WorkspaceVector4::maxLenghtOfData()
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
