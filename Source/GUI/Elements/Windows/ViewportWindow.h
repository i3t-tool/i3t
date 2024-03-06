/**
 * \file
 * \brief
 * \author Dan Rakušan <rakusan.dan@gmail.com>
 * \copyright Copyright (C) 2016-2023 I3T team, Department of Computer Graphics
 * and Interaction, FEE, Czech Technical University in Prague, Czech Republic
 *
 * This file is part of I3T - An Interactive Tool for Teaching Transformations
 * http://www.i3t-tool.org
 *
 * GNU General Public License v3.0 (see LICENSE.txt or https://www.gnu.org/licenses/gpl-3.0.txt)
 */
#pragma once

#define IMGUI_DEFINE_MATH_OPERATORS
#include "imgui.h"

#include "GUI/Elements/IWindow.h"

#include "Viewport/data/DisplayOptions.h"
#include "Viewport/data/RenderOptions.h"

namespace Vp
{
class Viewport;
class SceneRenderTarget;
} // namespace Vp

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

	Vp::DisplayOptions m_displayOptions;
	Vp::RenderOptions m_renderOptions;

	std::shared_ptr<Vp::SceneRenderTarget> m_renderTarget;

	ImDrawListSplitter m_channelSplitter;

	bool showViewportMenu();
};
} // namespace UI
