#pragma once

#include "WorldShader.h"

namespace Vp
{
class ObjectShader : public WorldShader
{
protected:
	GLint opacityId;

public:
	float m_opacity{1.0f};

	explicit ObjectShader(GLuint id);

	void init(bool initSuperclass) override;
	void setUniforms() override;
};

} // namespace Vp
