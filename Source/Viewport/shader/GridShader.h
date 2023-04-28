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

	GLint m_gridColorId;
	GLint m_gridSizeId;
	GLint m_axisWidthId;
	GLint m_gridStrengthId;
	GLint m_gridFadeOffsetId;

public:
	glm::vec3 m_gridColor{glm::vec3(0.45, 0.49, 0.53)};
	float m_gridSize{1.0f};
	float m_axisWidth{0.9f};
	float m_gridStrength{0.5f};
	float m_gridFadeOffset{0.2f};

public:
	explicit GridShader(GLuint id);

	void init(bool initSuperclass) override;
	void setUniforms() override;
};
} // namespace Vp