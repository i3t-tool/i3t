#include "WorkspaceFloat.h"

WorkspaceFloat::WorkspaceFloat(ImTextureID headerBackground, WorkspaceFloatArgs const& args)
	: WorkspaceNodeWithCoreData(headerBackground, { .levelOfDetail = args.levelOfDetail, .headerLabel = args.headerLabel, .nodeLabel = args.nodeLabel, .nodebase = args.nodebase })
{
	fw.showMyPopup = false;
	fw.id = "";
	fw.value = NAN; 
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
	fw.value = NAN; 
	fw.name = "float";
	fw.rows = 0;
	fw.columns = 0;
	setDataItemsWidth();
}


void WorkspaceFloat::drawDataFull(util::NodeBuilder& builder, int index)
{

	//assert if operator its imposible to value change (except free operators)
	const float coreData = m_nodebase->getData(index).getFloat();
	int const idOfNode = this->m_id.Get();
	int const map = m_nodebase->getDataMapRef()[0];

	bool valueChanged = false;
	float localData;

	//builder.Middle();

	ImGui::PushItemWidth(m_dataItemsWidth);
  ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, {I3T::getSize(ESize::Nodes_floatPaddingX), I3T::getSize(ESize::Nodes_floatPaddingY)});
	ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, { I3T::getSize(ESize::Nodes_ItemsSpacingX), I3T::getSize(ESize::Nodes_ItemsSpacingY) });

	localData = coreData;
	valueChanged |= drawDragFloatWithMap_Inline(&localData, map, fmt::format("##{}:{}", idOfNode, index)); /* datamap value 1 is changeable */

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
		//ask MH
		m_nodebase->setValue(localData);
		setDataItemsWidth();
	}
}

int WorkspaceFloat::maxLenghtOfData()
{
    return numberOfCharWithDecimalPoint( m_nodebase->getData().getFloat(), m_numberOfVisibleDecimal );
}
