#include "WorkspaceVector4.h"

WorkspaceVector4::WorkspaceVector4(ImTextureID headerBackground, std::string headerLabel, Ptr<Core::NodeBase> nodebase, WorkspaceViewScale viewScale)
    : WorkspaceNodeWithCoreData(nodebase, headerBackground, headerLabel, viewScale)
{}

void WorkspaceVector4::drawDataFull(util::NodeBuilder& builder)
{
	const glm::vec4& coreData = m_nodebase->getData().getVec4();
	int const coreMap[4] = {1,2,3,4}; /* \todo JH will be map for vector? */
	int const idOfNode = this->m_id.Get();

	bool valueChanged = false;
	/* \todo JH if same function setValue(value, position) as for Transformations will be added -> than this is probably better /same as in Matrix4x4/
	int columnOfChange;
	float localData, valueOfChange;
    */
    glm::vec4 localData;

	builder.Middle();

    ImGui::PushItemWidth(100.0f);
	for (int columns = 0; columns < 4; columns++)
    {
        localData[columns] = coreData[columns];
        valueChanged |= drawDragFloatWithMap_Inline(&localData[columns],
                                                    coreMap[columns],
                                                    fmt::format("##{}:{}", idOfNode, columns));

//        localData = coreData[columns];
//        if (drawDragFloatWithMap_Inline(&localData,
//                                        coreMap[columns],
//                                        fmt::format("##{}:{}", idOfNode, columns)))
//        {
//            valueChanged = true;
//            columnOfChange = columns;
//            valueOfChange = localData;
//        }
}
    ImGui::PopItemWidth();

	if (valueChanged)
	{
	    m_nodebase->setValue(localData);
//		Nodebase->setValue(valueOfChange, {columnOfChange});
	}

	ImGui::Spring(0);
}
