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

#include <memory>
#include <unordered_map>

#include "Core/Defs.h"

#include "Core/Resources/ResourceManager.h"
#include "Viewport/shader/Shader.h"

namespace Vp
{
class ColorShader;
class PhongShader;
class GridShader;
class FrustumShader;
class WBOITCompositeShader;
class BoxBlurShader;
class SelectionCompositeShader;
class ScreenOverlayShader;

class Shaders
{
public:
	static Shaders& instance()
	{
		static Shaders instance; // Guaranteed to be destroyed.
		                         // Instantiated on first use.
		                         // https://stackoverflow.com/a/1008289/3452003
		return instance;
	}

private:
	Shaders() = default;

public:
	Shaders(Shaders const&) = delete;
	void operator=(Shaders const&) = delete;

	bool loaded{false};

	bool load();
	bool reload();

	std::shared_ptr<PhongShader> m_phongShader;
	std::shared_ptr<ColorShader> m_colorShader;
	std::shared_ptr<GridShader> m_gridShader;
	std::shared_ptr<FrustumShader> m_frustumShader;
	std::shared_ptr<WBOITCompositeShader> m_wboitCompositeShader;
	std::shared_ptr<BoxBlurShader> m_boxBlurShader;
	std::shared_ptr<SelectionCompositeShader> m_selectionCompositeShader;
	std::shared_ptr<ScreenOverlayShader> m_screenOverlayShader;

private:
	template <typename T>
	std::shared_ptr<T> loadShader(bool& success, const std::string& vertSource, const std::string& fragSource)
	{
		GLuint id = RMI.shader(vertSource, fragSource);
		if (id == 0)
		{
			success = false;
		}
		return std::make_shared<T>(id);
	}

	bool reloadShader(Shader& shader, const std::string& vertSource, const std::string& fragSource);
	bool checkForError(Shader& shader);
};

} // namespace Vp
