#pragma once

#include "DIWNE/Core/diwne_imgui.h"

namespace DIWNE
{
/**
 * Interface for DIWNE style variables.
 * Default style is defined in the StyleBase subclass, that is held by NodeEditor instances.
 * Other DiwneObjects then hold a reference to a StyleOverride object that can override variables in the base style.
 * The style variables can be accessed from every DiwneObject using its style() method,
 * which returns this general interface, implemented by either StyleBase or StyleOverride.
 */
class Style
{
public:
	/**
	 * Style variables keys enum, they can be accessed by the get(key) method.
	 * The key is one of the enum values.
	 * Real style values are kept in a std::vector registry that is accessed by the key index
	 */
	enum Var : short
	{
		GRID_COLOR = 0,  // ImVec4
		GRID_DOTS_COLOR, // ImVec4

		NODE_BG,        // ImVec4
		NODE_HEADER_BG, // ImVec4

		NODE_ROUNDING, // int
		NODE_PADDING,  // float // TODO: Review in example app

		MOUSE_DRAG_THRESHOLD, // float // TODO: Maybe shouldnt be style but setting? what about dpi?

		SELECTION_ROUNDING,    // float
		SELECTED_BORDER_WIDTH, // float
		SELECTED_BORDER_COLOR, // ImVec4
		HOVER_BORDER_WIDTH,    // float
		HOVER_BORDER_COLOR,    // ImVec4

		LINK_COLOR,           // ImVec4
		LINK_COLOR_SELECTED,  // ImVec4
		LINK_UNPLUGGED_ALPHA, // float, replacement link color alpha value set when the link is connected on both sides
		LINK_WIDTH,           // float
		LINK_SELECTED_WIDTH,  // float
		LINK_BORDER_WIDTH,    // float
		LINK_BORDER_COLOR,    // ImVec4

		PIN_SPACING,           // ImVec2, spacing around the pin
		PIN_LABEL_SPACING,     // float, gap between the pin icon and the label
		PIN_ENABLE_DRAG_LABEL, // bool, whether to include pin label in the "draggable" area
		PIN_ENABLE_HOVER_BG,   // bool, whether to draw pin background on hover
		PIN_BG_SPACING,        // ImVec2, extra visual spacing added to pin background, might overlap other elements!
		PIN_BG_COLOR,          // ImVec4
		PIN_BG_ROUNDING,       // float
		PIN_HOVER_COLOR_SHIFT, // ImVec4, added to the pin color on hover

		PIN_SOCKET_OFFSET,          // float, offset between the pin socket and the edge of a node
		PIN_SOCKET_BORDER_WIDTH,    // float, thickness of the socket pin border
		PIN_SOCKET_BORDER_COLOR,    // ImVec4, color of the socket pin border
		PIN_SOCKET_BORDER_ROUNDING, // float
		PIN_SOCKET_BG,              // ImVec4
		PIN_SOCKET_THICKNESS,       // float
		PIN_SOCKET_CONNECTED_GAP,   // float, gap between the outer and inner circle of a connected socket
		PIN_SOCKET_ROUNDING,        // float, rounding for square socket style
		PIN_SOCKET_INNER_ROUNDING,  // float

		PIN_SQUARE_OFFSET,       // float, offset between the pin icon and the edge of a node
		PIN_SQUARE_BORDER_WIDTH, // float, thickness of the square pin border
		PIN_SQUARE_BORDER_COLOR, // ImVec4, color of the square pin border
		PIN_SQUARE_ROUNDING,     // float

		PIN_CIRCLE_OFFSET,       // float
		PIN_CIRCLE_BORDER_WIDTH, // float
		PIN_CIRCLE_BORDER_COLOR, // ImVec4

		PIN_DIM_ENABLED, // bool, whether to dim incompatible pins when dragging a link
		PIN_DIM_ALPHA,   // float, pin dim alpha

		SELECTION_RECT_FULL_COLOR,  // ImVec4
		SELECTION_RECT_TOUCH_COLOR, // ImVec4
		SELECTION_RECT_ALPHA,       // float

		DROP_ZONE_BG,         // ImVec4
		DROP_INDICATOR_COLOR, // ImVec4
		DROP_ZONE_DROP_GAP,   // ImVec2
		DROP_ZONE_MARGIN,     // ImVec2

		STYLE_VAR_COUNT // Total number of style variables, used internally
	};

	virtual const ImVec4& color(Style::Var key) const = 0;
	virtual ImVec2 size(Style::Var key) const = 0;
	virtual float decimal(Style::Var key) const = 0;
	virtual int integer(Style::Var key) const = 0;
	virtual bool boolean(Style::Var key) const = 0;

	virtual ~Style() = default;
};
} // namespace DIWNE