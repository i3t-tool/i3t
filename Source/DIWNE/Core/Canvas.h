#pragma once

#include <string>

#include "diwne_imgui.h"

namespace DIWNE
{
class NodeEditor;
enum IconType : unsigned int;
struct IconStyle;

// TODO: (DR) Canvas and style zooming COULD be replaced with context switching and DrawList magic
//  See gitlab issue #339. Could bring huge benefit and simplify much of the code (avoids integer UI rounding).
//  However it's a big change, supporting both methods, while possible, is extremely impractical
//  So an experimental branch would need to be thoroughly tested before implementing, as it would be hard to go back
//  That is unless we did a slow transition, keeping the old zoom code and just fix it to 1x
//  In theory the Canvas class could have to implementations.
// TODO: The TODO above MIGHT be a prerequisite for Multi-View support.
//  With the way we zoom and pan right now, it might be impossible to keep two views at different zoom levels without
//  some major overhead / messy instancing.

/**
 * Representation of the 2D infinite plane of a node editor.
 *
 * // TODO: Coordinate systems docs (screen space, viewport coordinates, canvas/editor/diwne coordinates
 */
class Canvas
{
protected:
	NodeEditor& editor;

public:
	/**
	 * Bounds of the canvas viewport in canvas/editor's coordinates (diwne coordinates).
	 * The top left corner (.Min) can be set manually, the bottom right corner (.Max) is determined by available space.
	 */
	ImRect m_viewRectDiwne;
	ImRect m_viewRectScreen; ///< Viewport bounds in screen space coordinates.
	float m_zoom;            ///< Viewport zoom factor (scale ratio between diwne and screen coordinates).
	float m_prevZoom;        ///< Last frame zoom factor

	bool m_zoomScalingApplied = false; ///< Whether zoom UI scaling has been applied or not to Dear ImGui
	float m_unscaledFontSize{16.f};    ///< Original font size
protected:
	float m_zoomOriginalFontScale;  ///< Original font scale
	ImGuiStyle m_zoomOriginalStyle; ///< Original ImGui style

public:
	Canvas(NodeEditor& editor);

	void moveViewportZoomed(const ImVec2& distance);
	void moveViewport(const ImVec2& distance);

	/// Update screen and diwne positions and sizes of viewport.
	void updateViewportRects();

	ImRect getViewportRectDiwne() const;
	void setViewportRectDiwne(ImRect rect);

	ImRect getViewportRectScreen() const;

	// Zoom scaling
	// =============================================================================================================

	float getZoom() const;
	virtual void setZoom(float val);

	/**
	 * Modifies the current ImGuiStyle and Font depending on the current diwne zoom level.
	 * @return Whether zoom scaling was active before making this call
	 */
	bool applyZoomScaling();

	/**
	 * Restores the current ImGuiStyle and Font back to its state before applyZoomScaling() was last called.
	 * @return Whether zoom scaling was active before making this call
	 */
	bool stopZoomScaling();

	/**
	 * Ensure that zoom scaling is active or not based on a passed parameter
	 * @return Whether zoom scaling was active before making this call, can be later passed to this method again to
	 * restore original state
	 */
	bool ensureZoomScaling(bool active);

	/**
	 * Modifies the current Font depending on the current diwne zoom level.
	 * @return Whether zoom scaling was active before making this call
	 */
	float applyZoomScalingToFont(ImFont* font, float scaleMultiplier);

	/// @see applyZoomScalingToFont
	void stopZoomScalingToFont(ImFont* font, float originalScale);

protected:
	void ScaleAllSizes(ImGuiStyle& style, float scale_factor);

public:
	// Coordinate conversion
	// =============================================================================================================
	// ImVec2 transformFromImGuiToDiwne(const ImVec2& point) const;
	// ImVec2 transformFromDiwneToImGui(const ImVec2& point) const;
	// ImVec4 transformFromImGuiToDiwne(const ImVec4& point) const;
	// ImVec4 transformFromDiwneToImGui(const ImVec4& point) const;

	// TODO: Rename workArea in the methods below to viewport
	// Essentially, we have 3 coordinate systems:
	// 1. Screen - Coordinates in screen space of the display device
	// 2. Viewport - Coordinates in screen space, but with origin in the top left corner of the node editor component
	// 3. Diwne - Coordinates in canvas (infinite plane) space, eg. of the node editor's contents affected by zoom.
	//          - Conversion between Viewport and Diwne coordinates involves the monitor/window dpi scaling factor

	ImVec2 screen2workArea(const ImVec2& point) const;
	ImVec2 workArea2screen(const ImVec2& point) const;
	ImVec2 diwne2workArea(const ImVec2& point) const;
	ImVec2 workArea2diwne(const ImVec2& point) const;
	ImVec2 screen2diwne(const ImVec2& point) const;
	ImVec2 diwne2screen(const ImVec2& point) const;

	ImVec2 diwne2screenTrunc(const ImVec2& point) const; ///< Convert diwne point to pixel aligned screen position.

	ImRect diwne2screen(const ImRect& rect) const;      ///< Convert diwne rect to a screen rect
	ImRect screen2diwne(const ImRect& rect) const;      ///< Convert screen rect to a diwne rect
	ImRect diwne2screenTrunc(const ImRect& rect) const; ///< Convert diwne rect to pixel aligned screen rect.

	float diwne2screenSize(float size) const; ///< Apply diwne scaling factors to a float size
	float screen2diwneSize(float size) const; ///< Revert diwne scaling factors from a float size

	float diwne2screenSizeTrunc(float size) const;

	ImVec2 diwne2screenSize(const ImVec2& point) const; ///< Apply diwne scaling factors to an ImVec2 size
	ImVec2 screen2diwneSize(const ImVec2& point) const; ///< Revert diwne scaling factors from a ImVec2 size

	// Basic drawing utilities
	// =============================================================================================================
	void AddLine(const ImVec2& p1, const ImVec2& p2, const ImVec4& col, float thickness = 1.0f,
	             bool ignoreZoom = false) const;

	/**
	 * Draw filled rectangle to window ImDrawlist
	 * This method accepts DIWNE coordinates and converts them to truncated screen coordinates.
	 * @param p_min bottom left corner in diwne coords
	 * @param p_max top right corner in diwne coords
	 * @param col
	 * @param rounding
	 * @param rounding_corners
	 */
	void AddRectFilledDiwne(const ImVec2& p_min, const ImVec2& p_max, const ImVec4& col, float rounding = 0.0f,
	                        ImDrawFlags rounding_corners = ImDrawFlags_RoundCornersAll, bool ignoreZoom = false) const;

	/**
	 * Draw rectangle to window ImDrawlist
	 * This method accepts DIWNE coordinates and converts them to truncated screen coordinates.
	 * \see AddRectFilledDiwne
	 */
	void AddRectDiwne(const ImVec2& p_min, const ImVec2& p_max, const ImVec4& col, float rounding = 0.0f,
	                  ImDrawFlags rounding_corners = ImDrawFlags_RoundCornersAll, float thickness = 1.0f,
	                  bool ignoreZoom = false) const;
	void AddRectForegroundDiwne(const ImVec2& p_min, const ImVec2& p_max, const ImVec4& col, float rounding = 0.0f,
	                            ImDrawFlags rounding_corners = ImDrawFlags_RoundCornersAll, float thickness = 1.0f,
	                            bool ignoreZoom = false) const;

	// /// Draws a rounded filled rect offset inwards
	// void AddRectFilledOffsetDiwne(const ImVec2& p_min, const ImVec2& p_max, const ImVec4& col, float rounding,
	//                               ImDrawFlags rounding_corners, bool ignoreZoom, float offset) const;

	/** \brief Draw Bezier (not Bezier really) curve to window ImDrawList
	 *
	 * \param p1 const ImVec2& start point in diwne coords
	 * \param p2 const ImVec2& start control point in diwne coords
	 * \param p3 const ImVec2& end control point in diwne coords
	 * \param p4 const ImVec2& end point in diwne coords
	 * \param col ImVec4
	 * \param thickness float
	 * \param 0 int num_segments with zero ImGui use some optimal? value
	 * \return void
	 *
	 */
	void AddBezierCurveDiwne(ImDrawList* idl, const ImVec2& p1, const ImVec2& p2, const ImVec2& p3, const ImVec2& p4,
	                         ImVec4 col, float thickness, int num_segments = 0) const;

	// =============================================================================================================

	void drawGrid(bool dots, float size, ImVec4 color, float fadeStart, float fadeEnd, bool ignoreZoom) const;

	// Icon drawing
	// =============================================================================================================
	/**
	 * Add ImGui Button with icon on it \see DrawIcon()
	 * \return true if interaction with button happen, false otherwise
	 */
	bool IconButton(IconType bgIconType, ImColor bgShapeColor, ImColor bgInnerColor, ImVec2 size, ImVec4 padding,
	                bool filled, std::string id) const;

	/**
	 * Add ImGui Button with icon on it \see DrawIcon()
	 * @param disabled Disables the button, prevents presses and retaining of active id on drag, but still allows
	 * querying of hover state when wrapped in a ImGui group or with appropriate IsItemHovered flags.
	 * \return true if interaction with button happen, false otherwise
	 */
	bool IconButton(std::string id, bool disabled, IconType bgIconType, ImColor bgShapeColor, ImColor bgInnerColor,
	                IconType fgIconType, ImColor fgShapeColor, ImColor fgInnerColor, ImVec2 size, ImVec4 padding,
	                bool filled, ImVec2 thickness = ImVec2(1, 1), float rounding = 0) const;

	/**
	 * // TODO: (DR) Docs, new IconButton that allows various styles depening on the button state (hover, maybe press)
	 * Add ImGui Button with icon on it \see DrawIcon()
	 * @param disabled Disables the button, prevents presses and retaining of active id on drag, but still allows
	 * querying of hover state when wrapped in a ImGui group or with appropriate IsItemHovered flags.
	 * \return true if interaction with button happen, false otherwise
	 */
	bool IconButton2(const std::string& id, ImVec2 size, bool disabled, IconType bgIconType, IconType fgIconType,
	                 const IconStyle& style, const IconStyle& hoveredStyle, const IconStyle& activeStyle) const;

	/**
	 * \brief Just the shape of the button, no interaction
	 * \param size
	 * \param color
	 * \param rounding
	 */
	void EmptyButton(ImVec2 size, ImColor color, float rounding);

	// padding - top, right, bottom, left

	// TODO: (DR) This method needs a serious review (and all the icon drawing methods it calls)
	//  Some initial issues:
	//   - Arguments are passed by value for no reason
	//   - way to many arguments, better to group them into some sort of IconStyle struct
	//   - But how to avoid the need to constantly construct new structs, they should be mostly static somewhere

	/**
	 * \brief Draw an Icon combined from two parts (foreground and background)
	 * to the window \a ImDrawList filled with a \a ShapeColor.
	 * When \a filled == true, both shapes have a border.
	 * Then, the border color is the ShapeColor and the shape is filled with the
	 * \a InnerColor
	 *
	 * The icon is drawn at the current ImGui cursor. This method does NOT advance the cursor.
	 *
	 * \param bgIconType background shape (typically a Rectangle)
	 * \param bgShapeColor color of the background shape (or a border color if not filled)
	 * \param bgInnerColor color of the background shape interior if filled == false
	 * \param fgIconType foreground shape (typically a Triangle or Cross)
	 * \param fgShapeColor color of the foreground shape if filled == false
	 * \param fgInnerColor color of foreground shape interior if filled == false
	 * \param size of the icon in screen coordinates
	 * \param padding of the fg shape (left, bottom, right, top)
	 * \param filled means
	 *  - true fill both shapes with their ShapeColor
	 *  - false draw both shapes with a ShapeColor border and fill them with InnerColor
	 * \return void
	 */
	void DrawIcon(IconType bgIconType, ImColor bgShapeColor, ImColor bgInnerColor, IconType fgIconType,
	              ImColor fgShapeColor, ImColor fgInnerColor, ImVec2 size, ImVec4 padding, bool filled,
	              ImVec2 thickness = ImVec2(1, 1), float rounding = 0) const;

	// TODO: I don't like the whole "filled" argument, yes it is technically correct, we fill the insides to create
	//  a border with the shape color, BUT, thats just a trick to make a BORDER, "filled" should mean absence of border
	//  meaning the whole icon has the innerColor, not the shapeColor
	//  I feel like that makes more sense to the user of the method, hence there should be a "color" and "borderColor"
	//  Not "shape color" and "inner color", not bool filled, but bool border.

	/**
	 * \brief Draw a circle icon
	 * \param idl ImDrawList* where to draw
	 * \param shapeColor color of the whole shape (border line if not filled)
	 * \param innerColor color of the inner part (middle) of shape
	 * \param topLeft position of the icon in screen coords
	 * \param bottomRight of the icon in screen coords
	 * \param filled false means use the InnerColor in the foreground Shape
	 * \param thickness of the border for zoomFactor = 1.0
	 */
	void DrawIconCircle(ImDrawList* idl, ImColor shapeColor, ImColor innerColor, ImVec2 topLeft, ImVec2 bottomRight,
	                    bool filled, float thickness = 1) const;

	void DrawIconRectangle(ImDrawList* idl, ImColor shapeColor, ImColor innerColor, ImVec2 topLeft, ImVec2 bottomRight,
	                       bool filled, ImVec2 thickness = ImVec2(1, 1), float rounding = 0) const;

	void DrawIconPause(ImDrawList* idl, ImColor shapeColor, ImColor innerColor, ImVec2 topLeft, ImVec2 bottomRight,
	                   bool filled, ImVec2 thickness = ImVec2(1, 1), float rounding = 0) const;

	void DrawIconTriangleLeft(ImDrawList* idl, ImColor shapeColor, ImColor innerColor, ImVec2 topLeft,
	                          ImVec2 bottomRight, bool filled, float thickness = 1) const;
	void DrawIconSkipBack(ImDrawList* idl, ImColor shapeColor, ImColor innerColor, ImVec2 topLeft, ImVec2 bottomRight,
	                      bool filled, float thickness = 1) const;
	void DrawIconSkipBack2(ImDrawList* idl, ImColor shapeColor, ImColor innerColor, ImVec2 topLeft, ImVec2 bottomRight,
	                       bool filled, float thickness = 1) const;
	void DrawIconRewind(ImDrawList* idl, ImColor shapeColor, ImColor innerColor, ImVec2 topLeft, ImVec2 bottomRight,
	                    bool filled, float thickness = 1) const;

	void DrawIconTriangleRight(ImDrawList* idl, ImColor shapeColor, ImColor innerColor, ImVec2 topLeft,
	                           ImVec2 bottomRight, bool filled, float thickness = 1) const;

	void DrawIconSkipForward(ImDrawList* idl, ImColor shapeColor, ImColor innerColor, ImVec2 topLeft,
	                         ImVec2 bottomRight, bool filled, float thickness = 1) const;
	void DrawIconSkipForward2(ImDrawList* idl, ImColor shapeColor, ImColor innerColor, ImVec2 topLeft,
	                          ImVec2 bottomRight, bool filled, float thickness = 1) const;
	void DrawIconFastForward(ImDrawList* idl, ImColor shapeColor, ImColor innerColor, ImVec2 topLeft,
	                         ImVec2 bottomRight, bool filled, float thickness = 1) const;

	void DrawIconCross(ImDrawList* idl, ImColor shapeColor, ImColor innerColor, ImVec2 topLeft, ImVec2 bottomRight,
	                   bool filled, float shapeThickness = 2, float innerThickness = 1) const;

	void DrawIconHyphen(ImDrawList* idl, ImColor shapeColor, ImColor innerColor, ImVec2 topLeft, ImVec2 bottomRight,
	                    bool filled, float thickness = 3.0f) const;

	void DrawIconTriangleDownLeft(ImDrawList* idl, ImColor shapeColor, ImColor innerColor, ImVec2 topLeft,
	                              ImVec2 bottomRight, bool filled, float thickness = 1) const;

	void DrawIconTriangleDownRight(ImDrawList* idl, ImColor shapeColor, ImColor innerColor, ImVec2 topLeft,
	                               ImVec2 bottomRight, bool filled, float thickness = 1) const;

	void DrawIconGrabDownLeft(ImDrawList* idl, ImColor shapeColor, ImColor innerColor, ImVec2 topLeft,
	                          ImVec2 bottomRight, bool filled, float thickness = 1) const;

	void DrawIconGrabDownRight(ImDrawList* idl, ImColor shapeColor, ImColor innerColor, ImVec2 topLeft,
	                           ImVec2 bottomRight, bool filled, float thickness = 1) const;

	void DrawBurgerMenu(ImDrawList* idl, const ImColor& color, const ImRect& rect, const ImVec2& padding,
	                    float thickness) const;
};

struct IconStyle
{
	ImColor bgShapeColor;
	ImColor bgInnerColor;
	ImColor fgShapeColor;
	ImColor fgInnerColor;
	ImVec4 padding;
	bool filled{false};
	ImVec2 thickness{1, 1};
	float rounding = 0;

	IconStyle(const ImColor& bgShapeColor, const ImColor& bgInnerColor, const ImColor& fgShapeColor,
	          const ImColor& fgInnerColor)
	    : bgShapeColor(bgShapeColor), bgInnerColor(bgInnerColor), fgShapeColor(fgShapeColor), fgInnerColor(fgInnerColor)
	{}

	IconStyle(const ImColor& bgShapeColor, const ImColor& bgInnerColor, const ImColor& fgShapeColor,
	          const ImColor& fgInnerColor, const ImVec4& padding, bool filled, const ImVec2& thickness, float rounding)
	    : bgShapeColor(bgShapeColor), bgInnerColor(bgInnerColor), fgShapeColor(fgShapeColor),
	      fgInnerColor(fgInnerColor), padding(padding), filled(filled), thickness(thickness), rounding(rounding)
	{}
};

/**
 * \brief Icon shape types
 */
enum IconType : unsigned int
{
	NoIcon,
	Circle,
	Rectangle,
	TriangleLeft,
	TriangleRight,
	TriangleDownLeft,
	TriangleDownRight,
	GrabDownLeft,
	GrabDownRight,
	Cross,
	Hyphen,
	Stop,         ///< Black Square For Stop (U+23F9)
	Pause,        ///< Double Vertical Bar (U+23F8)
	SkipBack,     ///< |< vertical bar followed by the left arrow
	SkipBack2,    ///< "<|" left arrow followed by the vertical bar
	SkipForward,  ///< ">|" right arrow followed by the vertical bar
	SkipForward2, ///< |> vertical bar followed by the right arrow
	Rewind,       ///< Black Left-Pointing Double Triangle (U+23EA)
	FastForward,  ///< Black Right-Pointing Double Triangle (U+23E9)
	AtFrom,       // todo, now a synonym to the FastForward
	AtTo,         // todo, now a synonym to the Rewind
};

} // namespace DIWNE
