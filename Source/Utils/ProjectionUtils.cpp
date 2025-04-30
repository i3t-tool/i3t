#include "ProjectionUtils.h"

namespace ProjectionUtils
{

void extractZNearZFar(const glm::mat4& projection, float& zNear, float& zFar)
{
	// Extract near and far values from projection matrix
	// Note that using this method there can be a relatively "HUGE" precision error
	// It might not suitable to use this method for something like comparing depth values (as near/far values will
	// shift)
	if (projection[2][3] == 0)
	{
		// Orthographic matrix
		float m32 = projection[3][2];
		float m22 = projection[2][2];
		zNear = (m32 + 1) / m22;
		zFar = (m32 - 1) / m22;
	}
	else
	{
		// Perspective matrix
		float m32 = projection[3][2];
		float m22 = projection[2][2];
		zNear = m32 / (m22 - 1);
		zFar = m32 / (m22 + 1);
	}
}

std::array<float, 6> decomposePerspective(const glm::mat4& m)
{
	float near;
	float far;
	extractZNearZFar(m, near, far);

	float bottom = near * (m[2][1] - 1) / m[1][1];
	float top = near * (m[2][1] + 1) / m[1][1];

	float left = near * (m[2][0] - 1) / m[1][1];
	float right = near * (m[2][0] + 1) / m[1][1];

	return {left, right, bottom, top, near, far};
}

std::array<glm::mat4, 2> constructPositiveZPerspective(const glm::mat4& m)
{
	glm::mat4 negator(1.f);
	negator[2][2] = -1;
	return {negator, negator * m};
}

// std::array<glm::mat4, 3> constructPiecewisePerspective(float l, float r, float b, float t, float n, float f)
// {
// 	glm::mat4 return
// 	{
// 		{
// 			{}
// 		}
// 	}
// }

} // namespace ProjectionUtils
