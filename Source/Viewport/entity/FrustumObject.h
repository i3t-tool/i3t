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
