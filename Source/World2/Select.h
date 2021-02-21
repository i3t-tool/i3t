#pragma once

#include "Component.h"
#include "GameObject.h"
#include "pgr.h"

class Select
{
public:
  static Component* stencilRef[256];
  static int registerStencil(Component* owner);
  static bool freeStencil(Component* owner);
  static unsigned char getStencilAt(int x, int y, int r, int filter);
};