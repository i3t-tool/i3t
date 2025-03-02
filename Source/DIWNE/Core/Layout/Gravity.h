#pragma once

#include <array>

#include "DiwnePanel.h"

namespace DIWNE
{
class Gravity
{
public:
	static inline void alignRightEdge(std::initializer_list<std::reference_wrapper<DiwnePanel>> panels)
	{
		float maxX =
		    std::max(panels,
		             [](const std::reference_wrapper<DiwnePanel>& a, const std::reference_wrapper<DiwnePanel>& b) {
			             return a.get().getMaxX() < b.get().getMaxX();
		             })
		        .get()
		        .getMaxX();
		for (auto& panel : panels)
		{
			panel.get().setMaxX(maxX);
		}
	}

	static inline void alignBottom(std::initializer_list<std::reference_wrapper<DiwnePanel>> panels)
	{
		float maxY =
		    std::max(panels,
		             [](const std::reference_wrapper<DiwnePanel>& a, const std::reference_wrapper<DiwnePanel>& b) {
			             return a.get().getMaxY() < b.get().getMaxY();
		             })
		        .get()
		        .getMaxY();
		for (auto& panel : panels)
		{
			panel.get().setMaxY(maxY);
		}
	}
};
} // namespace DIWNE
