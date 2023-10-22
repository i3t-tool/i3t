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

#include "ObjectShader.h"

#include "Core/Resources/Mesh.h"

namespace Vp
{
/**
 * Shader for lit objects with textures.
 */
class PhongShader : public ObjectShader
{
public:
	enum LightingModel
	{
		PHONG,
		BLINN_PHONG
	};

protected:
	// Material uniforms
	GLint material_diffuse;
	GLint material_specular;
	GLint material_ambient;
	GLint material_shininess;

	// Texture uniforms
	GLint normalStrength;

	GLint tintId;

	GLint m_lightingModelId;

public:
	glm::vec3 m_tint{1.0f};
	LightingModel m_lightingModel{BLINN_PHONG};

	explicit PhongShader(GLuint id);

	void init(bool initSuperclass) override;

	void setUniforms() override;
	void setUniformsPerMeshPart(Core::Mesh::MeshPart& meshPart) override;

protected:
	void setMaterialUniforms(Core::Mesh::Material material);
	void bindTextures(Core::Mesh::TextureSet tSet);

	/**
	 * \brief Sets all texture active flags to false. Used to reset texture state
	 * before binding new set of textures.
	 */
	void clearTextures() const;
	void bindTexture(GLuint textureID, const std::string& type, int typeIndex, GLuint textureUnit);
};
} // namespace Vp