#pragma once

#include "glm/glm.hpp"

#include "Viewport/camera/AggregateCamera.h"
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
