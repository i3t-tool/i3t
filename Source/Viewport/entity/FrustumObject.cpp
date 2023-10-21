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

void FrustumObject::update(Scene& scene)
{
	ColoredObject::update(scene);
	// Set model matrix to later retrieve position for transparency sorting
	this->m_modelMatrix = glm::inverse(m_frustumViewMatrix);
}

void FrustumObject::render(Shader* shader, glm::mat4 view, glm::mat4 projection, bool silhouette)
{
	FrustumShader* frustumShader = static_cast<FrustumShader*>(shader);
	frustumShader->m_frustumProjectionMatrix = m_frustumProjectionMatrix;
	frustumShader->m_frustumViewMatrix = m_frustumViewMatrix;
	ColoredObject::render(shader, view, projection, silhouette);
}
