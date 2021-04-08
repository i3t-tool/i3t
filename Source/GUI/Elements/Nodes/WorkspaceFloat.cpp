#include "WorkspaceFloat.h"

WorkspaceFloat::WorkspaceFloat(ImTextureID headerBackground, WorkspaceFloatArgs const& args)
	: WorkspaceNodeWithCoreData(headerBackground, { .headerLabel = args.headerLabel, .nodeLabel = args.nodeLabel, .nodebase = args.nodebase })
{}

WorkspaceFloat::WorkspaceFloat(ImTextureID headerBackground, Ptr<Core::NodeBase> nodebase, std::string headerLabel, std::string nodeLabel)
	: WorkspaceNodeWithCoreData(headerBackground, nodebase, headerLabel, nodeLabel)
{
}

void WorkspaceFloat::drawData(util::NodeBuilder& builder)
{
	drawDataFull(builder); 
}

void WorkspaceFloat::drawDataFull(util::NodeBuilder& builder)
{
	const float& coreData = m_nodebase->getData().getFloat();
	int const idOfNode = this->m_id.Get();

	bool valueChanged = false;

	float localData;

	builder.Middle();

	ImGui::PushItemWidth(I3T::getSize(ESize::Nodes_FloatWidth));
	ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing,
		{ I3T::getSize(ESize::Nodes_ItemsSpacingX), I3T::getSize(ESize::Nodes_ItemsSpacingY) });

	localData = coreData;
	valueChanged |= drawDragFloatWithMap_Inline(&localData, 0, fmt::format("##{}:{}", idOfNode, 0));

	ImGui::PopStyleVar();
	ImGui::PopItemWidth();

	if (valueChanged)
	{
		m_nodebase->setValue(localData);
	}

	ImGui::Spring(0);
}
