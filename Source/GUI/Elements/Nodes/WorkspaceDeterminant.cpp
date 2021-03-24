#include "WorkspaceDeterminant.h"

WorkspaceDeterminant::WorkspaceDeterminant(ImTextureID headerBackground, WorkspaceDeterminantArgs const& args)
	: WorkspaceNodeWithCoreData(headerBackground, { .viewScale = args.viewScale, .headerLabel = args.headerLabel, .nodeLabel = args.nodeLabel, .nodebase = args.nodebase })
{}

void WorkspaceDeterminant::drawDataFull(util::NodeBuilder& builder)
{
	const float& coreData = m_nodebase->getData().getFloat();
	int const idOfNode = this->m_id.Get();

	float localData;

	builder.Middle();

	ImGui::PushItemWidth(100.0f);

	localData = coreData;
	ImGui::DragFloat(.nodeLabel, localData);

	ImGui::PopItemWidth();

	ImGui::Spring(0);
}
