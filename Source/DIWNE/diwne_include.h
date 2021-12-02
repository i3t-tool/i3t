#ifndef DIWNE_INCLUDE_H
#define DIWNE_INCLUDE_H

#define DIWNE_DEBUG

#include <limits>

#include "spdlog/fmt/fmt.h"

//------------------------------------------------------------------------------
# include <imgui.h>
# define IMGUI_DEFINE_MATH_OPERATORS
# include <imgui_internal.h>

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
