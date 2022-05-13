#ifndef DIWNE_INCLUDE_H
#define DIWNE_INCLUDE_H

/*!
 * \file DIWNE/diwne_include.h
 * \author Jaroslav Holeček, holecek.jaroslav@email.cz
 * \date 20.3.2022
 *
 * Including all libraries and declaration of objects in right order.
 *
 * All functions return true if some interaction happen inside of them (in cascade way from inner elements)
 * All objects have access to one DIWNE::Diwne (and inhereted) object so they can share information if needed
 * Overwrite whatever function you want - be aware of calling (so simply call) overwrited function (of parent class) in your function for maintain all funkcionality
 * Base concept is call-super https://en.wikipedia.org/wiki/Call_super , where you fill in function, that is called in middle of whole code
 * You probably need not overwrite function with "Diwne" in name - they are intended as internal - be very careful when you do it
 * DIWNE offer you only wrappers of ImGui functions - so storing your object and drawing them in some order is up to you \see TODOexample for inspiration
 *
 * See DiwneObject::drawDiwne() for inspect what functions are called when processing object
 *
 * Node editor is infinite surface where you can positioning (and moving) your nodes. When somewhere is used diwne (position, coords etc.) this surface is meant
 * There are "two" workareas coords (in DINWE::Diwne)
 *  1) is position and size of node editor window on screen
 *  2) is position of node editor surface in window
 *
 */

#include <limits>
#include "spdlog/fmt/fmt.h"
//------------------------------------------------------------------------------
# include <imgui.h>
# define IMGUI_DEFINE_MATH_OPERATORS
# include <imgui_internal.h>

#define DIWNE_DEBUG(node_editor, debugCode) do{ if(node_editor.m_diwneDebug_on){ debugCode } }while(0) /* do-while only for code-technical reason */

namespace DIWNE
{

typedef unsigned int ID;

/*! \brief Drawing mode of DiwneObject  */
enum DrawMode
{
    JustDraw, /*!< Draw object only - block all (node editor's) interactions */
    Interacting /*!< Usual mode - draw and allow interaction too */
};

enum IconType
{
    Circle,
    Rectangle,
    TriangleLeft,
    TriangleRight,
    TriangleDownLeft,
    TriangleDownRight,
    Cross
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

}

#include "DiwneObject.h"


#include "Link.h"
#include "diwne.h"

#include "Pin.h"
#include "Node.h"

#endif // DIWNE_INCLUDE_H
