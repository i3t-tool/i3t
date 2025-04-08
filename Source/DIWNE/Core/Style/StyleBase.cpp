#include "StyleBase.h"

#include "DIWNE/Core/diwne_common.h"

namespace DIWNE
{
short StyleBase::sanityCounter = 0;

StyleBase::StyleBase()
{
	sanityCounter = 0;

	set<ImVec4>(GRID_COLOR, ImColor(57, 57, 57, 120));
	set<ImVec4>(GRID_DOTS_COLOR, ImColor(200, 200, 200, 40));

	set<ImVec4>(NODE_BG, {0.195, 0.195, 0.195, 0.6});
	set<ImVec4>(NODE_HEADER_BG, {0.020, 0.341, 0.012, 0.9});

	set<float>(NODE_ROUNDING, 0, true);
	set<ImVec2>(NODE_PADDING, {8.0f, 4.0f}, true);

	set<float>(SELECTION_ROUNDING, 0.0f, true);
	set<float>(MOUSE_DRAG_THRESHOLD, 2.0f, true); // 6.0 is ImGui default
	set<float>(SELECTED_BORDER_WIDTH, 2.5f, true);
	set<ImVec4>(SELECTED_BORDER_COLOR, {1.0f, 0.9f, 0.4f, 0.6f});
	set<float>(HOVER_BORDER_WIDTH, 1.5f, true);
	set<ImVec4>(HOVER_BORDER_COLOR, {0.0, 0.0, 0.0, 0.40});

	set<ImVec4>(DROP_ZONE_BG, {0.1, 0.1, 0.1, 0.4});
	set<ImVec4>(DROP_INDICATOR_COLOR, {0.26, 0.59, 0.98, 0.67});
	set<ImVec2>(DROP_ZONE_DROP_GAP, {10.0f, 10.0f}, true);
	set<ImVec2>(DROP_ZONE_MARGIN, {8.0f, 8.0f}, true);

	set<ImVec4>(LINK_COLOR, {0.6, 0.3, 0.35, 0.6});
	set<ImVec4>(LINK_COLOR_SELECTED, {0.6, 0.3, 0.35, 0.8});
	set<float>(LINK_UNPLUGGED_ALPHA, 0.5f);
	set<float>(LINK_WIDTH, 5.0f, true);
	set<float>(LINK_SELECTED_WIDTH, 7.0f, true);
	set<float>(LINK_BORDER_WIDTH, 0.0f, true);
	set<ImVec4>(LINK_BORDER_COLOR, {0.0, 0.0, 0.0, 0.23});

	set<ImVec2>(PIN_SPACING, {4.0f, 4.0f}, true);
	set<float>(PIN_LABEL_SPACING, 4.0f, true);
	set<bool>(PIN_ENABLE_DRAG_LABEL, true);
	set<bool>(PIN_ENABLE_HOVER_BG, true);
	set<ImVec2>(PIN_BG_SPACING, {4.0f, 2.0f}, true);
	set<ImVec4>(PIN_BG_COLOR, {0.0, 0.0, 0.0, 0.14});
	set<float>(PIN_BG_ROUNDING, 5.0f, true);
	set<ImVec4>(PIN_HOVER_COLOR_SHIFT, {0.1f, 0.1f, 0.1f, 0.0f});

	set<float>(PIN_SOCKET_OFFSET, 4.0f, true);
	set<float>(PIN_SOCKET_BORDER_WIDTH, 0.0f, true);
	set<ImVec4>(PIN_SOCKET_BORDER_COLOR, {0.f, 0.f, 0.f, 0.25f});
	set<float>(PIN_SOCKET_BORDER_ROUNDING, 4.0f, true);
	set<ImVec4>(PIN_SOCKET_BG, {0.f, 0.f, 0.f, 0.31f});
	set<float>(PIN_SOCKET_THICKNESS, 2.0f, true);
	set<float>(PIN_SOCKET_CONNECTED_GAP, 2.0f, true);

	set<float>(PIN_SQUARE_OFFSET, 4.0f, true);
	set<float>(PIN_SQUARE_BORDER_WIDTH, 0.0f, true);
	set<ImVec4>(PIN_SQUARE_BORDER_COLOR, {0.f, 0.f, 0.f, 0.25f});
	set<float>(PIN_SQUARE_ROUNDING, 2.0f, true);

	set<bool>(PIN_DIM_ENABLED, true);
	set<float>(PIN_DIM_ALPHA, 0.35f);

	set<ImVec4>(SELECTION_RECT_FULL_COLOR, {0.0, 0.0, 1.0, 0.1});
	set<ImVec4>(SELECTION_RECT_TOUCH_COLOR, {0.0, 1.0, 0.0, 0.1});
	set<float>(SELECTION_RECT_ALPHA, 0.8f);

	assert(Style::Var::STYLE_VAR_COUNT == sanityCounter && "Are you sure all variables are set properly?");
}

const ImVec4& StyleBase::color(Style::Var key) const
{
	return get<ImVec4>(key);
}
ImVec2 StyleBase::size(Style::Var key) const
{
	const StyleEntry* entry = nullptr;
	ImVec2 ret = getImpl<ImVec2>(key, &entry);
	if (entry->dpiScaled)
		ret *= dpiScale;
	return ret;
}
float StyleBase::decimal(Style::Var key) const
{
	const StyleEntry* entry = nullptr;
	float ret = getImpl<float>(key, &entry);
	if (entry->dpiScaled)
		ret *= dpiScale;
	return ret;
}
int StyleBase::integer(Style::Var key) const
{
	return get<int>(key);
}
bool StyleBase::boolean(Style::Var key) const
{
	return get<bool>(key);
}

// void StyleBase::scaleAllSizes(float scale)
// {
// 	if (scale == 1.0f)
// 		return;
// 	for (auto& entry : registry)
// 	{
// 		if (!entry.dpiScaled)
// 			continue;
// 		if (entry.type == typeid(float))
// 		{
// 			*std::static_pointer_cast<float>(entry.var) *= scale;
// 		}
// 		else if (entry.type == typeid(ImVec2))
// 		{
// 			*std::static_pointer_cast<ImVec2>(entry.var) *= scale;
// 		}
// 		else if (entry.type == typeid(int))
// 		{
// 			std::shared_ptr<int> ptr = std::static_pointer_cast<int>(entry.var);
// 			*ptr = static_cast<int>(*ptr * scale);
// 		}
// 		// ImVec4 is not dpi scaled, presumed to always be a color
// 	}
// }

void StyleBase::copyRegistry(std::vector<StyleEntry>& target) const
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
		else if (entry.type == typeid(bool))
		{
			auto newVar = std::make_shared<bool>(*std::static_pointer_cast<bool>(entry.var));
			target.emplace_back(std::move(newVar), entry.dpiScaled, typeid(bool));
		}
		else
		{
			DIWNE_FAIL("[DIWNE] Encountered invalid StyleBase type!");
		}
	}
}
} // namespace DIWNE