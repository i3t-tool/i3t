/**
 * \file
 * \brief
 * \author Martin Herich <martin.herich@phire.cz>
 * \copyright Copyright (C) 2016-2023 I3T team, Department of Computer Graphics
 * and Interaction, FEE, Czech Technical University in Prague, Czech Republic
 *
 * This file is part of I3T - An Interactive Tool for Teaching Transformations
 * http://www.i3t-tool.org
 *
 * GNU General Public License v3.0 (see LICENSE.txt or https://www.gnu.org/licenses/gpl-3.0.txt)
 */
#pragma once

#include <cstdlib>
#include <time.h>

namespace Detail
{
struct RandInitializer
{
	RandInitializer() noexcept
	{
		srand(time(0));
	}
};

static RandInitializer randInitializer;
} // namespace Detail

long randLong()
{
	if (sizeof(int) < sizeof(long))
		return (static_cast<long>(rand()) << (sizeof(int) * 8)) | rand();

	return rand();
}
