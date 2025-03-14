#pragma once

#include <vector>

#include "DiwnePanel.h"

namespace DIWNE
{
class HStack
{
	std::vector<DiwnePanel> children;
	float spacing = 0.0f;

	HStack(std::vector<DiwnePanel>&& children);

	void begin();
	void end();
};

} // namespace DIWNE
