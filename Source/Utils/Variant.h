/**
 * \file
 * \brief
 * \author Martin Herich <martin.herich@phire.cz>
 * \copyright Copyright (C) 2016-2023 I3T team, Department of Computer Graphics
 * and Interaction, FEE, Czech Technical University in Prague, Czech Republic
 *
 * This file is part of I3T - An Interactive Tool for Teaching Transformations
 * http://www.i3t-tool.org
 *
 * GNU General Public License v3.0 (see LICENSE.txt or https://www.gnu.org/licenses/gpl-3.0.txt)
 */
#pragma once

#include <variant>

template <typename VariantType, typename T, std::size_t index = 0>
inline constexpr size_t variant_index()
{
	static_assert(std::variant_size_v<VariantType> > index, "Type not found in variant!");
	if constexpr (index == std::variant_size_v<VariantType>)
	{
		return index;
	}
	else if constexpr (std::is_same_v<std::variant_alternative_t<index, VariantType>, T>)
	{
		return index;
	}
	else
	{
		return variant_index<VariantType, T, index + 1>();
	}
}
