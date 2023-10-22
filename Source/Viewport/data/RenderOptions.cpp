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
#include "RenderOptions.h"
#include "Viewport/shader/PhongShader.h"

using namespace Vp;

RenderOptions::RenderOptions()
{
	multisample = false;
	samples = 4;
	framebufferAlpha = false;
	wboit = false;
	wboitFunc = 0;
	selection = false;
	lightingModel = PhongShader::LightingModel::BLINN_PHONG;

	clearColor = glm::vec3(0.2f, 0.2f, 0.2f);
}
