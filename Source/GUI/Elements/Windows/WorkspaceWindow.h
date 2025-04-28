/**
 * \file
 * \brief
 * \authors Sofie Sasorina, Jaroslav Holecek
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
#include "imgui_internal.h"

#include <algorithm>
#include <map>
#include <math.h>
#include <memory>
#include <string>
#include <utility>
#include <vector>

#include "glm/glm.hpp"

#include "Config.h"
#include "Core/Result.h"
#include "I3T.h"
#include "Logger/Logger.h"

#include "GUI/Elements/IWindow.h"


#include "GUI/Workspace/WorkspaceDiwne.h"

/**
 * \brief Class for Workspace window object, stores everything what Workspace window need
 */
class WorkspaceWindow : public IWindow
{
public:
	I3T_WINDOW(WorkspaceWindow)

	explicit WorkspaceWindow(bool show);

public:
	Application& m_wholeApplication;

	void render() override;

private:
	ImDrawListSplitter m_channelSplitter;

	bool showQuickAddButtons();

	float m_trackingSliderProgress = 0.f;
	ImRect m_trackingBox;
	bool showTrackingTimeline();

	void showMenuBar();
	void showEditMenu();
	void showAddMenu();
	void showViewMenu();
	void showDebugMenu();
	void showDiwneStyleMenu();

	bool TrackingSlider(Core::MatrixTracker* tracker, const char* label, void* p_data, Core::TrackingDirection dir);
	bool TrackingSlider(Core::MatrixTracker* tracker, const char* label, ImGuiDataType data_type, void* p_data,
	                    const void* p_min, const void* p_max, const char* format = NULL, ImGuiSliderFlags flags = 0,
	                    float minHeight = 0);
	void TrackingSlider_drawTick(float tParam, const ImRect& trackRect, bool leftToRight, const ImVec4& tickColor);
	void TrackingSlider_drawArrow(float tParam, const ImRect& trackRect, bool leftToRight, const ImVec4& color);
	void TrackingSlider_drawRect(float tParam, float tStep, const ImRect& trackRect, bool leftToRight,
	                             const ImVec4& color);
};
