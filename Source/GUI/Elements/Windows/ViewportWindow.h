/**
 * \file	GUI/Elements/Windows/ViewportWindows.h
 *
 * A viewport element.
 */
#pragma once

#include "Core/World.h"
#include "GUI/Elements/IWindow.h"
#include <imgui.h>
#include "World2/World2.h"

class ViewportWindow : public IWindow
{
public:
	I3T_WINDOW(ViewportWindow)

	ViewportWindow(bool show, World* world,World2*world2);
	void render();

private:
	World* m_world;
	World2*m_world2;
	unsigned int m_fboMain;
	unsigned int m_texColBufMain;
	unsigned int m_rboMain;
	ImVec2 m_wcMin;
	ImVec2 m_wcMax;
};
