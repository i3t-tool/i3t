//
// Created by Sofie on 18.05.2021.
//
#pragma once
#include "WorkspaceFloat.h"

#include <World/Components/Camera.h>
#include <World/Components/Renderer.h>
#include <World/HardcodedMeshes.h>
#include <World/RenderTexture.h>
#include "Core/Nodes/GraphManager.h"

class WorkspaceScreen : public WorkspaceFloat /* \todo JH really float?*/
{
    GLuint renderTexture;
	RenderTexture* rend;
	Camera* cam;
public:



	WorkspaceScreen();

	bool drawDataFull(DIWNE::Diwne& diwne, int index);
};
