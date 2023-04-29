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

	GLint m_showGridId;
	GLint m_showXAxisId;
	GLint m_showYAxisId;
	GLint m_showZAxisId;

	GLint m_gridColorId;
	GLint m_axisXColorId;
	GLint m_axisYColorId;
	GLint m_axisZColorId;

	GLint m_gridSizeId;
	GLint m_gridStrengthId;

	GLint m_lineWidthId;

	GLint m_grid1FadeStartId;
	GLint m_grid1FadeEndId;
	GLint m_grid2FadeStartId;
	GLint m_grid2FadeEndId;

public:
	bool m_showGrid = true;
	bool m_showXAxis = true;
	bool m_showYAxis = true;
	bool m_showZAxis = true;

	glm::vec3 m_gridColor{glm::vec3(0.45, 0.49, 0.53)};
	glm::vec3 m_axisXColor{glm::vec3(1.0, 0.49, 0.53)};
	glm::vec3 m_axisYColor{glm::vec3(0.41, 0.96, 0.49)};
	glm::vec3 m_axisZColor{glm::vec3(0.45, 0.49, 1.0)};

	float m_gridSize{1.0f};
	float m_gridStrength{0.5f};

	float m_lineWidth{1.0f};

	float m_grid1FadeStart{0.0f};
	float m_grid1FadeEnd{0.4f};
	float m_grid2FadeStart{0.1f};
	float m_grid2FadeEnd{1.0f};

public:
	explicit GridShader(GLuint id);

	void init(bool initSuperclass) override;
	void setUniforms() override;
};
} // namespace Vp