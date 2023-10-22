/**
 * \file
 * \brief Registration of stencil selection values.
 * \author Daniel Gruncl
 * \copyright Copyright (C) 2016-2023 I3T team, Department of Computer Graphics
 * and Interaction, FEE, Czech Technical University in Prague, Czech Republic
 *
 * This file is part of I3T - An Interactive Tool for Teaching Transformations
 * http://www.i3t-tool.org
 *
 * GNU General Public License v3.0 (see LICENSE.txt or https://www.gnu.org/licenses/gpl-3.0.txt)
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