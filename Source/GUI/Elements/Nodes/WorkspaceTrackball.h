//
// Created by Sofie on 16.05.2021.
//

#pragma once
#include "WorkspaceMatrix4x4.h"

#include <World/Components/Camera.h>
#include <World/Components/Renderer.h>
#include <World/HardcodedMeshes.h>
#include <World/RenderTexture.h>

struct WorkspaceTrackballArgs
{
	WorkspaceLevelOfDetail levelOfDetail = WorkspaceLevelOfDetail::Full;
	std::string headerLabel = "default Trackball header";
	std::string nodeLabel = "Trackball";
	//TODO uncomment when MH make trackball in Core
	//Ptr<Core::NodeBase> nodebase = Core::Builder::createNode<ENodeType::Trackball>();
	Ptr<Core::NodeBase> nodebase = Core::Builder::createNode<ENodeType::MatrixToMatrix>();
};

class WorkspaceTrackball : public WorkspaceMatrix4x4
{
public:
	WorkspaceTrackball(ImTextureID headerBackground, WorkspaceTrackballArgs const& args);
	WorkspaceTrackball(ImTextureID headerBackground, std::string headerLabel = "Trackball", std::string nodeLabel = "Trackball");

	ImVec2 textureSize;
	ImVec2 buttonSize;

	ImRect texturePos;

	GLuint renderTexture;
	RenderTexture* rend;
	GameObject* trackball;
	Camera* cam;

	bool move;

	bool isTrackball();

	void drawDataSetValues(util::NodeBuilder& builder);
	void drawDataFull(util::NodeBuilder& builder, int index);
};
