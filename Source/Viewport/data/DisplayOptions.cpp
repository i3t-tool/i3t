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
#include "DisplayOptions.h"

#include "Viewport/entity/Entity.h"

using namespace Vp;

bool DisplayOptions::shouldDraw(const Entity& entity) const
{
	switch (entity.getDisplayType())
	{
	case DisplayType::Default:
		return showDefault;
	case DisplayType::Axes:
		return showAxes;
	case DisplayType::Grid:
		return showGrids;
	case DisplayType::Camera:
		return showCamera;
	case DisplayType::Frustum:
		return showFrustum;
	case DisplayType::Tracking:
		return showTracking;
	default:
		return true;
	}
}
