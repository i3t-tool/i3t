#pragma once

#include <memory>
#include <unordered_map>
#include <utility>

#include "DIWNE/Core/diwne_common.h"
#include "DiwneStyle.h"

namespace DIWNE
{
struct StyleOverride
{
	std::unordered_map<int, std::shared_ptr<void>> overrides; ///< Overrides to the style
	DiwneStyle& style;                                        ///< The base style

	StyleOverride(DiwneStyle& style) : style(style) {}

	template <typename T>
	const T& get(short key)
	{
		if (overrides.empty() || overrides.find(key) == overrides.end())
			return style.get<T>(key);
		return *static_cast<T*>(overrides[key].get());
	};

	template <typename T>
	void addOverride(short key, T value)
	{
		if (key < 0 || key >= style.registry.size())
		{
			DIWNE_ERROR("Invalid style override! Key not present in base style.");
			return;
		}
		overrides[key] = std::make_shared<T>(value);
	}

	// clang-format off
	const ImVec4& color(short key) { return get<ImVec4>(key); };
	const ImVec2& size(short key) { return get<ImVec2>(key); };
	const float& decimal(short key) { return get<float>(key); };
	const int& integer(short key) { return get<int>(key); };
	// clang-format on
};

} // namespace DIWNE
