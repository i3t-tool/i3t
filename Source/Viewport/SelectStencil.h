/**
 * \file Select.h
 * \author Daniel Gruncl
 * \brief Registration of stencil selection values.
 */

#pragma once

#include "pgr.h"

namespace Vp
{
class SelectStencil
{
private:
	bool debug;

public:
	bool stencilRef[256]; ///< table of registred stencil values. true -
	                      ///< stencil value is already used by another
	                      ///< object, false - free to use.
	SelectStencil();

	/**
	 * \fn static int registerStencil();
	 * Finds unused stencil value, sets it as registred and returns it.
	 * \return stencil value, or -1, if no stencil value is available
	 */
	int registerStencil();

	bool freeStencil(int stencil);

	/**
	 * \fn static unsigned char getStencilAt(int x, int y, int r, int filter);
	 * reads quad with center at x,y and radius of r from stencil buffer. If value
	 * not equal to filter and not equal zero is found within the quad, the value
	 * is returned. Otherwise, 0 is returned. This is for selecting objects by
	 * stencil. Return is selected object, filter is ignored object - only one
	 * stencil value can be ignored. Stencil value 0 is interpreted as nothing
	 * selectable.
	 */
	unsigned char getStencilAt(int x, int y, int r, int filter);
};
} // namespace Vp