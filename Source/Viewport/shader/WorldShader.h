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

#include "Shader.h"

#include "Core/Resources/Mesh.h"

namespace Vp
{
/**
 * Abstract shader that provides PVM matrices uniforms.
 */
class WorldShader : public Shader
{
public:
	// World uniforms
	GLint pvmMatrixId;
	GLint projectionMatrixId;
	GLint modelMatrixId;
	GLint viewMatrixId;
	GLint normalMatrixId;

	glm::mat4 m_model;
	glm::mat4 m_view;
	glm::mat4 m_projection;

	GLint m_wboitNearId;
	GLint m_wboitFarId;

	// Flags
	bool m_pvm{true};         ///< Whether to provide precalculated PVM matrix uniform
	bool m_normal{true};      ///< Whether to provide normal matrix uniform
	bool m_pvmSeparate{true}; ///< Whether to provide individual pvm matrices

	explicit WorldShader(GLuint id);

	void init(bool initSuperclass) override;
	void setUniforms() override;

	/**
	 * \brief Updates MVP matrices.
	 * \param model Model use
	 * \param view View use
	 * \param projection Projection use
	 */
	void setWorldTransform(const glm::mat4& model, const glm::mat4& view, const glm::mat4& projection);
};
} // namespace Vp
