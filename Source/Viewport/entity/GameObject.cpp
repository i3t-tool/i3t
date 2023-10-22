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
#include "GameObject.h"

#include "Viewport/shader/ObjectShader.h"
#include "Viewport/shader/Shaders.h"

using namespace Vp;

GameObject::GameObject(Core::Mesh* mesh, ObjectShader* shader) : m_mesh(mesh)
{
	m_shader = shader;
}

void GameObject::render(Shader* shader, glm::mat4 view, glm::mat4 projection, bool silhouette)
{
	auto objectShader = static_cast<ObjectShader*>(shader);
	objectShader->use();
	objectShader->m_wboit = m_wboit;
	objectShader->m_wboitFunc = m_wboitFunc;
	objectShader->m_opacity = m_opaque || silhouette ? 1.0f : m_opacity;
	objectShader->setWorldTransform(m_modelMatrix, view, projection);
	objectShader->setUniforms();

	if (m_mesh)
	{
		glBindVertexArray(m_mesh->m_vao);
		for (auto& meshPart : m_mesh->m_meshParts)
		{
			objectShader->setUniformsPerMeshPart(meshPart);
			m_mesh->renderMeshPart(meshPart);
		}
		glBindVertexArray(0);
	}
}

void GameObject::update(Scene& scene)
{
	// Empty
}

void GameObject::dispose()
{
	// Empty
}
