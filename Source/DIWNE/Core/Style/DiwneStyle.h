#pragma once

#include "DIWNE/Core/diwne_imgui.h"

#include <memory>
#include <utility>
#include <vector>

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
	//	std::vector<std::pair<int, std::shared_ptr<void>>> registry;
	std::vector<std::shared_ptr<void>> registry;

	static const short nodeBg; // ImVec4
	static const short nodeHeaderBg; // ImVec4

	static const short nodeRounding; // int
	static const short nodePadding; // float

	static const short dropZoneBg; // ImVec4
	static const short dropIndicatorColor; // ImVec4
	static const short dropZoneDropGap; // ImVec2
	static const short dropZoneMarginWidth; // float

	DiwneStyle()
	{
		set<ImVec4>(nodeBg, {0.195, 0.195, 0.195, 1.000});
		set<ImVec4>(nodeHeaderBg, {0.020, 0.341, 0.012, 1.000});

		set<float>(nodeRounding, 0);
		set<ImVec2>(nodePadding, {8.0f, 4.0f});

		set<ImVec4>(dropZoneBg, {0.14, 0.14, 0.14, 1.000});
		set<ImVec4>(dropIndicatorColor, {0.26, 0.59, 0.98, 0.67});
		set<ImVec2>(dropZoneDropGap, {10.0f, 10.0f});
		set<float>(dropZoneMarginWidth, 10.0f);
	}

	template <typename T>
	const T& get(short key) const
	{
		return *static_cast<T*>(registry[key].get());
	};

	template <typename T>
	void set(short key, T value)
	{
		if (registry.size() <= key)
			registry.resize((key + 1) * 1.5);
		registry[key] = std::make_shared<T>(value);
	}

	// clang-format off
	const ImVec4& color(short key) const { return get<ImVec4>(key); };
	const ImVec2& size(short key) const { return get<ImVec2>(key); };
	const float& decimal(short key) const { return get<float>(key); };
	const int& integer(short key) const { return get<int>(key); };
	// clang-format on

	void scale(float factor)
	{
		//		nodePadding *= factor;
	}

private:
	static short counter;
};

} // namespace DIWNE
