#pragma once

#include <memory>
#include <string>

#define IMGUI_DEFINE_MATH_OPERATORS
#include "imgui.h"

namespace DIWNE
{
class NodeEditor;

/**
 * \brief pin icon shape type
 * used in WorkspaceCorePin::content(), file WorkspaceElementWitCoreData.cpp::379
 * drawn by Diwne::DrawIcon, method Diwne::DrawIconXXXX, file Diwne.cpp
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

class DrawHelper
{
public:
	NodeEditor* e;

	DrawHelper(NodeEditor* editor) : e(editor){};

	// TODO: Maybe move the drawing methods into a class of some sort
	//  IconDrawing is related to this as well, could be put under some kind of a Drawer/Renderer helper class

	void AddLine(const ImVec2& p1, const ImVec2& p2, ImVec4 col, float thickness = 1.0f, bool ignoreZoom = false) const;

	/**
	 * Draw filled rectangle to window ImDrawlist
	 * @param p_min bottom left corner in diwne coords
	 * @param p_max top right corner in diwne coords
	 * @param col
	 * @param rounding
	 * @param rounding_corners
	 */
	void AddRectFilledDiwne(const ImVec2& p_min, const ImVec2& p_max, ImVec4 col, float rounding = 0.0f,
	                        ImDrawFlags rounding_corners = ImDrawFlags_RoundCornersAll, bool ignoreZoom = false) const;

	/**
	 * Draw rectangle to window ImDrawlist
	 * \see AddRectFilledDiwne
	 */
	void AddRectDiwne(const ImVec2& p_min, const ImVec2& p_max, ImVec4 col, float rounding = 0.0f,
	                  ImDrawFlags rounding_corners = ImDrawFlags_RoundCornersAll, float thickness = 1.0f,
	                  bool ignoreZoom = false) const;

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
	void AddBezierCurveDiwne(const ImVec2& p1, const ImVec2& p2, const ImVec2& p3, const ImVec2& p4, ImVec4 col,
	                         float thickness, int num_segments = 0) const;

	/**
	 * Add ImGui Button with icon on it \see DrawIcon()
	 * \return true if interaction with button happen, false otherwise
	 */
	bool IconButton(DIWNE::IconType bgIconType, ImColor bgShapeColor, ImColor bgInnerColor, ImVec2 size, ImVec4 padding,
	                bool filled, std::string const id) const;
	/**
	 * \brief Just the shape of the button, no interaction
	 * \param size
	 * \param color
	 * \param rounding
	 */
	void EmptyButton(ImVec2 size, ImColor color, float rounding);

	/**
	 * Add ImGui Button with icon on it \see DrawIcon()
	 * \return true if interaction with button happen, false otherwise
	 */
	bool IconButton(DIWNE::IconType bgIconType, ImColor bgShapeColor, ImColor bgInnerColor, DIWNE::IconType fgIconType,
	                ImColor fgShapeColor, ImColor fgInnerColor, ImVec2 size, ImVec4 padding, bool filled,
	                std::string const id) const;

	// padding - top, right, bottom, left

	/**
	 * \brief Draw an Icon combined from two parts (foreground and background)
	 * to the window \a ImDrawList filled with a \a ShapeColor.
	 * When \a filled == true, both shapes have a border.
	 * Then, the border color is the ShapeColor and the shape is filled with the
	 * \a InnerColor
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
	void DrawIcon(DIWNE::IconType bgIconType, ImColor bgShapeColor, ImColor bgInnerColor, DIWNE::IconType fgIconType,
	              ImColor fgShapeColor, ImColor fgInnerColor, ImVec2 size, ImVec4 padding, bool filled,
	              ImVec2 thickness = ImVec2(1, 1), float rounding = 0) const;

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

protected:
	float zoom() const;
};

} // namespace DIWNE
