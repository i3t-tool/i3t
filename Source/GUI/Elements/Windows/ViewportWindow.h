/**
 * \file	GUI/Elements/Windows/ViewportWindows.h
 *
 * A viewport element.
 */
#pragma once

#include <imgui.h>

#include "GUI/Elements/IWindow.h"
#include "World2/World2.h"

namespace UI
{
class Viewport : public IWindow
{
public:
	I3T_WINDOW(Viewport)

	Viewport(bool show, World2* world2);
	void render();

private:
	World2* m_world2;
	unsigned int m_fboMain;
	unsigned int m_texColBufMain;
	unsigned int m_rboMain;
	ImVec2 m_wcMin;
	ImVec2 m_wcMax;
};
} // namespace UI
