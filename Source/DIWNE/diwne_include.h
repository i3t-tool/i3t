#ifndef DIWNE_INCLUDE_H
#define DIWNE_INCLUDE_H

#include "spdlog/fmt/fmt.h"
#include <limits>

//------------------------------------------------------------------------------
# include <../Dependencies/imgui_node_editor/external/imgui/imgui.h>
# define IMGUI_DEFINE_MATH_OPERATORS
# include <../Dependencies/imgui_node_editor/external/imgui/imgui_internal.h>
//# include "../Dependencies/imgui_node_editor/external/imgui/imgui_extra_math.h"
//# include "../Dependencies/imgui_node_editor/external/imgui/imgui_bezier_math.h"
//# include "../Dependencies/imgui_node_editor/external/imgui/imgui_canvas.h"

//# include <vector>
//# include <string>

namespace DIWNE
{

typedef unsigned int ID;
class Node;
class Pin;
class Link;

class Diwne;

}


#include "Link.h"
#include "Pin.h"
#include "Node.h"

#include "diwne.h"



#endif // DIWNE_INCLUDE_H
