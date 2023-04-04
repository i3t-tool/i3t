#pragma once

#include "pgr.h"

#include "Core/Resources/Mesh.h"

namespace Vp
{
/**
 * Base class for shaders.
 *
 * Note that shader objects are just a facade/API for the glsl shaders.
 * They shouldn't store state across frames and should have ALL their variables and uniforms set before each use.
 * However sometimes it makes sense to store values in them. Just be aware that shaders are shared between many objects.
 */
class Shader
{
public:
	GLuint m_id;

	/**
	 * Whether to use weighted blended transparency for output if supported
	 * \see supportsWboit()
	 */
	bool m_wboit = false;
	int m_wboitFunc = 0;

protected:
	GLint m_wboitFlagId{-1}; ///< Uniform id of the wboit enable/disable flag
	GLint m_wboitFuncId{-1};

public:
	explicit Shader(GLuint id);
	virtual ~Shader() = default;

	virtual void init(bool initSuperclass);

	virtual void use() const;

	/**
	 * Set uniforms before rendering
	 */
	virtual void setUniforms();

	/**
	 * Set uniforms before rendering individual mesh parts
	 * @param meshPart MeshPart being rendered
	 */
	virtual void setUniformsPerMeshPart(Core::Mesh::MeshPart& meshPart);

	void bindTexture2D(GLuint textureUnit, GLuint textureID, GLint samplerLocation);
	void bindTexture2DMS(GLuint textureUnit, GLuint textureID, GLint samplerLocation);

	bool hasUniform(GLint location);

	/**
	 * @return Whether this shader supports output to wboit buffers
	 */
	bool supportsWboit();
};
} // namespace Vp