#include "DiwneStyle.h"

#include "DIWNE/Core/diwne_common.h"
#include "GUI/Workspace/diwne_config.h"

namespace DIWNE
{
short DiwneStyle::counter = 0;
short DiwneStyle::sanityCounter = 0;

const short DiwneStyle::nodeBg = counter++;
const short DiwneStyle::nodeHeaderBg = counter++;

const short DiwneStyle::nodeRounding = counter++;
const short DiwneStyle::nodePadding = counter++;

const short DiwneStyle::selectionRounding = counter++;
const short DiwneStyle::mouseDragThreshold = counter++;
const short DiwneStyle::itemSelectedBorderThicknessDiwne = counter++;
const short DiwneStyle::objectHoverBorderThicknessDiwne = counter++;
const short DiwneStyle::objectHoverBorderColor = counter++;

const short DiwneStyle::dropZoneBg = counter++;
const short DiwneStyle::dropIndicatorColor = counter++;
const short DiwneStyle::dropZoneDropGap = counter++;
const short DiwneStyle::dropZoneMarginWidth = counter++;

const short DiwneStyle::linkColor = counter++;
const short DiwneStyle::linkColorSelected = counter++;
const short DiwneStyle::linkWidth = counter++;
const short DiwneStyle::linkSelectedBorderWidth = counter++;

void DiwneStyle::scaleAllSizes(float scale)
{
	if (scale == 1.0f)
		return;
	for (auto& entry : registry)
	{
		if (!entry.dpiScaled)
			continue;
		if (entry.type == typeid(float))
		{
			*std::static_pointer_cast<float>(entry.var) *= scale;
		}
		else if (entry.type == typeid(ImVec2))
		{
			*std::static_pointer_cast<ImVec2>(entry.var) *= scale;
		}
		else if (entry.type == typeid(int))
		{
			std::shared_ptr<int> ptr = std::static_pointer_cast<int>(entry.var);
			*ptr = static_cast<int>(*ptr * scale);
		}
		// ImVec4 is not dpi scaled, presumed to always be a color
	}
}

void DiwneStyle::copyRegistry(std::vector<StyleEntry>& target) const
{
	target.clear();
	for (const auto& entry : registry)
	{
		if (entry.type == typeid(float))
		{
			auto newVar = std::make_shared<float>(*std::static_pointer_cast<float>(entry.var));
			target.emplace_back(std::move(newVar), entry.dpiScaled, typeid(float));
		}
		else if (entry.type == typeid(int))
		{
			auto newVar = std::make_shared<int>(*std::static_pointer_cast<int>(entry.var));
			target.emplace_back(std::move(newVar), entry.dpiScaled, typeid(int));
		}
		else if (entry.type == typeid(ImVec2))
		{
			auto newVar = std::make_shared<ImVec2>(*std::static_pointer_cast<ImVec2>(entry.var));
			target.emplace_back(std::move(newVar), entry.dpiScaled, typeid(ImVec2));
		}
		else if (entry.type == typeid(ImVec4))
		{
			auto newVar = std::make_shared<ImVec4>(*std::static_pointer_cast<ImVec4>(entry.var));
			target.emplace_back(std::move(newVar), entry.dpiScaled, typeid(ImVec4));
		}
		else
		{
			DIWNE_FAIL("[DIWNE] Encountered invalid DiwneStyle type!");
		}
	}
}
} // namespace DIWNE