/**
 * \file	GUI/Elements/Windows/ViewportWindows.h
 *
 * A viewport element.
 */
#pragma once

#include <imgui.h>

#include "GUI/Elements/IWindow.h"

#include "Viewport/data/DisplayOptions.h"
#include "Viewport/data/RenderOptions.h"

namespace Vp
{
class Viewport;
}

namespace UI
{
class ViewportWindow : public IWindow
{
public:
	I3T_WINDOW(ViewportWindow)

	ViewportWindow(bool show, Vp::Viewport* viewport);
	void render() override;

private:
	Vp::Viewport* m_viewport;

	Vp::DisplayOptions displayOptions;
	Vp::RenderOptions renderOptions;

	void showViewportMenu();
};
} // namespace UI
