#include "DisplayOptions.h"

#include "Viewport/entity/Entity.h"

using namespace Vp;

DisplayOptions::DisplayOptions(bool showDefault, bool showAxes, bool showGrid, bool showCamera, bool showFrustum)
    : showDefault(showDefault), showAxes(showAxes), showGrid(showGrid), showCamera(showCamera), showFrustum(showFrustum)
{
	// Empty
}

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
	}
}
