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

void GameObject::prepareRenderContext(RenderContext& context)
{
	switch (context.m_renderType)
	{
	case RenderType::SILHOUETTE:
	case RenderType::NORMAL:
	{
		if (context.m_shader == nullptr)
			context.m_shader = m_shader;

		context.m_opaque = this->m_opaque;
		context.m_opacity = this->m_opacity;
		context.m_wboit = this->m_wboit;
		context.m_wboitFunc = this->m_wboitFunc;
		break;
	}
	// case RenderType::DEPTH:
	// {
	// 	DepthShader* depthShader = SHADERS.getShaderPtr<DepthShader>();
	// 	context.m_shader = depthShader;
	// 	break;
	// }
	default:
		break;
	}
}

void GameObject::render(const glm::mat4& model, const glm::mat4& view, const glm::mat4& projection,
                        const RenderContext& context)
{
	assert(m_mesh != nullptr);

	assert(dynamic_cast<ObjectShader*>(context.m_shader) != nullptr);
	auto objectShader = static_cast<ObjectShader*>(context.m_shader);

	objectShader->use();
	objectShader->m_wboit = context.m_wboit;
	objectShader->m_wboitFunc = context.m_wboitFunc;
	objectShader->m_opacity = context.m_opaque ? 1.f : context.m_opacity;
	this->m_lastModelMatrix = model * m_modelMatrix * m_modMatrix;
	objectShader->setWorldTransform(m_lastModelMatrix, view, projection);
	objectShader->setUniforms();

	glBindVertexArray(m_mesh->m_vao);
	for (auto& meshPart : m_mesh->m_meshParts)
	{
		objectShader->setUniformsPerMeshPart(meshPart);
		if (context.m_instanceCount <= 0)
		{
			m_mesh->renderMeshPart(meshPart);
		}
		else
		{
			m_mesh->renderMeshPartInstanced(meshPart, context.m_instanceCount);
		}
	}
	glBindVertexArray(0);
}

void GameObject::update(Scene& scene)
{
	// Empty
}

void GameObject::dispose()
{
	// Empty
}
