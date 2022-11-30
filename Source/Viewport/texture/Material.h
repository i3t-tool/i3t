#pragma once

#include "../shader/PhongShader.h"
#include "Core/Resources/Mesh.h"
#include "pgr.h"

namespace Vp
{
/**
 * \brief Material wrapper.
 */
class Material
{
public:
	Core::Mesh::Material m_material;
	glm::vec3 diffuse = glm::vec3(1.0, 1.0, 1.0);
	glm::vec3 specular = glm::vec3(1.0, 1.0, 1.0);
	glm::vec3 ambient = diffuse * 0.25f;
	float shininess = 16.0f;

	/**
	 * \brief Set material uniforms for a shader.
	 * \param shader The shader
	 */
	void setUniforms(PhongShader& shader) const;
};
} // namespace Vp