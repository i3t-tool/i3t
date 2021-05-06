
#include "WorkspaceVector3.h"

WorkspaceVector3::WorkspaceVector3(ImTextureID headerBackground, WorkspaceVector3Args const& args)
    : WorkspaceNodeWithCoreData(headerBackground, {.levelOfDetail=args.levelOfDetail, .headerLabel=args.headerLabel, .nodeLabel=args.nodeLabel, .nodebase=args.nodebase})
{
	fw.showMyPopup = false;
	fw.id = "";
	fw.value = NAN; 
	fw.name = "vector3";
	fw.rows = 0;
	setDataItemsWidth();
}

WorkspaceVector3::WorkspaceVector3(ImTextureID headerBackground, Ptr<Core::NodeBase> nodebase, std::string headerLabel, std::string nodeLabel)
    : WorkspaceNodeWithCoreData(headerBackground, nodebase, headerLabel, nodeLabel)
{
	fw.showMyPopup = false;
	fw.id = "";
	fw.value = NAN; 
	fw.name = "vector3";
	fw.rows = 0;

    setDataItemsWidth();
}

void WorkspaceVector3::drawDataFull(util::NodeBuilder& builder, int index)
{
	const glm::vec3& coreData = m_nodebase->getData().getVec3();
	int const coreMap[3] = {1,2,3}; /* \todo JH will be map for vector? */
	int const idOfNode = this->m_id.Get();

	bool valueChanged = false;
	/* \todo JH if same function setValue(value, position) as for Transformations will be added -> than this is probably
	better /same as in Matrix4x4/ int columnOfChange; float localData, valueOfChange;
	  */
	glm::vec3 localData;

	//builder.Middle();


	ImGui::PushItemWidth(m_dataItemsWidth);
	ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, {I3T::getSize(ESize::Nodes_ItemsSpacingX), I3T::getSize(ESize::Nodes_ItemsSpacingY)});

	for (int columns = 0; columns < 3; columns++)
	{
		localData[columns] = coreData[columns];
		valueChanged |=
				drawDragFloatWithMap_Inline(&localData[columns], coreMap[columns], fmt::format("##{}:{}", idOfNode, columns));

		//        localData = coreData[columns];
		//        if (drawDragFloatWithMap_Inline(&localData,
		//                                        coreMap[columns],
		//                                        fmt::format("##{}:{}", idOfNode, columns)))
		//        {
		//            valueChanged = true;
		//            columnOfChange = columns;
		//            valueOfChange = localData;
		//        }
				if (ImGui::IsMouseReleased(1) && ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenBlockedByPopup))
				{
					fw.showMyPopup = true;
					fw.id = fmt::format("##{}:{}", idOfNode, columns);
					fw.value = localData[columns];
					fw.columns = columns;
				}

	}
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

int WorkspaceVector3::maxLenghtOfData()
{
    int act, maximal = 0;
    const glm::vec3& coreData = m_nodebase->getData().getVec3();

    for(int column=0; column < 3; column++)
    {
        act = numberOfCharWithDecimalPoint( coreData[column], m_numberOfVisibleDecimal );
        if(act > maximal)
        {
            maximal = act;
        }
    }

    return maximal;
}
