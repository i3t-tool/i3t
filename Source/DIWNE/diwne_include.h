#ifndef DIWNE_INCLUDE_H
#define DIWNE_INCLUDE_H

#include <limits>
#include "spdlog/fmt/fmt.h"
//------------------------------------------------------------------------------
# include <imgui.h>
# define IMGUI_DEFINE_MATH_OPERATORS
# include <imgui_internal.h>



#define DIWNE_DEBUG(debugCode) do{ if(DIWNE::s_diwneDebug_on){ debugCode } }while(0) /* do-while only for code-technical reason */


namespace DIWNE
{

#ifdef DIWNE_DEBUG
    static bool s_diwneDebug_on = true;
#endif // DIWNE_DEBUG

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

    HoldNode,
    DragNode,
    InteractingContent, /* for other unspecified interactions */

    FocusOnObject,

    HoldPin,
    DragPin,

    NewLink,
    HoldLink,
    DragLink, /* dragging already existing/connected link */

    TouchNode,

    HoldWorkarea,
    DragWorkarea,

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
