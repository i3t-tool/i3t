#pragma once

#include "ObjectShader.h"

namespace Vp
{
/**
 * Shader for the infinite grid
 */
class GridShader : public ObjectShader
{
protected:
	GLint m_nearId;
	GLint m_farId;

public:
	explicit GridShader(GLuint id);

	void setUniforms() override;
};
} // namespace Vp