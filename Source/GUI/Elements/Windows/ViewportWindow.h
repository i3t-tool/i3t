/**
 * \file	GUI/Elements/Windows/ViewportWindows.h
 *
 * A viewport element.
 */
#pragma once

#include <imgui.h>

#include "GUI/Elements/IWindow.h"

#include "Viewport/scene/DisplayOptions.h"
#include "Viewport/scene/RenderOptions.h"

class World;

namespace UI
{
class ViewportWindow : public IWindow
{
public:
	I3T_WINDOW(ViewportWindow)

	ViewportWindow(bool show, World* world);
	void render() override;

private:
	World* m_world;

	Vp::DisplayOptions displayOptions;
	Vp::RenderOptions renderOptions;

	ImVec2 m_wcMin;
	ImVec2 m_wcMax;

	void showViewportMenu();
};
} // namespace UI
