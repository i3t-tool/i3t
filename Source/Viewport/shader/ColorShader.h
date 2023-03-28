#pragma once

#include "ObjectShader.h"

namespace Vp
{
/**
 * Basic shader to display unlit colored primitives.
 */
class ColorShader : public ObjectShader
{
protected:
	GLint useSingleColorId;
	GLint singleColorId;

public:
	bool m_useSingleColor{false};
	glm::vec3 m_singleColor{1.0f, 0.0f, 1.0f};

	explicit ColorShader(GLuint id);

	void init(bool initSuperclass) override;
	void setUniforms() override;
};
} // namespace Vp