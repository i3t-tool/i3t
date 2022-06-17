#pragma once
//----------------
/**
 * \file Select.h
 * \author Daniel Gruncl
 * \brief Registration of m_stencil values.
 * This class keeps global record of Components and m_stencil values registered by them,
 * which allows to use globally unique m_stencil values and find Component instance binded to int.
 * This is used for selecting objects in 3D scene.
 */
//---------------
#include "Component.h"
#include "GameObject.h"
#include "pgr.h"

class Select
{
public:
	static bool stencilRef
	    [256]; ///<table of registred stencil values. true - stencil value is already used by another object, false - free to use.
	/**
  * \fn static int registerStencil();
  * Finds unused stencil value, sets it as registred and returns it.
  * \return stencil value, or -1, if no stencil value is available
  */
	static int  registerStencil();
	static bool freeStencil(char stencil);
	/**
  * \fn static unsigned char getStencilAt(int x, int y, int r, int filter);
  * reads quad with center at x,y and radius of r from stencil buffer. If value not equal to filter and not equal zero is found within the quad,
  * the value is returned. Otherwise, 0 is returned.
  * This is for selecting objects by stencil. Return is selected object, filter is ignored object - only one stencil value can be ignored.
  * Stencil value 0 is interpreted as nothing selectable.
  */
	static unsigned char getStencilAt(int x, int y, int r, int filter);
};