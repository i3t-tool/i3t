#include "WorkspaceMatrix4x4.h"
// #include "Source/Core/Nodes/Node.h"

// WorkspaceMatrix4x4::WorkspaceMatrix4x4(ImTextureID headerBackground, std::string headerLabel = "default Matrix4x4
// header")
//    : WorkspaceNodeWithCoreData(Builder::createNode<ENodeType::Matrix>(), headerBackground, headerLabel)
//{
//}

WorkspaceMatrix4x4::WorkspaceMatrix4x4(ImTextureID headerBackground, std::string headerLabel,
                                       Ptr<Core::NodeBase> nodebase)
		: WorkspaceNodeWithCoreData(nodebase, headerBackground, headerLabel)
{
}

void WorkspaceMatrix4x4::drawData(util::NodeBuilder& builder)
{
	drawDataFull(builder); /* default function always draw all data */
}

void WorkspaceMatrix4x4::drawDataFull(util::NodeBuilder& builder)
{
	const glm::mat4& coreData = Nodebase->getData().getMat4();
	const Core::Transform::DataMap& coreMap = Nodebase->getDataMap();
	int const idOfNode = this->Id.Get();

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
		Nodebase->setValue(valueOfChange, {columnOfChange, rowOfChange});
	}

	ImGui::Spring(0); /* \todo JH what is Spring? */
}
