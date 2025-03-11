#pragma once

namespace DIWNE
{
/** \brief Storage of all values
 */
struct SettingsDiwne
{
	// TODO: Unused? DiwneObject id should cover this? Remove since ids are generated automatically now.
	std::string const editorlabel = "NodeEditor";    /**< as well as all other DiwneObject, Diwne has to
	                                                         have identification label too */
	ImRect const workAreaDiwne = ImRect(0, 0, 0, 0); /**< workarea in Diwne coordinates (so what part of infinite space
	                                                      of node editor is on window) only initial value - mostly based
	                                                      on window size ( \see updateViewportRects() ) */

	float minWorkAreaZoom = 0.25;  /**< minimal value of zoom */
	float maxWorkAreaZoom = 4;     /**< maximal value of zoom */
	float workAreaInitialZoom = 1; /**< initial value of zoom */
	float zoomSpeed = 0.5f;
	float zoomSensitivity = 0.5f;
	float selectionRounding = 0;     /**< rounding od selection */
	float mouseDragThreshold = 2.0f; // 6.0 is ImGui default

	ImVec2 initPopupPosition = ImVec2(0, 0); /**< where to show popup when not set later */

	ImVec4 selectionRectFullColor = ImVec4(0.0, 0.0, 1.0, 0.1);
	ImVec4 selectionRectTouchColor = ImVec4(0.0, 1.0, 0.0, 0.1);
	float selectionRectBorderAlpha = 0.8f;

	ImVec4 itemSelectedBorderColor = ImVec4(1.0, 0.9, 0.4, 0.6);
	float itemSelectedBorderThicknessDiwne = 2.5;

	ImVec4 objectHoverBorderColor = ImVec4(0.0, 0.0, 0.0, 0.20);
	float objectHoverBorderThicknessDiwne = 1.5;
	ImVec4 objectFocusForInteractionBorderColor = ImVec4(0.0, 0.0, 0.0, 0.20);
	float objectFocusForInteractionBorderThicknessDiwne = 1.5;

	float middleAlign = 0.5; /**< value < 0 , 1 > where is horizontal position of
	                            middle of nodes */

	ImVec4 pinHoveredBorderColor = ImVec4(0.35, 0.35, 0.0, 1.0);
	float pinHoveredBorderThicknessDiwne = 2;
	ImVec4 nodeHoveredBorderColor = ImVec4(0.35, 0.35, 0.0, 1.0);
	float nodeHoveredBorderThicknessDiwne = 2;
	ImVec4 backgroundHoveredBorderColor = ImVec4(0.35, 0.35, 0.0, 1.0);
	float backgroundHoveredBorderThicknessDiwne = 2;

	float linkInteractionWidthDiwne = 7;
	ImVec2 linkStartControlOffsetDiwne = ImVec2(10, 0);
	ImVec2 linkEndControlOffsetDiwne = ImVec2(-10, 0);
	float linkAlphaHovered = 1;
	float linkAlpha = 0.2;
	float linkAlphaSelectedHovered = 1;
	float linkAlphaSelected = 0.5;
	// TODO: Font color is unused, was deleted, reimplement if needed
	ImVec4 fontColor = ImVec4(1.0f, 1.0f, 1.0f, 1.0f); ///< Color of the text in the node

	// Some new flags that are yet to be truly finalized, subject to be moved to a different place
	bool selectNodeOnDrag = false;
};
} // namespace DIWNE