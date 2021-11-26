#include "Operations.h"

namespace Core
{
template <typename T, typename... Args>
void forTransform(ETransformType transformType, T&& fn, Args&&... args)
{
	constexpr auto& transforms = magic_enum::enum_values<ETransformType>();

	for (const auto& transform : transforms)
		if (transform == transformType)
			fn(std::forward<Args>(args)...);
}
}
