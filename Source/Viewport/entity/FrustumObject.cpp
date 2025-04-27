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
#include "FrustumObject.h"

#include "Viewport/shader/FrustumShader.h"

using namespace Vp;

FrustumObject::FrustumObject(Core::Mesh* mesh, FrustumShader* shader) : ColoredObject(mesh, shader)
{
	setDisplayType(DisplayType::Frustum);
}

void FrustumObject::prepareRenderContext(RenderContext& context)
{
	Super::prepareRenderContext(context);

	assert(dynamic_cast<FrustumShader*>(context.m_shader) != nullptr);
	FrustumShader* frustumShader = static_cast<FrustumShader*>(context.m_shader);
	frustumShader->m_frustumProjectionMatrix = m_frustumProjectionMatrix;
	frustumShader->m_frustumViewMatrix = m_frustumViewMatrix;
}

void FrustumObject::render(const glm::mat4& model, const glm::mat4& view, const glm::mat4& projection,
                           const RenderContext& context)
{
	this->m_modelMatrix = glm::identity<glm::mat4>();
	ColoredObject::render(model, view, projection, context);
	// Set model matrix to later retrieve position for transparency sorting
	this->m_modelMatrix = model * m_frustumViewMatrixInv;
}
