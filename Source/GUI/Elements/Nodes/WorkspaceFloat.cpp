#include "WorkspaceFloat.h"

WorkspaceFloat::WorkspaceFloat(ImTextureID headerBackground, WorkspaceFloatArgs const& args)
	: WorkspaceNodeWithCoreData(headerBackground, {.levelOfDetail = args.levelOfDetail, .headerLabel = args.headerLabel, .nodeLabel = args.nodeLabel, .nodebase = args.nodebase })
{}

WorkspaceFloat::WorkspaceFloat(ImTextureID headerBackground, Ptr<Core::NodeBase> nodebase, std::string headerLabel, std::string nodeLabel)
	: WorkspaceNodeWithCoreData(headerBackground, nodebase, headerLabel, nodeLabel)
{
}

void WorkspaceFloat::drawDataFull(util::NodeBuilder& builder)
{
	const float& coreData = m_nodebase->getData().getFloat();
	int const idOfNode = this->m_id.Get();

	bool valueChanged = false;

	float localData;

	/* \todo JH somehow include m_dataItemsWidth - width based on number of characters*/
	ImGui::PushItemWidth(I3T::getSize(ESize::Nodes_FloatWidth));
	ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing,
		{ I3T::getSize(ESize::Nodes_ItemsSpacingX), I3T::getSize(ESize::Nodes_ItemsSpacingY) });

	localData = coreData;
	valueChanged |= drawDragFloatWithMap_Inline(&localData, 1, fmt::format("##{}:{}", idOfNode, 0)); /* datamap value 1 is changeable */

	ImGui::PopStyleVar();
	ImGui::PopItemWidth();

	if (valueChanged)
	{
		m_nodebase->setValue(localData);
	}

	ImGui::Spring(0);
}

int WorkspaceFloat::maxLenghtOfData()
{
    return numberOfCharWithDecimalPoint( m_nodebase->getData().getFloat(), m_numberOfVisibleDecimal );
}
