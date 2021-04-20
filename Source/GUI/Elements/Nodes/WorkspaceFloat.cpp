#include "WorkspaceFloat.h"

WorkspaceFloat::WorkspaceFloat(ImTextureID headerBackground, WorkspaceFloatArgs const& args)
	: WorkspaceNodeWithCoreData(headerBackground, { .headerLabel = args.headerLabel, .nodeLabel = args.nodeLabel, .nodebase = args.nodebase })
{
	fw.showMyPopup = false;
	fw.id = "";
	fw.value = NULL;
	fw.name = "float";
	fw.rows = 0;
	fw.columns = 0;
	setDataItemsWidth();
}

WorkspaceFloat::WorkspaceFloat(ImTextureID headerBackground, Ptr<Core::NodeBase> nodebase, std::string headerLabel, std::string nodeLabel)
	: WorkspaceNodeWithCoreData(headerBackground, nodebase, headerLabel, nodeLabel)
{
	fw.showMyPopup = false;
	fw.id = "";
	fw.value = NULL;
	fw.name = "float";
	fw.rows = 0;
	fw.columns = 0;
	setDataItemsWidth();
}


void WorkspaceFloat::drawDataFull(util::NodeBuilder& builder)
{

	const float coreData = m_nodebase->getData().getFloat();
	int const idOfNode = this->m_id.Get();

	bool valueChanged = false;
	float localData;

	builder.Middle();

	ImGui::PushItemWidth(m_dataItemsWidth);
	ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, { I3T::getSize(ESize::Nodes_ItemsSpacingX), I3T::getSize(ESize::Nodes_ItemsSpacingY) });

	localData = coreData;
	valueChanged |= drawDragFloatWithMap_Inline(&localData, 0, fmt::format("##{}", idOfNode));

		if (ImGui::IsMouseReleased(1) && ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenBlockedByPopup))
		{
			fw.showMyPopup = true;
			fw.id = fmt::format("##{}", idOfNode);
			fw.value = localData;
		}


	ImGui::PopStyleVar();
	ImGui::PopItemWidth();

	if (valueChanged)
	{
		m_nodebase->setValue(localData);
		setDataItemsWidth();
	}

	ImGui::Spring(0);


}

int WorkspaceFloat::maxLenghtOfData() {
	int act, maximal = 0;
	const float& coreData = m_nodebase->getData().getFloat();

	act = numberOfCharWithDecimalPoint(coreData, m_numberOfVisibleDecimal);
	if (act > maximal)
	{
		maximal = act;
	}

	return maximal;
}
