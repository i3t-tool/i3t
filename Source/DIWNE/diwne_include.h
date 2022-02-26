#ifndef DIWNE_INCLUDE_H
#define DIWNE_INCLUDE_H

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

enum IconType
{
    Circle,
    Rectangle,
    TriangleLeft,
    TriangleRight,
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
