#include "WorkspaceNormalizeVector.h"

WorkspaceNormalizeVector::WorkspaceNormalizeVector(ImTextureID headerBackground, std::string headerLabel = "default NormalizeVector header")
    : WorkspaceNodeWithCoreData(Builder::createNode<ENodeType::NormalizeVector>(), headerBackground, headerLabel)
{
}

void WorkspaceNormalizeVector::drawData(util::NodeBuilder& builder)
{
	const glm::vec4& coreData = Nodebase->getData().getVec4();
	bool valueCH = false;

	glm::vec4 localData = coreData;
	float localDataArray[4] = {0, 0, 0, 0};

	fromVecToArray4(localData, localDataArray);

	builder.Middle();

	ImGui::PushItemWidth(200.0f);
	valueCH = ImGui::InputFloat4("x", localDataArray, 3); /* \todo /"x"/ as some property of class? or from Const.h? */
	ImGui::PopItemWidth();

	if (valueCH)
	{
		fromArrayToVec4(localData, localDataArray);
		Nodebase->setValue(localData);
	}

	ImGui::Spring(0);
}

void WorkspaceNormalizeVector::drawNode(util::NodeBuilder& builder, Core::Pin* newLinkPin)
{
	builder.Begin(Id);

	drawHeader(builder);
	drawInputs(builder, newLinkPin);
	drawData(builder);
	drawOutputs(builder, newLinkPin);

	builder.End();
}
