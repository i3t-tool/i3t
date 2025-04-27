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

#include <set>

#include "pgr.h"

#include "Core/Defs.h"
#include "ManagedResource.h"

namespace Core
{
#define aPOS 0
#define aNORMAL 1
#define aTEXCOORD 2
#define aTANGENT 3
#define aCOLOR 4

/**
 * \brief Mesh represents a complete 3D model.
 * It hold relevant opengl state data and a list of mesh parts that actually
 * make up the model. Each mesh part also holds information about a material and
 * a set of textures. Geometry is immediately passed to OpenGL and not preserved
 * in memory. All geometry data is stored in a set of buffers that are shared
 * among the mesh parts of the mesh.
 *
 * Can be loaded using assimp with Mesh::load()
 * Or created directly from data using Mesh::create()
 *
 * Vertex attribute locations are hardcoded and are:
 * - location 0 = aPos (vec3)
 * - location 1 = aNormal (vec3)
 * - location 2 = aTexCoord (vec2)
 * - location 3 = aTangent (vec3)
 * - location 4 = aColor (vec3)
 *
 * Normals, TexCoords, Tangents and Colors can be individually disabled
 */
class Mesh : public ManagedResource
{
public:
	friend class ResourceManager;

	enum DrawType
	{
		ARRAYS,
		ELEMENTS,
	};

	enum PrimitiveType
	{
		LINES,
		TRIANGLES
	};

	/**
	 * \brief A set of textures for multi-mapping.
	 */
	struct TextureSet
	{
		GLuint texture = 0;
		GLuint specularMap = 0;
		GLuint normalMap = 0;
		GLuint aoMap = 0;
		GLuint emissionMap = 0;
		float normalStrength = 1.0f;
		float aoStrength = 1.0f;
	};

	/**
	 * \brief Data class holding basic material information.
	 */
	struct Material
	{
		glm::vec3 diffuse = glm::vec3(0.8f, 0.8f, 0.8f);
		glm::vec3 specular = glm::vec3(0.96f);
		glm::vec3 ambient = diffuse * 0.1f;
		float shininess = 20.0f;
	};

	/**
	 * Individual part of the bigger mesh containing separate geometry, material
	 * and textures.
	 */
	struct MeshPart
	{
		std::string name;

		Material material;
		TextureSet textureSet;

		glm::vec3 boundingBoxMin{0.0f};
		glm::vec3 boundingBoxMax{0.0f};

		/// number of indices in this submesh
		GLuint nIndices;
		/// first index in array of indices
		GLuint startIndex;
		/// vertex in array of vertices added to index in the index buffer
		GLuint baseVertex;
	};

	DrawType m_drawType;
	PrimitiveType m_primitiveType;

	/// Whether the normal vbo is used
	bool m_useNormals{false};
	/// Whether the texture coordinates vbo is used
	bool m_useTexcoords{false};
	/// Whether the tangent vbo is used
	bool m_useTangents{false};
	/// Whether the vertex colors vbo is used
	bool m_useColors{false};

	/// List of mesh parts making up this mesh
	std::vector<MeshPart> m_meshParts;

	/// Whether the mesh geometry has been normalized on load
	bool m_normalized{false};
	/// Mesh bounding box min, derived from bounding boxes of mesh parts.
	glm::vec3 m_boundingBoxMin{0.0f};
	/// Mesh bounding box max, derived from bounding boxes of mesh parts.
	glm::vec3 m_boundingBoxMax{0.0f};

	// Vertices
	GLuint m_vertex_vbo;
	GLuint m_normal_vbo;
	GLuint m_uv_vbo;
	GLuint m_tangent_vbo;
	GLuint m_color_vbo;
	/// count of vertices af all the mesh parts together
	unsigned int m_nVertices{0};

	// Indices
	GLuint m_ebo;
	/// count of indices af all the mesh parts together
	unsigned int m_nIndices{0};

	GLuint m_vao;

	// Filesystem
	std::string m_path;                      ///< If applicable, file path to the primary source file
	std::set<std::string> m_textureFileList; ///< List of texture file paths this mesh requires
	                                         ///< (not including embedded textures)

	int m_textureCount{0}; ///< Number of loaded textures for this mesh (files or embedded)

private:
	Mesh() = default;
	Mesh(Mesh::PrimitiveType primitiveType, Mesh::DrawType drawType, bool useNormals, bool useTexcoords,
	     bool useTangents, bool useColors);

public:
	/**
	 * \brief Renders the mesh and all its mesh parts.
	 * For more control over rendering you can iterate through individual
	 * meshParts and use renderMeshPart()
	 */
	void render() const;

	void renderInstanced(int instances) const;

	/**
	 * Renders an individual mesh part.
	 * <b>Note that prior to calling this method the mesh's VAO needs to be
	 * bound!</b>
	 *
	 * @param meshPart The mesh part to render.
	 */
	void renderMeshPart(const MeshPart& meshPart) const;

	void renderMeshPartInstanced(const MeshPart& meshPart, int instances) const;

	void dispose() const;

	/**
	 * Create mesh from vertices and colors.
	 * Only fills the color vertex buffer.
	 * @return Pointer to the new Mesh object (newly allocated, to be managed
	 * externally)
	 */
	static Mesh* create(Mesh::PrimitiveType primitiveType, const float* verts, const unsigned int nVertices,
	                    const float* colors, const unsigned int nColors);

	/**
	 * Create mesh from indexed vertices and colors;
	 * Only fills the color vertex buffer.
	 * @return Pointer to the new Mesh object (newly allocated, to be managed
	 * externally)
	 */
	static Mesh* create(Mesh::PrimitiveType primitiveType, const float* verts, const unsigned int nVertices,
	                    const unsigned int* indices, const unsigned int nIndices, const float* colors,
	                    const unsigned int nColors);

	/**
	 * Load mesh from a model file using assimp
	 * Fills normal, texcoord and tangent vertex buffers.
	 * @param path Path to the model file.
	 * @param minimalLoad If true, only the bare minimum of the model is loaded. Used to quickly aquire basic
	 * information from assimp without actually meaning to use the mesh.
	 * @return Pointer to the new Mesh object (newly allocated, to be managed
	 * externally)
	 */
	static Mesh* load(const std::string& path, bool normalize = false, bool minimalLoad = false);

private:
	static void loadGeometry(Mesh* mesh, const aiScene* scn, float scalingFactor);
	static void loadIndices(Mesh* mesh, const aiScene* scn);
	static void createVaoAndBindAttribs(Mesh* mesh);

	static void loadTextureCoordinates(Mesh* mesh, const aiScene* scn);
	static void loadTextures(Mesh* mesh, const aiScene* scn, const fs::path rootPath, bool minimalLoad);

	static void loadTextureSet(TextureSet& textureSet, const aiMaterial* material, const aiScene* scene, Mesh* mesh,
	                           const fs::path& rootPath, bool minimalLoad);
	static GLuint loadTexture(aiTextureType type, const aiMaterial* material, const aiScene* scene, Mesh* mesh,
	                          const fs::path& rootPath, bool minimalLoad);
	static GLuint loadEmbeddedTexture(const unsigned char* data, int length, bool mipmap = true);
	static void loadMaterial(Material& meshMaterial, const aiMaterial* material);

	static inline glm::vec3 convertVec3(const aiVector3D& v)
	{
		return glm::vec3(v.x, v.y, v.z);
	}
	static inline glm::vec3 vec3min(const glm::vec3& v1, const glm::vec3& v2)
	{
		return glm::vec3(std::min(v1.x, v2.x), std::min(v1.y, v2.y), std::min(v1.z, v2.z));
	}
	static inline glm::vec3 vec3max(const glm::vec3& v1, const glm::vec3& v2)
	{
		return glm::vec3(std::max(v1.x, v2.x), std::max(v1.y, v2.y), std::max(v1.z, v2.z));
	}
};
} // namespace Core
