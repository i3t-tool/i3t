/**
 * \file
 * \brief
 * \author Dan Rakušan <rakusan.dan@gmail.com>
 * \copyright Copyright (C) 2016-2025 I3T team, Department of Computer Graphics
 * and Interaction, FEE, Czech Technical University in Prague, Czech Republic
 *
 * This file is part of I3T - An Interactive Tool for Teaching Transformations
 * http://www.i3t-tool.org
 *
 * GNU General Public License v3.0 (see LICENSE.txt or https://www.gnu.org/licenses/gpl-3.0.txt)
 */
#pragma once

#include <memory>
#include <typeindex>
#include <utility>
#include <vector>

#include "Style.h"

namespace DIWNE
{
/**
 * Default base style. Implementation of the Style interface.
 *
 * Each NodeEditor contains a StyleBase member that contains the default style settings for objects inside that node
 * editor. The base style can be modifed by setting a DiwneObject's StyleOverride.
 **
 * Style variables are accessed using the StyleBase::get(key) method, key being one style variable enum values.
 * Variables themselves are stored in a std::vector registry and accessed by the key (index based access).
 * This is different from how, for example, ImGuiStyle works where each variable is just a plain variable in a struct.
 *
 * The base style is DPI-aware.
 * It has a dpiScale parameter to scale relevant style variables at runtime by the UI DPI scaling factor.
 * Whether a particular theme variable will be scaled in that call depends on an internal dpiScaled flag that
 * can be passed to the StyleBase::set() method during style initialization.
 *
 * StyleBase::setDpiScale() should be called on DPI scale change.
 *
 * Note that StyleBase variables are not automatically scaled by the DIWNE NodeEditor zoom factor.
 * This must be done manually on case by case basis inside DIWNE code.
 */
class StyleBase : public Style
{
public:
	float dpiScale = 1.0f;

	StyleBase();

	const ImVec4& color(Style::Var key) const override;
	ImVec2 size(Style::Var key) const override;
	float decimal(Style::Var key) const override;
	int integer(Style::Var key) const override;
	bool boolean(Style::Var key) const override;

	/**
	 * Set style variable value.
	 * @param key Style variable key to set
	 * @param value Style value, passed as a copy. To pass a pointer see the pointer method overload.
	 * @param dpiScaled Whether to scale the style variable with dpi scaling.
	 */
	template <typename T, typename std::enable_if<!std::is_pointer<T>::value, bool>::type = true>
	void set(Style::Var key, T value, bool dpiScaled = false)
	{
		setImpl(key, value, static_cast<T*>(nullptr), dpiScaled, false);
	}

	/**
	 * Set style variable from a pointer, so that it can be modified dynamically.
	 * @param key Style variable key to set
	 * @param ptr Pointer to the style value, ownership will NOT be transferred (memory is managed externally)
	 * @param dpiScaled Whether to scale the style variable with dpi scaling.
	 */
	template <typename T, typename std::enable_if<std::is_pointer<T>::value, bool>::type = true>
	void set(Style::Var key, T ptr, bool dpiScaled = false)
	{
		assert(ptr);
		setImpl(key, *ptr, ptr, dpiScaled, false);
	}

private:
	struct StyleEntry
	{
		std::shared_ptr<void> var; // Could be using a unique_ptr with a custom deleter, or raw pointers
		bool dpiScaled{false};
		std::type_index type{typeid(void)};
		StyleEntry() = default;
		StyleEntry(const std::shared_ptr<void>& var, bool dpiScaled, const std::type_index& type)
		    : var(var), dpiScaled(dpiScaled), type(type)
		{}
	};

public:
	template <typename T>
	const T& get(Style::Var key) const
	{
		return getImpl<T>(key);
	}

	template <typename T>
	T* getPtr(Style::Var key) const
	{
		return &getImpl<T>(key);
	}

	void setDpiScale(float scale)
	{
		dpiScale = scale;
	}
	float getDpiScale() const
	{
		return dpiScale;
	}

	// Copy and move constructor (the registry is not implicitly copyable)
	StyleBase(const StyleBase& other)
	{
		other.copyRegistry(this->registry);
	}
	StyleBase& operator=(const StyleBase& other)
	{
		if (this == &other)
			return *this;
		other.copyRegistry(this->registry);
		return *this;
	}

	StyleBase(StyleBase&& other) noexcept : registry(std::move(other.registry)) {}
	StyleBase& operator=(StyleBase&& other) noexcept
	{
		if (this == &other)
			return *this;
		registry = std::move(other.registry);
		return *this;
	}

private:
	template <typename T>
	T& getImpl(Style::Var key, const StyleEntry** entryPtr = nullptr) const
	{
#ifndef NDEBUG
		assert(key >= 0 && key < registry.size() && "Invalid StyleEntry key!");
#endif
		const StyleEntry* entry = &registry[key];
		if (entryPtr)
			*entryPtr = entry;
#ifndef NDEBUG
		assert(entry->var && "StyleEntry shouldn't be empty when fetched!");
		assert(entry->type == typeid(T) && "Requesting invalid StyleEntry type!");
#endif
		return *static_cast<T*>(entry->var.get());
	}

	template <typename T>
	void setImpl(Style::Var key, T value, T* ptr, bool dpiScaled, bool useDefaults)
	{
		assert(key >= 0);
		bool newKey = registry.size() <= key;
		if (newKey)
			registry.resize(key + 1);

		std::shared_ptr<void> variablePtr;
		if (ptr == nullptr)
		{
			variablePtr = std::make_shared<T>(value);
		}
		else
		{
			auto nonOwningDeleter = [](void*) { /* do nothing */ };
			variablePtr = std::shared_ptr<void>(ptr, nonOwningDeleter);
		}

		if (!newKey)
		{
			auto& entry = registry[key];
			if (!useDefaults)
				entry.dpiScaled = dpiScaled;
			entry.var = std::move(variablePtr);
			entry.type = typeid(T);
		}
		else
		{
			registry[key] = {std::move(variablePtr), dpiScaled, typeid(T)};
		}
		sanityCounter++;
	}

	friend class StyleOverride;

	std::vector<StyleEntry> registry;
	static short sanityCounter;

	void copyRegistry(std::vector<StyleEntry>& target) const;
};

} // namespace DIWNE
