#pragma once

#include "diwne_imgui.h"

namespace DIWNE
{
/**
 * Each NodeEditor contains a DiwneStyle member that contains the default style settings for objects inside that node
 * editor. DiwneObjects can use these style variables directly or, to support custom styling of particular objects,
 * they can copy these style variables into their own member variables that can then further be modified to deviate
 * from these defaults.
 */
struct DiwneStyle
{
	ImVec4 nodeBackground{0.195, 0.195, 0.195, 1.000};
	ImVec4 nodeHeaderBackground{0.020, 0.341, 0.012, 1.000};
	int nodeRounding{0};
};

} // namespace DIWNE
