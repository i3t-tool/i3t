#pragma once

#include "ColorShader.h"

#include "pgr.h"

namespace Vp
{
/**
 * Shader for displaying projection frustums
 */
class FrustumShader : public ColorShader
{
public:
	GLint inverseProjectionMatrixId;

	glm::mat4 m_frustumProjectionMatrix;
	glm::mat4 m_frustumViewMatrix;

	explicit FrustumShader(GLuint id);

	void setUniforms() override;
};
} // namespace Vp