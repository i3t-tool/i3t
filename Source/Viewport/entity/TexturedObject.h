#pragma once

#include "GameObject.h"

namespace Vp
{
class PhongShader;

class TexturedObject : public GameObject
{
public:
	glm::vec3 m_tint{1.0f};

	TexturedObject(Core::Mesh* mesh, PhongShader* shader);

	void render(Shader* shader, glm::mat4 view, glm::mat4 projection, bool silhouette) override;
};
} // namespace Vp
