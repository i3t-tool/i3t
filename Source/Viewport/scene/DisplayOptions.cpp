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
		return showGrid;
	case DisplayType::Camera:
		return showCamera;
	case DisplayType::Frustum:
		return showFrustum;
	default:
		return true;
	}
}
