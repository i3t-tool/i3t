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
#include "Shaders.h"

using namespace Vp;

#include "Viewport/shader/BoxBlurShader.h"
#include "Viewport/shader/ColorShader.h"
#include "Viewport/shader/FrustumShader.h"
#include "Viewport/shader/GridShader.h"
#include "Viewport/shader/PhongShader.h"
#include "Viewport/shader/ScreenOverlayShader.h"
#include "Viewport/shader/SelectionCompositeShader.h"
#include "Viewport/shader/WBOITCompositeShader.h"

#include "Logger/Logger.h"

bool Shaders::load()
{
	if (loaded)
		return true;

	bool ok = true;

	// clang-format off
	ok &= createShader<PhongShader>("Data/Shaders/phongVert.glsl", "Data/Shaders/phongFrag.glsl");
	ok &= createShader<ColorShader>("Data/Shaders/colorVert.glsl", "Data/Shaders/colorFrag.glsl");
	// ok &= createShader<DepthShader>("Data/Shaders/depthVert.glsl", "");
	ok &= createShader<GridShader>("Data/Shaders/gridVert.glsl", "Data/Shaders/gridFrag.glsl");
	ok &= createShader<GridShaderGeneric>("Data/Shaders/gridVert.glsl", "Data/Shaders/gridFrag.glsl", "", "#define GENERIC_GRID");
	ok &= createShader<FrustumShader>("Data/Shaders/frustumVert.glsl", "Data/Shaders/frustumFrag.glsl");
	ok &= createShader<WBOITCompositeShader>("Data/Shaders/basicVert.glsl", "Data/Shaders/wboitCompositeFrag.glsl");
	ok &= createShader<BoxBlurShader>("Data/Shaders/basicVert.glsl", "Data/Shaders/boxBlurFrag.glsl");
	ok &= createShader<SelectionCompositeShader>("Data/Shaders/basicVert.glsl", "Data/Shaders/selectionCompositeFrag.glsl");
	ok &= createShader<ScreenOverlayShader>("Data/Shaders/basicVert.glsl", "Data/Shaders/screenOverlayFrag.glsl");
	// clang-format on

	loaded = true;
	return ok;
}

bool Shaders::reload()
{
	if (!loaded)
	{
		load();
		return true;
	}

	bool ok = true;
	for (const auto& [key, value] : m_shaders)
	{
		ok &=
		    reloadShader(*value, value->m_vertSource, value->m_fragSource, value->m_geoSource, value->m_injectedSource);
	}

	return ok;
}

bool Shaders::reloadShader(Shader& shader, const std::string& vertSource, const std::string& fragSource)
{
	return reloadShader(shader, vertSource, fragSource, "", "");
}

bool Shaders::reloadShader(Shader& shader, const std::string& vertSource, const std::string& fragSource,
                           const std::string& geoSource, const std::string& sourceToInject)
{
	RMI.m_forceReload = true;
	GLuint id = RMI.shaderGI(vertSource, fragSource, geoSource, sourceToInject);
	RMI.m_forceReload = false;
	if (id != 0)
	{
		shader.m_id = id;
		shader.init(true);
		return true;
	}
	else
	{
		return false;
	}
}

bool Shaders::checkForError(Shader& shader)
{
	return shader.m_id == 0;
}
