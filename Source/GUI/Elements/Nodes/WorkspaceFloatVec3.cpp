//
// Created by Sofie on 06.05.2021.
//

#include "WorkspaceFloatVec3.h"

WorkspaceFloatVec3::WorkspaceFloatVec3(ImTextureID headerBackground, WorkspaceFloatVec3Args const& args)
		: WorkspaceNodeWithCoreData(
					headerBackground,
					{.levelOfDetail = args.levelOfDetail, .headerLabel = args.headerLabel, .nodeLabel = args.nodeLabel,
           /*.nodebase = args.nodebase*/})
{
	fw.showMyPopup = false;
	fw.id = "";
	fw.value = NAN;
	fw.name = "float";
	fw.rows = 0;
	fw.columns = 0;
	setDataItemsWidth();
}

WorkspaceFloatVec3::WorkspaceFloatVec3(ImTextureID headerBackground, Ptr<Core::NodeBase> nodebase,
                                       std::string headerLabel, std::string nodeLabel)
		: WorkspaceNodeWithCoreData(headerBackground, nodebase, headerLabel, nodeLabel)
{
	fw.showMyPopup = false;
	fw.id = "";
	fw.value = NAN;
	fw.name = "float";
	fw.rows = 0;
	fw.columns = 0;
	setDataItemsWidth();
}

void WorkspaceFloatVec3::drawDataFull(util::NodeBuilder& builder, int index)
{
	if (index == 0)
	{ // -> float

		fw.name = "float";
		fw.columns = 0;

		const float coreData = m_nodebase->getData(index).getFloat();
		int const idOfNode = this->m_id.Get();
		const Core::Transform::DataMap& coreMap = m_nodebase->getDataMapRef();

		bool valueChanged = false;
		float localData;

		ImGui::PushItemWidth(m_dataItemsWidth);
		ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, I3T::getSize(ESizeVec2::Nodes_FloatPadding));
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, I3T::getSize(ESizeVec2::Nodes_ItemsSpacing));

		localData = coreData;
		valueChanged |= drawDragFloatWithMap_Inline(
				&localData, coreMap[0], fmt::format("##{}:{}", idOfNode, index)); /* datamap value 1 is changeable */

		if (ImGui::IsMouseReleased(1) && ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenBlockedByPopup))
		{
			fw.showMyPopup = true;
			fw.id = fmt::format("##{}", idOfNode);
			fw.value = localData;
		}

		ImGui::PopStyleVar();
		ImGui::PopStyleVar();
		ImGui::PopItemWidth();

		if (valueChanged)
		{
			setDataItemsWidth();
		}
    ImGui::Spring(1);


		//TODO better way to solve this?
		if(isQuatToFloatVec()){
			ImGuiWindow* window = ImGui::GetCurrentWindowRead();
			ne::PinRect(ImVec2(window->DC.LastItemRect.Min.x,window->DC.LastItemRect.Min.y),ImVec2(window->DC.LastItemRect.Max.x + 58.0f,window->DC.LastItemRect.Max.y));
		}
	}
	else if (index == 1)
	{ // -> vector3

		fw.name = "vector3";

		const glm::vec3& coreData = m_nodebase->getData(index).getVec3();
		const Core::Transform::DataMap& coreMap = m_nodebase->getDataMapRef();
		int const idOfNode = this->m_id.Get();

		bool valueChanged = false;
		glm::vec3 localData;

		ImGuiWindow* window = ImGui::GetCurrentWindowRead();
		ImVec2 pinStart = ImVec2(window->DC.LastItemRect.Min.x,window->DC.LastItemRect.Min.y);

		ImGui::PushItemWidth(m_dataItemsWidth);
		ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, I3T::getSize(ESizeVec2::Nodes_FloatPadding));
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, I3T::getSize(ESizeVec2::Nodes_ItemsSpacing));

		for (int columns = 0; columns < 3; columns++)
		{
			localData[columns] = coreData[columns];
			valueChanged |= drawDragFloatWithMap_Inline(&localData[columns], coreMap[columns],
			                                            fmt::format("##{}:{}", idOfNode, columns));

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
			setDataItemsWidth();
		}

		if(isQuatToAngleAxis()){
			ImGuiWindow* window = ImGui::GetCurrentWindowRead();
			ne::PinRect(pinStart,ImVec2(window->DC.LastItemRect.Max.x + 55.0f,window->DC.LastItemRect.Max.y));
		}
	}
}

int WorkspaceFloatVec3::maxLenghtOfData()
{
	if (fw.name == "float")
	{
		return numberOfCharWithDecimalPoint(m_nodebase->getData(0).getFloat(), m_numberOfVisibleDecimal);
	}
	else if (fw.name == "vector3")
	{
		int act, maximal = 0;
		const glm::vec3& coreData = m_nodebase->getData(1).getVec3();

		for (int column = 0; column < 3; column++)
		{
			act = numberOfCharWithDecimalPoint(coreData[column], m_numberOfVisibleDecimal);
			if (act > maximal)
			{
				maximal = act;
			}
		}
		return maximal;
	}
	else
	{
		return 0;
	}
}