#include "WorkspaceMatrix4x4.h"

WorkspaceMatrix4x4::WorkspaceMatrix4x4(ImTextureID headerBackground, WorkspaceMatrix4x4Args const& args)
    : WorkspaceNodeWithCoreData(headerBackground, { .headerLabel=args.headerLabel, .nodeLabel=args.nodeLabel, .nodebase=args.nodebase})
{
	fw.showMyPopup = false;
	fw.id = "";
	fw.value = NULL;
	fw.name = "matrix4x4";

    setDataItemsWidth();
}

WorkspaceMatrix4x4::WorkspaceMatrix4x4(ImTextureID headerBackground, Ptr<Core::NodeBase> nodebase, std::string headerLabel, std::string nodeLabel)
    : WorkspaceNodeWithCoreData(headerBackground, nodebase, headerLabel, nodeLabel)
{
	fw.showMyPopup = false;
	fw.id = "";
	fw.value = NULL;
	fw.name = "matrix4x4";
	setDataItemsWidth();
}



void WorkspaceMatrix4x4::drawDataFull(util::NodeBuilder& builder)
{
	const glm::mat4& coreData = m_nodebase->getData().getMat4();
	const Core::Transform::DataMap& coreMap = m_nodebase->getDataMap();
	int const idOfNode = this->m_id.Get();

	bool valueChanged = false;
	int rowOfChange, columnOfChange;
	float valueOfChange, localData; /* user can change just one value at the moment */

	builder.Middle();

	ImGui::PushItemWidth(m_dataItemsWidth);
	/* Drawing is row-wise */

	ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0));
	ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0, 0));


	for (int rows = 0; rows < 4; rows++)
	{
		for (int columns = 0; columns < 4; columns++)
		{
			

			localData = coreData[columns][rows]; /* Data are column-wise */
			if (drawDragFloatWithMap_Inline(&localData, coreMap[columns * 4 + rows],
			                                fmt::format("##{}:r{}c{}", idOfNode, rows, columns)))
			{
				valueChanged = true;
				rowOfChange = rows;
				columnOfChange = columns;
				valueOfChange = localData;
			}
			if (ImGui::IsMouseReleased(1) && ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenBlockedByPopup))
			{
				fw.showMyPopup = true;
				fw.id = fmt::format("##{}:r{}c{}", idOfNode, rows, columns);
				fw.value = localData;
				fw.columns = columns;
				fw.rows = rows;
				//ImGui::OpenPopup("float_context_menu");
			}
			
			
		}
		ImGui::NewLine();
	}

	//ImGui::EndVertical();
	//ImGui::EndHorizontal();	

	ImGui::PopStyleVar();
	ImGui::PopStyleVar();

	ImGui::PopItemWidth();

	if (valueChanged)
	{
		m_nodebase->setValue(valueOfChange, {columnOfChange, rowOfChange});
		setDataItemsWidth(); /* \todo JH maybe somehow wrap setValue to Core and set Items Width */
	}

	ImGui::Spring(0); /* \todo JH what is Spring? */
}

int WorkspaceMatrix4x4::maxLenghtOfData()
{
    int act, maximal = 0;
    const glm::mat4& coreData = m_nodebase->getData().getMat4();

    for(int column = 0; column < 4; column++)
    {
        for(int row = 0; row < 4; row++)
        {
            act = numberOfCharWithDecimalPoint( coreData[column][row], m_numberOfVisibleDecimal );
            if(act > maximal)
            {
                maximal = act;
            }
        }
    }

    return maximal;
}

