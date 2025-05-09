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
	using Super = ColoredObject;

public:
	glm::mat4 m_frustumProjectionViewMatrixInv{1.f};
	glm::mat4 m_frustumViewMatrixInv{1.f};
	bool m_visualizeDepth{false};

	FrustumObject(Core::Mesh* mesh, FrustumShader* shader);

	void prepareRenderContext(RenderContext& context, const DisplayOptions& displayOptions) override;
	void render(const glm::mat4& model, const glm::mat4& view, const glm::mat4& projection,
	            const RenderContext& context) override;
};
} // namespace Vp
