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

	DisplayOptions() = default;

	bool shouldDraw(const Entity& entity) const;
};
} // namespace Vp
