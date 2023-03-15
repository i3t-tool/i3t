#pragma once

#include <memory>

#include "Core/Resources/Mesh.h"
#include "Viewport/entity/Entity.h"

namespace Vp
{
class ObjectShader;

/**
 * \brief Regular world object entity. Represents a physical drawable object.
 */
class GameObject : public Entity
{
public:
	Core::Mesh* m_mesh;

	float m_opacity{1.0f};

	/**
	 * Creates an empty GameObject. Before rendering a mesh and a shader need to
	 * be set!
	 */
	GameObject() = default;

	GameObject(Core::Mesh* mesh, ObjectShader* shader);

	void render(Shader* shader, glm::mat4 view, glm::mat4 projection, bool silhouette) override;
	void update(Scene& scene) override;
	void dispose() override;
};
} // namespace Vp