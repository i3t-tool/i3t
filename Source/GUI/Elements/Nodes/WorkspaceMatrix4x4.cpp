#include "WorkspaceMatrix4x4.h"
// #include "Source/Core/Nodes/Node.h"

WorkspaceMatrix4x4::WorkspaceMatrix4x4(ImTextureID headerBackground, std::string headerLabel = "default Matrix4x4 header")
    : WorkspaceNodeWithCoreData(Builder::createNode<ENodeType::Matrix>(), headerBackground, headerLabel)
{
}

void WorkspaceMatrix4x4::drawData(util::NodeBuilder& builder)
{

    //const glm::mat4& coreData = Nodebase->getInternalData().getMat4();

	const glm::mat4& coreData = Nodebase->getData().getMat4();
	const Core::Transform::DataMap& coreMap = Nodebase->getDataMap();

	bool valueChanged = false;

	glm::mat4 localData;
	for (int rows = 0; rows < 4; rows++)
	{
		for (int columns = 0; columns < 4; columns++)
		{
			localData[rows][columns] = coreData[rows][columns];
		}
	}

	builder.Middle();

	ImGui::PushItemWidth(50.0f);
	for (int rows = 0; rows < 4; rows++)
	{
		for (int columns = 0; columns < 4; columns++)
		{

		    valueChanged |= drawDragFloatWithMap_Inline(&localData[rows][columns],
                                                        coreMap[rows*4+columns],
                                                        fmt::format("##{}-{}", rows, columns));
		}
		ImGui::NewLine();
	}
	ImGui::PopItemWidth();

	if (valueChanged)
	{
		Nodebase->setValue(localData);
	}

	ImGui::Spring(0); /* \todo JH ehat is Spring? */
}
