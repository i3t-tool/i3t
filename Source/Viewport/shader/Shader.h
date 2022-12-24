#pragma once

#include "pgr.h"

#include "Core/Resources/Mesh.h"

namespace Vp
{
/**
 * Base class for shaders.
 *
 * Note that shader objects are just a facade/API for the glsl shaders.
 * They are NOT meant to store state across frames and should have ALL their variables and uniforms set before each use.
 */
class Shader
{
public:
	GLuint id;

	GLint wboitFlagUniform{-1}; ///< Uniform id of the wboit enable/disable flag

	explicit Shader(GLuint id);
	virtual ~Shader() = default;

	virtual void use() const;

	/**
	 * Set uniforms before rendering
	 */
	virtual void setUniforms(){};

	/**
	 * Set uniforms before rendering individual mesh parts
	 * @param meshPart MeshPart being rendered
	 */
	virtual void setUniformsPerMeshPart(Core::Mesh::MeshPart& meshPart){};

	virtual void bindTexture2D(GLuint textureUnit, GLuint textureID, GLint samplerLocation);

	virtual bool hasUniform(GLint location);
};
} // namespace Vp