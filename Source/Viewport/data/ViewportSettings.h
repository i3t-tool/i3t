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

#include "ViewportGlobalSettings.h"
#include "ViewportSceneSettings.h"

namespace Vp
{
/**
 * A collection of viewport settings.
 * Contains the following settings groups:
 * - scene() = Settings specific to the currently loaded scene
 * - global() = Global viewport settings
 */
struct ViewportSettings
{
private:
	ViewportSceneSettings m_sceneSettings;
	ViewportGlobalSettings m_globalSettings;

public:
	ViewportSettings();

	ViewportSceneSettings& scene()
	{
		return m_sceneSettings;
	}
	ViewportGlobalSettings& global()
	{
		return m_globalSettings;
	}
};

} // namespace Vp
