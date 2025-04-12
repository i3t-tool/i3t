#pragma once

namespace DIWNE
{
/** \brief Storage of all values
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

	float linkAlphaHovered = 1;         // TODO: Reimplement
	float linkAlpha = 0.2;              // TODO: Reimplement
	float linkAlphaSelectedHovered = 1; // TODO: Reimplement
	float linkAlphaSelected = 0.5;      // TODO: Reimplement

	// Some new flags that are yet to be truly finalized, subject to be moved to a different place
	bool selectNodeOnDrag = false;
};
} // namespace DIWNE