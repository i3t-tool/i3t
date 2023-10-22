/**
 * \file
 * \brief
 * \author Dan Rakušan <rakusan.dan@gmail.com>
 * \copyright Copyright (C) 2016-2023 I3T team, Department of Computer Graphics
 * and Interaction, FEE, Czech Technical University in Prague, Czech Republic
 *
 * This file is part of I3T - An Interactive Tool for Teaching Transformations
 * http://www.i3t-tool.org
 *
 * GNU General Public License v3.0 (see LICENSE.txt or https://www.gnu.org/licenses/gpl-3.0.txt)
 */
#pragma once

namespace Vp
{
class Entity;

/**
 * Various viewport display settings.
 */
class DisplayOptions
{
public:
	// Entity visibility options
	bool showDefault{true};
	bool showAxes{true};
	bool showGrid{true};
	bool showCamera{true};
	bool showFrustum{true};

	// Grid options
	bool showGridLines{true};
	bool showGridXAxis{true};
	bool showGridYAxis{false};
	bool showGridZAxis{true};

	DisplayOptions() = default;

	bool shouldDraw(const Entity& entity) const;
};
} // namespace Vp
