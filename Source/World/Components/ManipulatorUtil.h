#pragma once
/**
 * \file ManipulatorUtil.h
 * \author Daniel Gruncl
 *
 * Utils for drawing manipulator interaction handles.
 */
#include "../GameObject.h"

#include "pgr.h"

class ManipulatorUtil
{
public:
	static void drawHandle(GameObject* _handle, glm::mat4 space, glm::vec4 color, int stencil, int active, int hover);
	/*
	 * \fn getstencil(char at)
	 *
	 * Allows to share same stencil values for multiple users (wrapper around
	 * Select.cpp) \param[in] index in array of predefined stencils, length 7
	 * \return stencil value, or -1 if array out of index
	 *
	 */
	static int getStencil(char at);
	static void hint(const char* label);
	static void hintAt(const char* label, glm::vec2 pos);

private:
	static bool s_stencilInit;
	static char s_stencilArr[7];
	// static char
	// s_stencilx,s_stencily,s_stencilz,s_stencilzx,s_stencilzy,s_stencilyx,s_stencilxyz,s_stencilaxisx,s_stencilaxisy,s_stencilaxisz,s_stencilaxisw;
};
