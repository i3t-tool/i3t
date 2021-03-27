#include "WorkspaceMatrix4x4.h"

WorkspaceMatrix4x4::WorkspaceMatrix4x4(ImTextureID headerBackground, WorkspaceMatrix4x4Args const& args)
    : WorkspaceNodeWithCoreData(headerBackground, {.levelOfDetail=args.levelOfDetail, .headerLabel=args.headerLabel, .nodeLabel=args.nodeLabel, .nodebase=args.nodebase})
{}

WorkspaceMatrix4x4::WorkspaceMatrix4x4(ImTextureID headerBackground, Ptr<Core::NodeBase> nodebase, std::string headerLabel, std::string nodeLabel)
    : WorkspaceNodeWithCoreData(headerBackground, nodebase, headerLabel, nodeLabel)
{}

void WorkspaceMatrix4x4::drawData(util::NodeBuilder& builder)
{
	drawDataFull(builder); /* default function always draw all data */
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

	//ImGui::PushItemWidth(default_width*scale);
	/* Drawing is row-wise */
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
		}
		ImGui::NewLine();
	}
	ImGui::PopItemWidth();

	if (valueChanged)
	{
		m_nodebase->setValue(valueOfChange, {columnOfChange, rowOfChange});
	}

	ImGui::Spring(0); /* \todo JH what is Spring? */
}
