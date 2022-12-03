#pragma once

namespace Vp
{
class Entity;

class DisplayOptions
{
public:
	bool showDefault{true};
	bool showAxes{true};
	bool showGrid{true};
	bool showCamera{true};
	bool showFrustum{true};

	DisplayOptions() = default;
	DisplayOptions(bool showDefault, bool showAxes, bool showGrid, bool showCamera, bool showFrustum);

	bool shouldDraw(const Entity& entity) const;
};
} // namespace Vp
