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

	explicit WorldShader(GLuint id);

	void setUniforms() override;
	void setUniformsPerMeshPart(Core::Mesh::MeshPart& meshPart) override;

	/**
	 * \brief Updates MVP matrices.
	 * \param model Model use
	 * \param view View use
	 * \param projection Projection use
	 */
	void setWorldTransform(const glm::mat4& model, const glm::mat4& view,
	                       const glm::mat4& projection);
};
} // namespace Vp
