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

#include "imgui.h"

#include "glm/matrix.hpp"

#include "GUI/Elements/IWindow.h"
#include "GUI/Viewport/ViewportWindowSettings.h"

#include "Viewport/data/DisplayOptions.h"
#include "Viewport/data/RenderOptions.h"


class ViewportModule;
namespace Vp
{
class AbstractCamera;
class Viewport;
class SceneRenderTarget;
} // namespace Vp

namespace UI
{
class ViewportWindow : public IWindow
{
public:
	I3T_WINDOW(ViewportWindow)

	ViewportWindowSettings m_settings;

	ViewportModule* m_module;

	struct ViewportSpace
	{
		bool standard = true;
		std::string label{"World space"};
		ImVec4 labelCol = ImVec4(1, 1, 1, 1);
		glm::mat4 m_referenceSpace{1.f};
		glm::mat4 m_referenceSpaceInv{1.f};
	};
	ViewportSpace m_space;

	std::shared_ptr<Vp::AggregateCamera> m_camera;

	ViewportWindow(ViewportModule* module, int index, bool show);
	void render() override;

	void updateSpace(); ///< Update state of the viewport reference matrix based on app context

private:
	Vp::Viewport* m_viewport;

	Vp::DisplayOptions m_displayOptions;
	Vp::RenderOptions m_renderOptions;

	std::shared_ptr<Vp::SceneRenderTarget> m_renderTarget;

	ImDrawListSplitter m_channelSplitter;

	bool showViewportButtons();
	bool showViewportMenu();
	bool showSpaceIndicators();
};
} // namespace UI
