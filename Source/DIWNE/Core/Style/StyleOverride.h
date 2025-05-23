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
#include <unordered_map>

#include "StyleBase.h"

#include "DIWNE/Core/NodeEditor.h"
#include "DIWNE/Core/diwne_common.h"

namespace DIWNE
{
/**
 * Style interface implementation that overrides certain style variables of an underlying StyleBase
 * of a particular NodeEditor. If this object does not contain an entry for a style variable,
 * it falls back to the base style. Modifications are stored in a hash map. This object can be set to multiple
 * DiwneObject's which will then return it in their DiwneObject::style() methods, instead of the NodeEditor base style.
 *
 * DPI scaling it performed at runtime and inferred from the base style.
 */
class StyleOverride : public Style
{
	std::unordered_map<Style::Var, std::shared_ptr<void>> overrides; ///< Overrides to the style
	NodeEditor* editor{nullptr}; ///< The associated node editor (from which the base style is taken)

public:
	StyleOverride() = default;
	StyleOverride(NodeEditor* editor) : editor(editor) {}

	void setEditor(NodeEditor* editor)
	{
		this->editor = editor;
	}

	template <typename T, typename std::enable_if<!std::is_pointer<T>::value, bool>::type = true>
	void setOverride(Style::Var key, T value)
	{
		if (!editor)
			throw std::runtime_error("DIWNE: StyleOverride has no editor set!");
		if (key < 0 || key >= editor->styleBase().registry.size())
		{
			DIWNE_ERROR("Invalid style override! Key not present in base style.");
			return;
		}
		overrides[key] = std::make_shared<T>(value);
	}

	// TODO: Implement
	template <typename T, typename std::enable_if<std::is_pointer<T>::value, bool>::type = true>
	void setOverride(Style::Var key, T ptr)
	{
		if (!editor)
			throw std::runtime_error("DIWNE: StyleOverride has no editor set!");
		if (key < 0 || key >= editor->styleBase().registry.size())
		{
			DIWNE_ERROR("Invalid style override! Key not present in base style.");
			return;
		}
		auto nonOwningDeleter = [](void*) { /* do nothing */ };
		overrides[key] = std::shared_ptr<void>(ptr, nonOwningDeleter);
	}

	const ImVec4& color(Style::Var key) const override;
	ImVec2 size(Style::Var key) const override;
	float decimal(Style::Var key) const override;
	int integer(Style::Var key) const override;
	bool boolean(Style::Var key) const override;

	template <typename T>
	const T& get(Style::Var key) const
	{
		return getImpl<T>(key);
	}

private:
	template <typename T>
	const T& getImpl(Style::Var key, const StyleBase::StyleEntry** entry = nullptr) const
	{
		if (!editor)
			throw std::runtime_error("DIWNE: StyleOverride has no editor set!");
		const T& originalVal = editor->styleBase().getImpl<T>(key, entry);
		if (overrides.empty() || overrides.find(key) == overrides.end())
		{
			return originalVal;
		}
		return *static_cast<T*>(overrides.at(key).get());
	};
};
} // namespace DIWNE
