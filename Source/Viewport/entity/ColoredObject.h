#pragma once

#include "GameObject.h"

namespace Vp
{
class ColorShader;

class ColoredObject : public GameObject
{
public:
	bool m_useSingleColor{false};
	glm::vec3 m_singleColor{1.0f, 0.0f, 1.0f};

	ColoredObject(Core::Mesh* mesh, ColorShader* shader);

	void render(Shader* shader, glm::mat4 view, glm::mat4 projection, bool silhouette) override;

	const glm::vec3& getColor() const;
	void setColor(const glm::vec3* color);
	void setColor(const glm::vec3& color);
};
} // namespace Vp
