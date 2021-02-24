/**
 * \file	GUI/Elements/Windows/ViewportWindows.h
 *
 * A viewport element.
 */
#pragma once

#include "Core/World.h"
#include "GUI/Elements/IWindow.h"
#include <imgui.h>

class ViewportWindow : public IWindow
{
public:
	ViewportWindow(bool show, World* world);
	void render();

private:
	World* m_world;
	unsigned int m_fboMain;
	unsigned int m_texColBufMain;
	unsigned int m_rboMain;
	ImVec2 m_wcMin;
	ImVec2 m_wcMax;
};
