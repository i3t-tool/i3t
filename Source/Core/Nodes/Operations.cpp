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
#include "Operations.h"

namespace Core
{
bool TransformOperation::hasDefaultValue(const std::string& name) const
{
	return getDefaultValueType(name).has_value();
}

std::optional<TransformOperation::NameTypePair> TransformOperation::getDefaultValueType(const std::string& name) const
{
	const auto it = std::find_if(defaultValuesTypes.begin(), defaultValuesTypes.end(), [&name](const auto& value) {
		return value.name == name;
	});

	if (it == defaultValuesTypes.end())
	{
		return std::nullopt;
	}

	return *it;
}

std::optional<TransformOperation*> getTransformOperation(const std::string& keyWord)
{
	auto type = magic_enum::enum_cast<ETransformType>(keyWord);
	if (!type.has_value())
	{
		return std::nullopt;
	}

	return (TransformOperation*) &getTransformOperation(*type);
}
} // namespace Core
