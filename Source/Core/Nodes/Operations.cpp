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
