#pragma once

#include "DIWNE/Core/diwne_imgui.h"

#include <memory>
#include <typeindex>
#include <utility>
#include <vector>

namespace DIWNE
{
/**
 * Each NodeEditor contains a DiwneStyle member that contains the default style settings for objects inside that node
 * editor. DiwneObjects can use these style variables directly or, to support custom styling of particular objects,
 * they can copy these style variables into their own member variables that can then further be modified to deviate
 * from these defaults.
 *
 * The default mechanism to do so is using the StyleOverride object, which can be used to override specific style
 * variables and fallback to the underlying style otherwise.
 *
 * Style variables are accessed using the DiwneStyle::get(key) method, key being one of the static short key variables.
 * Variables themselves are stored in a std::vector registry and accessed by the key index.
 * This is different from how, for example, ImGuiStyle works where each variable is just a plain variable in a struct.
 *
 * Styles are DPI-aware, but scaling should be done manually on the whole object using scaleAllSizes().
 * Whether a particular theme variable will be scaled in that call depends on an internal dpiScaled flag that
 * can be passed to the DiwneStyle::set() method.
 *
 * The way to handle DPI is to create a new DiwneStyle object when DPI changes, scale it and apply any additional
 * modifications. It can be then set to replace the original unscaled style using the NodeEditor::setStyle() method.
 *
 * \code{.cpp}
 * auto style = std::make_unique<DIWNE::DiwneStyle>();
 * // Modify style pre-scaling
 * style->scaleAllSizes(dpiScale);
 * // or post-scaling
 * editor->setStyle(std::move(style));
 * \endcode
 *
 * Note that DiwneStyle variables are not automatically scaled by the DIWNE NodeEditor zoom factor.
 * This is to be done manually inside DIWNE code.
 */
struct DiwneStyle
{
private:
	struct StyleEntry
	{
		std::shared_ptr<void> var;
		bool dpiScaled{false};
		std::type_index type{typeid(void)};
		StyleEntry() = default;
		StyleEntry(const std::shared_ptr<void>& var, bool dpiScaled, const std::type_index& type)
		    : var(var), dpiScaled(dpiScaled), type(type)
		{}
	};

public:
	std::vector<StyleEntry> registry;

	// Copy and move constructor (the registry is not implicitly copyable)
	DiwneStyle(const DiwneStyle& other)
	{
		other.copyRegistry(this->registry);
	}
	DiwneStyle& operator=(const DiwneStyle& other)
	{
		if (this == &other)
			return *this;
		other.copyRegistry(this->registry);
		return *this;
	}

	DiwneStyle(DiwneStyle&& other) noexcept : registry(std::move(other.registry)) {}
	DiwneStyle& operator=(DiwneStyle&& other) noexcept
	{
		if (this == &other)
			return *this;
		registry = std::move(other.registry);
		return *this;
	}

	// Style variables can be accessed by the get(key) method
	// The key is one of the static variables below
	// Instead of an enum, static shorts are used.
	// Real style values are kept in a std::vector registry that is accessed by the key index

	static const short nodeBg;       // ImVec4
	static const short nodeHeaderBg; // ImVec4

	static const short nodeRounding; // int
	static const short nodePadding;  // float

	static const short selectionRounding;                // float
	static const short mouseDragThreshold;               // float
	static const short itemSelectedBorderThicknessDiwne; // float
	static const short objectHoverBorderThicknessDiwne;  // float
	static const short objectHoverBorderColor;           // ImVec4

	static const short dropZoneBg;          // ImVec4
	static const short dropIndicatorColor;  // ImVec4
	static const short dropZoneDropGap;     // ImVec2
	static const short dropZoneMarginWidth; // float

	static const short linkColor;               // ImVec4
	static const short linkColorSelected;       // ImVec4
	static const short linkWidth;               // float
	static const short linkSelectedBorderWidth; // float

	// TODO: I don't really like this system
	// TODO: How do we allow components to have their own specific colors?
	// TODO: But without storing another separate DiwneStyle
	// TODO: Maybe each object type could have its own reduced DiwneStyle?
	//   and then each hold an instance -> big memory usage wasted if most objects arent using custom overrides
	// TODO: All defaults need to be easily modifiable at runtime
	// TODO: The StyleOverride system adds 80 bytes to each object + small runtime cost for fetching (map lookup)
	//   maybe the memory cost isn't that bad? Could be avoided by just holding a pointer to the map and allocating that
	//   dynamically, kinda like PIMPL
	// TODO: What about the runtime cost though?

	DiwneStyle()
	{
		sanityCounter = 0;
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
		set<float>(dropZoneMarginWidth, 10.0f, true);

		set<ImVec4>(linkColor, {0.6, 0.3, 0.35, 0.6});
		set<ImVec4>(linkColorSelected, {0.6, 0.3, 0.35, 0.8});
		set<float>(linkWidth, 5.0f, true);
		set<float>(linkSelectedBorderWidth, 4.0f, true);

		assert(counter == sanityCounter && "Are you sure all variables are set properly?");
	}

	template <typename T>
	const T& get(short key) const
	{
#ifndef NDEBUG
		assert(key >= 0 && key < registry.size() && "Invalid StyleEntry key!");
		auto& entry = registry[key];
		assert(entry.var && "StyleEntry shouldn't be empty when fetched!");
		assert(entry.type == typeid(T) && "Requesting invalid StyleEntry type!");
#endif
		return *static_cast<T*>(registry[key].var.get());
	}

	template <typename T>
	void setImpl(short key, T value, bool dpiScaled, bool useDefaults)
	{
		assert(key >= 0);
		bool newKey = registry.size() <= key;
		if (newKey)
			registry.resize(key + 1);
		if (!newKey)
		{
			auto& entry = registry[key];
			if (!useDefaults)
				entry.dpiScaled = dpiScaled;
			entry.var = std::make_shared<T>(value);
			entry.type = typeid(T);
		}
		else
		{
			registry[key] = {std::make_shared<T>(value), dpiScaled, typeid(T)};
		}
		sanityCounter++;
	}

	template <typename T>
	void set(short key, T value, bool dpiScaled)
	{
		setImpl(key, value, dpiScaled, false);
	}

	template <typename T>
	void set(short key, T value)
	{
		setImpl(key, value, false, true);
	}

	// clang-format off
	const ImVec4& color(short key) const { return get<ImVec4>(key); };
	const ImVec2& size(short key) const { return get<ImVec2>(key); };
	const float& decimal(short key) const { return get<float>(key); };
	const int& integer(short key) const { return get<int>(key); };
	// clang-format on

	void scaleAllSizes(float scale);

private:
	void copyRegistry(std::vector<StyleEntry>& target) const;

	static short counter;
	static short sanityCounter;
};

} // namespace DIWNE
