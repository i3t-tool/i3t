#pragma once
//----------------
/**
 * \file Select.h
 * \author Daniel Gruncl
 * \brief Registration of stencil values.
 * This class keeps global record of Components and stencil values registered by them,
 * which allows to use globally unique stencil values and find Component instance binded to int.
 * This is used for selecting objects in 3D scene.
 */
//---------------
#include "Component.h"
#include "GameObject.h"
#include "pgr.h"

class Select
{
public:
  static bool stencilRef[256];
  static int registerStencil();
  static bool freeStencil(char stencil);
  static unsigned char getStencilAt(int x, int y, int r, int filter);
};