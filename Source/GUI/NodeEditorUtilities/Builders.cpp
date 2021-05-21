//------------------------------------------------------------------------------
// LICENSE
//   This software is dual-licensed to the public domain and under the following
//   license: you are granted a perpetual, irrevocable license to copy, modify,
//   publish, and distribute this file as you see fit.
//
// CREDITS
//   Written by Michal Cichon
//------------------------------------------------------------------------------
#include "Builders.h"
#define IMGUI_DEFINE_MATH_OPERATORS

#include <imgui_internal.h>

//------------------------------------------------------------------------------
namespace ed = ax::NodeEditor;
namespace util = ax::NodeEditor::Utilities;

util::NodeBuilder::NodeBuilder(ImTextureID texture, int textureWidth, int textureHeight):
			HeaderTextureId(texture),
			HeaderTextureWidth(textureWidth),
			HeaderTextureHeight(textureHeight),
			CurrentNodeId(0),
			CurrentStage(Stage::Invalid),
			HasHeader(false)
{
}

void util::NodeBuilder::Begin(ed::NodeId id)
{
	HasHeader = false;
	HeaderMin = HeaderMax = ImVec2();

	ed::PushStyleVar(StyleVar_NodePadding, I3T::getColor(EColor::Builder_NodePadding)); // ImVec4(8, 4, 8, 8) ImVec4(0, 0, 0, 0)

	ed::BeginNode(id);

	ImGui::PushID(id.AsPointer());
	CurrentNodeId = id;

	SetStage(Stage::Begin);
}

void util::NodeBuilder::End()
{
	SetStage(Stage::End);

	ed::EndNode();

	if (ImGui::IsItemVisible())
	{
		auto alpha = static_cast<int>(255 * ImGui::GetStyle().Alpha);

		auto drawList = ed::GetNodeBackgroundDrawList(CurrentNodeId);

		const auto halfBorderWidth = ed::GetStyle().NodeBorderWidth * 0.5f;	//0.5f

		auto headerColor = IM_COL32(0, 0, 0, alpha) | (HeaderColor & IM_COL32(255, 255, 255, 0));
		if ((HeaderMax.x > HeaderMin.x) && (HeaderMax.y > HeaderMin.y) && HeaderTextureId)
		{
			const auto uv = ImVec2((HeaderMax.x - HeaderMin.x) / (float)(4.0f * HeaderTextureWidth),
			                       (HeaderMax.y - HeaderMin.y) / (float)(4.0f * HeaderTextureHeight));

			ImVec4 builderpadd = I3T::getColor(EColor::Builder_NodePadding);

			//Header rect
			drawList->AddImageRounded(HeaderTextureId,
			                          HeaderMin - ImVec2(builderpadd.x - halfBorderWidth, builderpadd.y - halfBorderWidth),
			                          HeaderMax + ImVec2(builderpadd.x - halfBorderWidth, 0),
			                          ImVec2(0.0f, 0.0f), uv,
			                          headerColor, GetStyle().NodeRounding, 1 | 2);

			/*drawList->AddImageRounded(HeaderTextureId,
			  HeaderMin - ImVec2(8 - halfBorderWidth, 4 - halfBorderWidth),
			  HeaderMax + ImVec2(8 - halfBorderWidth, 0), ImVec2(0.0f, 0.0f), uv,
			  headerColor, GetStyle().NodeRounding, 1 | 2);*/

			auto headerSeparatorMin = ImVec2(HeaderMin.x, HeaderMax.y);
			auto headerSeparatorMax = ImVec2(HeaderMax.x, HeaderMin.y);

			if ((headerSeparatorMax.x > headerSeparatorMin.x) && (headerSeparatorMax.y > headerSeparatorMin.y))
			{
				drawList->AddLine(
						  headerSeparatorMin + ImVec2(-(builderpadd.x - halfBorderWidth), -0.0f),
              headerSeparatorMax + ImVec2((builderpadd.x - halfBorderWidth), -0.0f),
              ImColor(255, 255, 255, 96 * alpha / (3 * 255)), 1.0f);

				/*drawList->AddLine(
						headerSeparatorMin + ImVec2(-(8 - halfBorderWidth), -0.5f),
						headerSeparatorMax + ImVec2((8 - halfBorderWidth), -0.5f),
						ImColor(255, 255, 255, 96 * alpha / (3 * 255)), 1.0f);*/
			}
		}
	}

	CurrentNodeId = 0;

	ImGui::PopID();

	ed::PopStyleVar();

	SetStage(Stage::Invalid);
}

void util::NodeBuilder::Header(const ImVec4& color)
{
	HeaderColor = ImColor(color);
	SetStage(Stage::Header);
}

void util::NodeBuilder::EndHeader()
{
	SetStage(Stage::Content);
}

void util::NodeBuilder::Input(ed::PinId id, ImColor color)
{
	if (CurrentStage == Stage::Begin)
		SetStage(Stage::Content);

	const auto applyPadding = (CurrentStage == Stage::Input);

	SetStage(Stage::Input);

	if (applyPadding) {
		ImGui::Spring(0);
	}
	Pin(id, PinKind::Input, color);

	//SS

	ImGui::BeginHorizontal(id.AsPointer());
  ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, I3T::getSize(ESizeVec2::Builder_ItemSpacing));
}

void util::NodeBuilder::EndInput()
{
	ImGui::PopStyleVar();
	ImGui::EndHorizontal();

	EndPin();
}

void util::NodeBuilder::Middle()
{
	if (CurrentStage == Stage::Begin)
		SetStage(Stage::Content);

	SetStage(Stage::Middle);
}

void util::NodeBuilder::Output(ed::PinId id, ImColor color)
{
	if (CurrentStage == Stage::Begin)
		SetStage(Stage::Content);

	const auto applyPadding = (CurrentStage == Stage::Output);

	SetStage(Stage::Output);

	if (applyPadding)
		ImGui::Spring(0);

	Pin(id, PinKind::Output, color);

	//SS

	ImGui::BeginHorizontal(id.AsPointer());
  ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, I3T::getSize(ESizeVec2::Builder_ItemSpacing));
}

void util::NodeBuilder::EndOutput()
{
	ImGui::PopStyleVar();
	ImGui::EndHorizontal();

	EndPin();
}

bool util::NodeBuilder::SetStage(Stage stage)
{
	if (stage == CurrentStage)
		return false;

	auto oldStage = CurrentStage;
	CurrentStage = stage;

	ImVec2 cursor;
	switch (oldStage)
	{
	case Stage::Begin:
		break;

	case Stage::Header:
		ImGui::EndHorizontal();
		HeaderMin = ImGui::GetItemRectMin();
		HeaderMax = ImGui::GetItemRectMax();

		// spacing between header and content
		ImGui::Spring(0, ImGui::GetStyle().ItemSpacing.y * 1.0f);	//2.0f

		break;

	case Stage::Content:
		break;

	case Stage::Input:
		ed::PopStyleVar(2);

		ImGui::Spring(1, 0);
		ImGui::EndVertical();

		// #debug
		// ImGui::GetWindowDrawList()->AddRect(
		//     ImGui::GetItemRectMin(), ImGui::GetItemRectMax(), IM_COL32(255, 0, 0, 255));

		break;

	case Stage::Middle:
		ImGui::EndVertical();

		// #debug
		// ImGui::GetWindowDrawList()->AddRect(
		//     ImGui::GetItemRectMin(), ImGui::GetItemRectMax(), IM_COL32(255, 0, 0, 255));

		break;

	case Stage::Output:
		ed::PopStyleVar(2);

		//ImGui::Spring(1, 0);
		ImGui::EndVertical();

		// #debug
		// ImGui::GetWindowDrawList()->AddRect(
		//     ImGui::GetItemRectMin(), ImGui::GetItemRectMax(), IM_COL32(255, 0, 0, 255));

		break;

	case Stage::End:
		break;

	case Stage::Invalid:
		break;
	}

	switch (stage)
	{
	case Stage::Begin:
		ImGui::BeginVertical("node");
		break;

	case Stage::Header:
		HasHeader = true;

		ImGui::BeginHorizontal("header");
		break;

	case Stage::Content:
		//if (oldStage == Stage::Begin)
			//ImGui::Spring(0);

		ImGui::BeginHorizontal("content");
		break;

	case Stage::Input:
		ImGui::BeginVertical("inputs", I3T::getSize(ESizeVec2::Nodes_InputsSize), I3T::getSize(ESize::Nodes_InputsAlignment));

		ed::PushStyleVar(ed::StyleVar_PivotAlignment, I3T::getSize(ESizeVec2::Nodes_PivotAlignment));
		ed::PushStyleVar(ed::StyleVar_PivotSize, I3T::getSize(ESizeVec2::Nodes_PivotSize));

		if (!HasHeader)
			ImGui::Spring(1, 0);
		break;

	case Stage::Middle:
		//ImGui::Spring(2,2);
		ImGui::BeginVertical("middle", I3T::getSize(ESizeVec2::Nodes_MiddleSize), I3T::getSize(ESize::Nodes_MiddleAlignment)); //SS
		break;

	case Stage::Output:
		if (oldStage == Stage::Middle || oldStage == Stage::Input)
			ImGui::Spring(1);
		else
			ImGui::Spring(1, 0);
		ImGui::BeginVertical("outputs", I3T::getSize(ESizeVec2::Nodes_OutputSize), I3T::getSize(ESize::Nodes_OutputsAlignment));

		ed::PushStyleVar(ed::StyleVar_PivotAlignment, ImVec2(1.0f, 0.5f));//Don't touch! or you  will break up links!
		ed::PushStyleVar(ed::StyleVar_PivotSize,  I3T::getSize(ESizeVec2::Nodes_PivotSize));

		if (!HasHeader)
			ImGui::Spring(1, 0);
		break;

	case Stage::End:
		if (oldStage == Stage::Input)
			ImGui::Spring(1, 0);
    if (oldStage == Stage::Content)
      ImGui::Spring(2, I3T::getSize(ESize::Nodes_rightSideSpacing));  //spring from right side
		if (oldStage != Stage::Begin)
			ImGui::EndHorizontal();
		ContentMin = ImGui::GetItemRectMin();
		ContentMax = ImGui::GetItemRectMax();

		ImGui::Spring(0);
		ImGui::EndVertical();
		NodeMin = ImGui::GetItemRectMin();
		NodeMax = ImGui::GetItemRectMax();
		break;

	case Stage::Invalid:
		break;
	}

	return true;
}

void util::NodeBuilder::Pin(ed::PinId id, ed::PinKind kind, ImColor color)
{
	ed::BeginPin(id, kind, color);
}

void util::NodeBuilder::EndPin()
{
	ed::EndPin();

	// #debug
	//ImGui::GetWindowDrawList()->AddRectFilled(
	//ImGui::GetItemRectMin(), ImGui::GetItemRectMax(), IM_COL32(255, 0, 0, 64));
}
