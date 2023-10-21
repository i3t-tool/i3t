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
	m_phongShader =	loadShader<PhongShader>(ok,
		"Data/Shaders/phongVert.glsl", "Data/Shaders/phongFrag.glsl"
	);
	m_colorShader =	loadShader<ColorShader>(ok,
		"Data/Shaders/colorVert.glsl", "Data/Shaders/colorFrag.glsl"
	);
	m_gridShader = loadShader<GridShader>(ok,
		"Data/Shaders/gridVert.glsl", "Data/Shaders/gridFrag.glsl"
	);
	m_frustumShader =	loadShader<FrustumShader>(ok,
		"Data/Shaders/frustumVert.glsl", "Data/Shaders/colorFrag.glsl"
	);
	m_wboitCompositeShader = loadShader<WBOITCompositeShader>(ok,
		"Data/Shaders/basicVert.glsl", "Data/Shaders/wboitCompositeFrag.glsl"
	);
	m_boxBlurShader =	loadShader<BoxBlurShader>(ok,
		"Data/Shaders/basicVert.glsl", "Data/Shaders/boxBlurFrag.glsl"
	);
	m_selectionCompositeShader = loadShader<SelectionCompositeShader>(ok,
		"Data/Shaders/basicVert.glsl", "Data/Shaders/selectionCompositeFrag.glsl"
	);
	m_screenOverlayShader =	loadShader<ScreenOverlayShader>(ok,
		"Data/Shaders/basicVert.glsl", "Data/Shaders/screenOverlayFrag.glsl"
	);
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

	RMI.m_forceReload = true;

	bool ok = true;
	ok &= reloadShader(*m_phongShader, "Data/Shaders/phongVert.glsl", "Data/Shaders/phongFrag.glsl");
	ok &= reloadShader(*m_colorShader, "Data/Shaders/colorVert.glsl", "Data/Shaders/colorFrag.glsl");
	ok &= reloadShader(*m_gridShader, "Data/Shaders/gridVert.glsl", "Data/Shaders/gridFrag.glsl");
	ok &= reloadShader(*m_frustumShader, "Data/Shaders/frustumVert.glsl", "Data/Shaders/colorFrag.glsl");
	ok &= reloadShader(*m_wboitCompositeShader, "Data/Shaders/basicVert.glsl", "Data/Shaders/wboitCompositeFrag.glsl");
	ok &= reloadShader(*m_boxBlurShader, "Data/Shaders/basicVert.glsl", "Data/Shaders/boxBlurFrag.glsl");
	ok &= reloadShader(*m_selectionCompositeShader, "Data/Shaders/basicVert.glsl",
	                   "Data/Shaders/selectionCompositeFrag.glsl");
	ok &= reloadShader(*m_screenOverlayShader, "Data/Shaders/basicVert.glsl", "Data/Shaders/screenOverlayFrag.glsl");

	RMI.m_forceReload = false;

	return ok;
}

bool Shaders::reloadShader(Shader& shader, const std::string& vertSource, const std::string& fragSource)
{
	GLuint id = RMI.shader(vertSource, fragSource);
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
