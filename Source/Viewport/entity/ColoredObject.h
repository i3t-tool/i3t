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

#include "GameObject.h"

namespace Vp
{
class ColorShader;

class ColoredObject : public GameObject
{
public:
	bool m_useSingleColor{false};
	glm::vec3 m_singleColor{1.0f, 0.0f, 1.0f};

	ColoredObject(Core::Mesh* mesh, ColorShader* shader);

	void prepareRenderContext(RenderContext& context) override;

	const glm::vec3& getColor() const;
	void setColor(const glm::vec3* color);
	void setColor(const glm::vec3& color);
};
} // namespace Vp
