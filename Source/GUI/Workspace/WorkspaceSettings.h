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

struct WorkspaceSettings
{
	float tracking_smoothScrollSpeed{0.2f};
	float tracking_smoothScrollModifier{1.0f}; // Not serialized on purpose
	float tracking_jaggedScrollSpeed{0.2f};

	bool showQuickAddMenu{false};      // Option in preferences
	bool quickAddMenuCollapsed{false}; // Persistant flag, no need for UI
};