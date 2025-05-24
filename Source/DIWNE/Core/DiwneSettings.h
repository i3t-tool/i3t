/**
 * \file
 * \brief
 * \author Dan Rakušan <rakusan.dan@gmail.com>
 * \copyright Copyright (C) 2016-2025 I3T team, Department of Computer Graphics
 * and Interaction, FEE, Czech Technical University in Prague, Czech Republic
 *
 * This file is part of I3T - An Interactive Tool for Teaching Transformations
 * http://www.i3t-tool.org
 *
 * GNU General Public License v3.0 (see LICENSE.txt or https://www.gnu.org/licenses/gpl-3.0.txt)
 */
#pragma once

namespace DIWNE
{
/**
 * Collection of various node editor settings. Style related options are handled by the style subsystem
 * @see DIWNE::Style
 */
struct SettingsDiwne
{
	float dpiScale = 1.0f;
	ImRect const workAreaDiwne = ImRect(0, 0, 0, 0); /**< workarea in Diwne coordinates (so what part of infinite space
	                                                      of node editor is on window) only initial value - mostly based
	                                                      on window size ( \see updateViewportRects() ) */

	float minWorkAreaZoom = 0.25;  /**< minimal value of zoom */
	float maxWorkAreaZoom = 4;     /**< maximal value of zoom */
	float workAreaInitialZoom = 1; /**< initial value of zoom */
	float zoomSpeed = 0.5f;
	float zoomSensitivity = 0.5f;

	bool showGrid = true;
	bool useDotGrid = true;

	/**
	 * Whether to wrap NodeEditor components in an ImGui child window.
	 * This is preferable when the node editor does not span the whole area of a parent window.
	 * But it prevents other ImGui components being overlaid on top of it, in such cases this can be disabled and
	 * the node editor can be wrapped in a child window manually.
	 */
	bool useChildWindow = true;

	float linkInteractionWidthDiwne = 7;
	ImVec2 linkStartControlOffsetDiwne = ImVec2(10, 0); // TODO: Turn into style variable, move calc from Worksp
	ImVec2 linkEndControlOffsetDiwne = ImVec2(-10, 0);  // TODO: Turn into style variable

	// Some new flags that are yet to be truly finalized, subject to be moved to a different place
	bool selectNodeOnDrag = false;
};
} // namespace DIWNE