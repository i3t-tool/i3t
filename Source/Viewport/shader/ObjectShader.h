#pragma once

#include "WorldShader.h"

namespace Vp
{
class ObjectShader : public WorldShader
{
protected:
	GLint opacityId;

public:
	float m_opacity;

	explicit ObjectShader(GLuint id);

	void setUniforms() override;
};

} // namespace Vp
