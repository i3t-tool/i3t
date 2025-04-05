#include "DiwneStyle.h"

#include "DIWNE/Core/diwne_common.h"
#include "GUI/Workspace/diwne_config.h"

namespace DIWNE
{
short DiwneStyle::counter = 0;
short DiwneStyle::sanityCounter = 0;

const short DiwneStyle::gridColor = counter++;
const short DiwneStyle::gridDotsColor = counter++;

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
const short DiwneStyle::dropZoneMargin = counter++;

const short DiwneStyle::linkColor = counter++;
const short DiwneStyle::linkColorSelected = counter++;
const short DiwneStyle::linkWidth = counter++;
const short DiwneStyle::linkSelectedBorderWidth = counter++;

DiwneStyle::DiwneStyle()
{
	sanityCounter = 0;

	set<ImVec4>(gridColor, ImColor(57, 57, 57, 120));
	set<ImVec4>(gridDotsColor, ImColor(200, 200, 200, 40));

	set<ImVec4>(nodeBg, {0.195, 0.195, 0.195, 0.6});
	set<ImVec4>(nodeHeaderBg, {0.020, 0.341, 0.012, 0.9});

	set<float>(nodeRounding, 0, true);
	set<ImVec2>(nodePadding, {8.0f, 4.0f}, true);

	set<float>(selectionRounding, 0.0f, true);
	set<float>(mouseDragThreshold, 2.0f, true); // 6.0 is ImGui default
	set<float>(itemSelectedBorderThicknessDiwne, 2.5f, true);
	set<float>(objectHoverBorderThicknessDiwne, 1.5f, true);
	set<ImVec4>(objectHoverBorderColor, {0.0, 0.0, 0.0, 0.40});

	set<ImVec4>(dropZoneBg, {0.1, 0.1, 0.1, 0.4});
	set<ImVec4>(dropIndicatorColor, {0.26, 0.59, 0.98, 0.67});
	set<ImVec2>(dropZoneDropGap, {10.0f, 10.0f}, true);
	set<ImVec2>(dropZoneMargin, {8.0f, 8.0f}, true);

	set<ImVec4>(linkColor, {0.6, 0.3, 0.35, 0.6});
	set<ImVec4>(linkColorSelected, {0.6, 0.3, 0.35, 0.8});
	set<float>(linkWidth, 5.0f, true);
	set<float>(linkSelectedBorderWidth, 4.0f, true);

	assert(counter == sanityCounter && "Are you sure all variables are set properly?");
}

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