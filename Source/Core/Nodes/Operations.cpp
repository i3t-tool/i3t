#include "Operations.h"

namespace Core
{
const TransformOperation& getTransformProps(const Operation* op)
{
	const auto transformTypeResult = magic_enum::enum_cast<ETransformType>(op->keyWord);
	I3T_ASSERT(transformTypeResult.has_value() && "Not a transform!");

	return getTransformOperation(transformTypeResult.value());
}
}
