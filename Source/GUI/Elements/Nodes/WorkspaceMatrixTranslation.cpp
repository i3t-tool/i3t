#include "WorkspaceMatrixTranslation.h"

WorkspaceMatrixTranslation::WorkspaceMatrixTranslation(ImTextureID headerBackground, WorkspaceMatrixTranslationArgs const& args)
    : WorkspaceMatrix4x4(headerBackground, {.viewScale=args.viewScale, .headerLabel=args.headerLabel, .nodeLabel=args.nodeLabel, .nodebase=args.nodebase})
{
}

WorkspaceMatrixTranslation::WorkspaceMatrixTranslation(ImTextureID headerBackground, Ptr<Core::NodeBase> nodebase, std::string headerLabel, std::string nodeLabel)
    : WorkspaceMatrix4x4(headerBackground, nodebase, headerLabel, nodeLabel)
{
}

void WorkspaceMatrixTranslation::drawData(util::NodeBuilder& builder)
{
    switch(m_viewScale)
    {
    case WorkspaceViewScale::Full:
        drawDataFull(builder); /* \todo JH here will be switch between different scale of view */
        break;
    case WorkspaceViewScale::SetValues:
        drawDataSetValues(builder);
        break;
    case WorkspaceViewScale::Label:
        drawDataLabel(builder);
        break;

    default:
        /* \todo JH log about not supported viewScale - this should not happen since setViewScale /not implemented yet/ should not allow set some other than implemented viewScale */
        drawDataFull(builder);
    }

}

void WorkspaceMatrixTranslation::drawDataSetValues(util::NodeBuilder& builder)
{
    const glm::mat4& coreData = m_nodebase->getData().getMat4();
	const Core::Transform::DataMap& coreMap = m_nodebase->getDataMap();
	int const idOfNode = this->m_id.Get();

	bool valueChanged = false;
	int rowOfChange, columnOfChange = 3;
	float valueOfChange, localData; /* user can change just one value at the moment */

	builder.Middle();

	ImGui::PushItemWidth(100.0f);
	/* Drawing is row-wise */
	for (int rows = 0; rows < 3; rows++)
	{
        localData = coreData[columnOfChange][rows]; /* Data are column-wise */
        if (drawDragFloatWithMap_Inline(&localData,
                                        coreMap[columnOfChange*4+rows],
                                        fmt::format("##{}:r{}c{}", idOfNode, rows, columnOfChange)))
        {
            valueChanged = true;
            rowOfChange = rows;
            valueOfChange = localData;
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

void WorkspaceMatrixTranslation::drawDataLabel(util::NodeBuilder& builder)
{
    builder.Middle();
    ImGui::Text(this->m_label.c_str());
    ImGui::Spring(0);
}
