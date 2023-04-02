#pragma once

#include "ColoredObject.h"

namespace Vp
{
class FrustumShader;

class FrustumObject : public ColoredObject
{
public:
	glm::mat4 m_frustumProjectionMatrix{1};
	glm::mat4 m_frustumViewMatrix{1};

	FrustumObject(Core::Mesh* mesh, FrustumShader* shader);

	void render(Shader* shader, glm::mat4 view, glm::mat4 projection, bool silhouette) override;
	void update(Scene& scene) override;
};
} // namespace Vp
