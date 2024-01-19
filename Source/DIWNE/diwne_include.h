#ifndef DIWNE_INCLUDE_H
#define DIWNE_INCLUDE_H

/**
 * \file
 * \brief
 * \author Jaroslav Holeček <holecek.jaroslav@email.cz>
 * \date 20.3.2022
 * \copyright Copyright (C) 2016-2023 I3T team, Department of Computer Graphics
 * and Interaction, FEE, Czech Technical University in Prague, Czech Republic
 *
 * This file is part of I3T - An Interactive Tool for Teaching Transformations
 * http://www.i3t-tool.org
 *
 * GNU General Public License v3.0 (see LICENSE.txt or https://www.gnu.org/licenses/gpl-3.0.txt)
 *
 * Including all libraries and declaration of objects in right order.
 *
 * All functions return true if some interaction happens inside (in a
 * cascade way from inner elements).
 * All objects have access to one DIWNE::Diwne (and inherited) object
 * so they can share information if needed.
 * Overwrite whatever function you want - do not forget to call the overwritten
 * function (of the parent class) in your function for maintain all functionality.
 * Base concept is call-super https://en.wikipedia.org/wiki/Call_super, where
 * you fill in function, that is called in the middle of the whole code.
 *
 * You probably do not need to overwrite any function with "Diwne" in its name
 * - they are intended as internal - be very careful when you do it.
 *
 * DIWNE offers you only wrappers of ImGui functions - so storing your object
 * and drawing them in some order is up to you.
 * \see TODOexample for inspiration
 *
 * See DiwneObject::drawDiwne() to see what functions are called when
 * processing object
 *
 * Node editor is an infinite surface where you can position (and move) your
 * nodes. When diwne (position, coords etc.) is used anywhere, this surface is
 * meant. There are "two" workareas coords (in DINWE::Diwne)
 * 1) is position and size of node editor window on screen
 * 2) is position of node editor surface in window
 */

#include "spdlog/fmt/fmt.h"
#include <limits>
//------------------------------------------------------------------------------
#include <imgui.h>
#define IMGUI_DEFINE_MATH_OPERATORS
#include <imgui_internal.h>
// TODO - REDEFINE DIWNE
/*
#define DIWNE_DEBUG(node_editor, debugCode)                                                                            \
    do                                                                                                                 \
    {                                                                                                                  \
        if (node_editor.m_diwneDebug_on)                                                                               \
        {                                                                                                              \
            debugCode                                                                                                  \
        }                                                                                                              \
    } while (0) // do-while only for code-technical reason
*/
namespace DIWNE
{
typedef unsigned int ID;

/*! \brief Drawing mode of DiwneObject  */
enum DrawMode
{
	JustDraw,   /*!< Draw object only - block all (node editor's) interactions */
	Interacting /*!< Usual mode - draw and allow interaction too */
};

enum IconType
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
	Stop,
	Pause,
	SkipBack,
	SkipBack2,
	SkipForward,
	SkipForward2,
	Rewind,
	FastForward,
	AtFrom,
	AtTo,
};

enum DiwneAction
{
	None,
	FocusOnObject,
	InteractingContent, /* for other unspecified interactions */
	NewLink,

	HoldNode,
	HoldPin,
	HoldLink,
	HoldWorkarea,

	DragNode,
	DragPin,
	DragLink, /* dragging already existing/connected link */
	DragWorkarea,

	TouchNode,
	TouchPin,
	TouchLink,
	TouchWorkarea,

	SelectionRectFull,
	SelectionRectTouch
};

class DiwneObject;

class Node;
class Pin;
class Link;

class Diwne;

} // namespace DIWNE

#include "DiwneObject.h"

#include "Link.h"
#include "diwne.h"

#include "Node.h"
#include "Pin.h"

#endif // DIWNE_INCLUDE_H
