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
#include <vector>

namespace Vp
{
class Light;
class PhongShader;

/**
 * \brief Class that manages passing of light information to shaders.
 */
class Lighting
{
	std::vector<std::shared_ptr<Light>> lights;

public:
	/**
	 * \brief Set light information uniforms for a shader.
	 * \param shader The shader
	 */
	void setUniforms(const PhongShader& shader) const;

	void addLight(Light* light);
	void addLight(const std::shared_ptr<Light>& light);

	void removeLight(std::shared_ptr<Light>& light);

	std::vector<std::shared_ptr<Light>> getLights() const;
};

} // namespace Vp
