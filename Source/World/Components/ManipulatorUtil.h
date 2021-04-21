#pragma once
#include "../GameObject.h"

#include "pgr.h"


class ManipulatorUtil
{
public:
	static void drawHandle(GameObject*_handle,glm::mat4 space,glm::vec4 color,int stencil,int active,int hover);
	static int getStencil(char at);

private:
	static bool s_stencilInit;
	static char s_stencilArr[7];
	//static char s_stencilx,s_stencily,s_stencilz,s_stencilzx,s_stencilzy,s_stencilyx,s_stencilxyz,s_stencilaxisx,s_stencilaxisy,s_stencilaxisz,s_stencilaxisw;
};

