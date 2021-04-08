#include "WorkspaceMatrixInversion.h"

WorkspaceMatrixInversion::WorkspaceMatrixInversion(ImTextureID headerBackground, WorkspaceMatrixInversionArgs const& args)
    : WorkspaceMatrix4x4(headerBackground, { .viewScale = args.viewScale, .headerLabel = args.headerLabel, .nodeLabel = args.nodeLabel, .nodebase = args.nodebase })
{}

WorkspaceMatrixInversion::WorkspaceMatrixInversion(ImTextureID headerBackground, std::string headerLabel, std::string nodeLabel)
    : WorkspaceMatrix4x4(headerBackground, Builder::createNode<ENodeType::Inversion>(), headerLabel, nodeLabel)
{}

void WorkspaceMatrixInversion::drawData(util::NodeBuilder& builder)
{
    switch (m_viewScale)
    {
    case WorkspaceViewScale::Full:
        //drawDataFull(builder); tmp
		drawDataSetValues(builder);
        break;
    case WorkspaceViewScale::SetValues:
        drawDataSetValues(builder);
        break;
    case WorkspaceViewScale::Label:
        drawDataLabel(builder);
        break;

    default:
         drawDataFull(builder);
    }

}

void WorkspaceMatrixInversion::drawDataSetValues(util::NodeBuilder& builder)
{
    //TODO SS
	const glm::mat4& coreData = m_nodebase->getData().getMat4();
	const Core::Transform::DataMap& coreMap = m_nodebase->getDataMap();
	int const idOfNode = this->m_id.Get();

	bool valueChanged = false;
	int rowOfChange, columnOfChange;
	float valueOfChange, localData; /* user can change just one value at the moment */

	builder.Middle();

	ImGui::PushItemWidth(100.0f);
	/* Drawing is row-wise */
	for (int rows = 0; rows < 4; rows++)
	{
		for (int columns = 0; columns < 4; columns++)
		{
			localData = coreData[columns][rows]; /* Data are column-wise */
			if (drawDragFloatWithMap_Inline(&localData, 0,
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
		m_nodebase->setValue(valueOfChange, { columnOfChange, rowOfChange });
	}

	ImGui::Spring(0); 

}

void WorkspaceMatrixInversion::drawDataLabel(util::NodeBuilder& builder)
{
    builder.Middle();
    ImGui::Text(this->m_label.c_str());
    ImGui::Spring(0);
}
