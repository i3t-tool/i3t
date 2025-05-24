/**
 * \file
 * \brief
 * \author Dan Rakušan <rakusan.dan@gmail.com>
 * \copyright Copyright (C) 2016-2025 I3T team, Department of Computer Graphics
 * and Interaction, FEE, Czech Technical University in Prague, Czech Republic
 *
 * This file is part of I3T - An Interactive Tool for Teaching Transformations
 * http://www.i3t-tool.org
 *
 * GNU General Public License v3.0 (see LICENSE.txt or https://www.gnu.org/licenses/gpl-3.0.txt)
 */
#include "ViewportModuleSettings.h"

#include "rttr/registration.h"

RTTR_REGISTRATION
{
	rttr::registration::class_<ViewportModuleSettings>("ViewportModuleSettings")
	    .property("lightFollowsCamera", &ViewportModuleSettings::lightFollowsCamera);
}