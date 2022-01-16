#ifndef DIWNE_INCLUDE_H
#define DIWNE_INCLUDE_H

#include <limits>
#include "spdlog/fmt/fmt.h"
//------------------------------------------------------------------------------
# include <imgui.h>
# define IMGUI_DEFINE_MATH_OPERATORS
# include <imgui_internal.h>



#define DIWNE_DEBUG

namespace DIWNE
{

typedef unsigned int ID;

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
