//
// Created by Sofie on 16.05.2021.
//

#pragma once
#include "WorkspaceMatrix4x4.h"

#include <World/Components/Camera.h>
#include <World/Components/Renderer.h>
#include <World/HardcodedMeshes.h>
#include <World/RenderTexture.h>

class WorkspaceTrackball : public WorkspaceMatrix4x4 /* \todo JH really matrix 4x4 ?*/
{
public:
    WorkspaceTrackball();

	ImVec2 textureSize;
	ImVec2 buttonSize;

	ImRect texturePos;

	GLuint renderTexture;
	RenderTexture* rend;
	GameObject* trackball;
	Camera* cam;

	bool move;

	bool isTrackball();

    bool drawDataFull(DIWNE::Diwne& diwne, int index);
};
