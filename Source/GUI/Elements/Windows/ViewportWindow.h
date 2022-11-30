/**
 * \file	GUI/Elements/Windows/ViewportWindows.h
 *
 * A viewport element.
 */
#pragma once

#include <imgui.h>

#include "GUI/Elements/IWindow.h"
#include "Viewport/Framebuffer.h"
#include "Viewport/Viewport.h"

class World;
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

	ViewportWindow(bool show, World* world, Vp::Viewport* viewport);
	void render() override;

private:
	World* m_world;
	Vp::Viewport* m_viewport;

	std::unique_ptr<Vp::Framebuffer> m_framebuffer;

	ImVec2 m_wcMin;
	ImVec2 m_wcMax;

	void showViewportsMenu();
};
} // namespace UI
