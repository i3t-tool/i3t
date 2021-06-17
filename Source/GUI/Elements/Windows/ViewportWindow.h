/**
 * \file	GUI/Elements/Windows/ViewportWindows.h
 *
 * A viewport element.
 */
#pragma once

#include <imgui.h>

#include "GUI/Elements/IWindow.h"
#include "World/World.h"

namespace UI
{
class Viewport : public IWindow
{
public:
	I3T_WINDOW(Viewport)

	Viewport(bool show, World* world);
	void render();
	InputController getInput();

private:
	World* m_world;
	unsigned int m_fboMain;
	unsigned int m_texColBufMain;
	unsigned int m_rboMain;
	ImVec2 m_wcMin;
	ImVec2 m_wcMax;
};
} // namespace UI
