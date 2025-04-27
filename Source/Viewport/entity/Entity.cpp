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
#include "Entity.h"

using namespace Vp;

Entity::Entity()
{
	m_backFaceCull = false;
	m_visible = true;
	m_opaque = true;
	m_wboit = false;
	m_wboitFunc = 0;
	m_explicitTransparencyOrder = 0;

	m_selectable = false;
	m_highlight = false;
	m_highlightUseDepth = true;
	m_highlightColor = glm::vec3(0.949, 0.682, 0.18);
}

void Entity::render(const glm::mat4& model, const glm::mat4& view, const glm::mat4& projection)
{
	RenderContext context;
	prepareRenderContext(context);
	render(model, view, projection, context);
}

Entity::RenderContext Entity::createRenderContext()
{
	RenderContext context;
	prepareRenderContext(context);
	return context;
}
