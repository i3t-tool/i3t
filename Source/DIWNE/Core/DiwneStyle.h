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

	// DropZone
	ImVec4 dropZoneBackground{0.14, 0.14, 0.14, 1.000};
	ImVec4 dropIndicatorColor{0.26, 0.59, 0.98, 0.67};
	ImVec2 dropZoneDropGap{10.0f, 10.0f};
	float dropZoneMarginWidth{10.0f};

	int nodeRounding{0};
	ImVec2 nodePadding = ImVec2(8.0f, 4.0f);

	void scale(float factor)
	{
		nodePadding *= factor;
	}
};

} // namespace DIWNE
