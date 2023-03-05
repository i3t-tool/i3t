#include "Utils/Math.h"

namespace Math
{
glm::mat4 lerp(const glm::mat4& lhs, const glm::mat4& rhs, float alpha, bool useQuat)
{
	if (!useQuat)
	{
		glm::mat4 result;

		result[0] = glm::mix(lhs[0], rhs[0], alpha);
		result[1] = glm::mix(lhs[1], rhs[1], alpha);
		result[2] = glm::mix(lhs[2], rhs[2], alpha);
		result[3] = glm::mix(lhs[3], rhs[3], alpha);

		return result;
	}

	auto q1 = glm::quat_cast(lhs);
	auto q2 = glm::quat_cast(rhs);
	auto result = glm::mix(q1, q2, alpha);

	return glm::mat4_cast(result);
}
}
