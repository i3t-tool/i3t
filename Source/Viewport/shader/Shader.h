#pragma once

#include "pgr.h"

#include "Core/Resources/Mesh.h"

namespace Vp
{
/**
 * Base abstract class for shaders.
 */
class Shader
{
public:
	GLuint id;

	explicit Shader(GLuint id);
	virtual ~Shader() = default;

	virtual void use();

	/**
	 * Set uniforms before rendering
	 */
	virtual void setUniforms() = 0;

	/**
	 * Set uniforms before rendering individual mesh parts
	 * @param meshPart MeshPart being rendered
	 */
	virtual void setUniformsPerMeshPart(Core::Mesh::MeshPart& meshPart) = 0;

	virtual void dispose();
};
} // namespace Vp