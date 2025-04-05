#pragma once

#include <limits>

#include "DiwnePanel.h"

namespace DIWNE
{
class Gravity
{
public:
	static inline void alignRightEdge(std::initializer_list<std::reference_wrapper<DiwnePanel>> panels)
	{
		float maxX = std::numeric_limits<float>::lowest();
		for (auto& panel : panels)
		{
			maxX = std::max(maxX, panel.get().getMinMaxX());
		}
		for (auto& panel : panels)
		{
			panel.get().setMaxX(maxX);
		}
	}

	static inline void alignBottom(std::initializer_list<std::reference_wrapper<DiwnePanel>> panels)
	{
		float maxY = std::numeric_limits<float>::lowest();
		for (auto& panel : panels)
		{
			maxY = std::max(maxY, panel.get().getMinMaxY());
		}
		for (auto& panel : panels)
		{
			panel.get().setMaxY(maxY);
		}
	}
};
} // namespace DIWNE
