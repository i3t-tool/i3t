/**
 * \file
 * \brief
 * \author Dan Rakušan <rakusan.dan@gmail.com>
 * \copyright Copyright (C) 2016-2023 I3T team, Department of Computer Graphics
 * and Interaction, FEE, Czech Technical University in Prague, Czech Republic
 *
 * This file is part of I3T - An Interactive Tool for Teaching Transformations
 * http://www.i3t-tool.org
 *
 * GNU General Public License v3.0 (see LICENSE.txt or https://www.gnu.org/licenses/gpl-3.0.txt)
 */
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

	// TODO: (DR) Move this to entity I suppose
	float m_opacity{0.5f};

	/**
	 * Creates an empty GameObject. Before rendering a mesh and a shader need to
	 * be set!
	 */
	GameObject() = default;

	GameObject(Core::Mesh* mesh, ObjectShader* shader);

	void prepareRenderContext(RenderContext& context) override;

	void render(const glm::mat4& model, const glm::mat4& view, const glm::mat4& projection,
	            const RenderContext& context) override;

	void update(Scene& scene) override;
	void dispose() override;
};
} // namespace Vp