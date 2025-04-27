/**
 * \file
 * \brief
 * \author Dan Rakušan <rakusan.dan@gmail.com>
 * \copyright Copyright (C) 2016-2025 I3T team, Department of Computer Graphics
 * and Interaction, FEE, Czech Technical University in Prague, Czech Republic
 *
 * This file is part of I3T - An Interactive Tool for Teaching Transformations
 * http://www.i3t-tool.org
 *
 * GNU General Public License v3.0 (see LICENSE.txt or https://www.gnu.org/licenses/gpl-3.0.txt)
 */
#include "GridObject.h"

#include "Core/Resources/ResourceManager.h"
#include "Viewport/Shaper.h"
#include "Viewport/data/ViewportSettings.h"
#include "Viewport/shader/GridShader.h"
#include "Viewport/shader/Shaders.h"

using namespace Vp;

GridObject::GridObject() : GameObject(RMI.meshByAlias(Shaper::screenQuad), SHADERS.getShaderPtr<GridShader>())
{
	setDisplayType(DisplayType::Grid);
}
void GridObject::setReferenceSpace(const glm::mat4& referenceSpace, const glm::mat4& referenceSpaceInv)
{
	this->m_modelMatrix = referenceSpaceInv;
	this->m_modelMatrixInv = referenceSpace;
}

void GridObject::updateGridShader()
{
	isStandard = this->m_modelMatrix == glm::identity<glm::mat4>();
	m_shader = isStandard ? SHADERS.getShaderPtr<GridShader>() : SHADERS.getShaderPtr<GridShaderGeneric>();
}

void GridObject::prepareRenderContext(RenderContext& context)
{
	Super::prepareRenderContext(context);

	GridShader* gridShader = static_cast<GridShader*>(context.m_shader);
	gridShader->m_modelInv = m_modelMatrixInv;

	// TODO: The way we're passing stuff like color data from preferences -> GridObject -> GridShader
	//  every frame is very clumsy. Find a better way, uniform data should probably be not stored
	//  in the shader itself and some other object should be responsible for setting uniforms.
	//  Shaders themselves should stay as essentially global objects as they represent global programs that
	//  might be recompiled later. However multiple instances of uniform data can exist and switching
	//  between them should be easy and avoid unnecessary copying.

	gridShader->m_showGrid = m_showGrid;
	gridShader->m_showXAxis = m_showXAxis;
	gridShader->m_showYAxis = m_showYAxis;
	gridShader->m_showZAxis = m_showZAxis;

	gridShader->m_gridColor = m_gridColor;
	gridShader->m_axisXColor = m_axisXColor;
	gridShader->m_axisYColor = m_axisYColor;
	gridShader->m_axisZColor = m_axisZColor;

	gridShader->m_gridSize = m_gridSize;
	gridShader->m_gridStrength = m_gridStrength;

	gridShader->m_lineWidth = m_lineWidth;

	gridShader->m_grid1FadeStart = m_grid1FadeStart;
	gridShader->m_grid1FadeEnd = m_grid1FadeEnd;
	gridShader->m_grid2FadeStart = m_grid2FadeStart;
	gridShader->m_grid2FadeEnd = m_grid2FadeEnd;
}

void GridObject::render(const glm::mat4& model, const glm::mat4& view, const glm::mat4& projection,
                        const RenderContext& context)
{
	updateGridShader();
	GameObject::render(glm::identity<glm::mat4>(), view, projection, context);
}
