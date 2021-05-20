//
// Created by Sofie on 18.05.2021.
//

#include "WorkspaceScreen.h"
#include <World/RenderTexture.h>

WorkspaceScreen::WorkspaceScreen(ImTextureID headerBackground, WorkspaceScreenArgs const& args)
		: WorkspaceFloat(headerBackground, {.levelOfDetail = args.levelOfDetail, .headerLabel = args.headerLabel, .nodeLabel = args.nodeLabel, .nodebase = args.nodebase })
{}

WorkspaceScreen::WorkspaceScreen(ImTextureID headerBackground, std::string headerLabel, std::string nodeLabel)
		: WorkspaceFloat(headerBackground, Core::Builder::createNode<ENodeType::Screen>(), headerLabel, nodeLabel)
{}

void WorkspaceScreen::drawDataSetValues(util::NodeBuilder& builder)
{
	drawDataFull(builder,0);
}

void WorkspaceScreen::drawDataFull(util::NodeBuilder& builder, int index)
{



	//No data in Core?
	auto camera = m_nodebase->getData();

	ImVec2 textureSize = ImVec2(50.0f, 50.0f);

	//GLuint renderTexture;
	//RenderTexture* rend = new RenderTexture(&renderTexture,256,256);

	//ImGui::Image(rend, textureSize,ImVec2(0.0f,0.0f),textureSize);
}
