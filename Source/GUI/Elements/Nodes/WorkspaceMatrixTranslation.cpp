#include "WorkspaceMatrixTranslation.h"

WorkspaceMatrixTranslation::WorkspaceMatrixTranslation(ImTextureID headerBackground, std::string headerLabel, WorkspaceViewScale viewScale)
    : WorkspaceMatrix4x4(headerBackground, headerLabel, Builder::createTransform<Core::Translation>(), viewScale )
{
}

void WorkspaceMatrixTranslation::drawData(util::NodeBuilder& builder)
{
    switch(m_viewScale)
    {
    case WorkspaceViewScale::Full:
        drawDataFull(builder); /* \todo JH here will be switch between different scale of view */
        break;
    case WorkspaceViewScale::JustChangeAble:
        drawDataJustChangAble(builder);
        break;

    default:
        /* \todo JH log about not supported viewScale - this should not happen since setViewScale /not implemented yet/ should not allow set some other than implemented viewScale */
        drawDataFull(builder);
    }

}

void WorkspaceMatrixTranslation::drawDataJustChangAble(util::NodeBuilder& builder)
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

