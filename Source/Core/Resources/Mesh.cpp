#include "Mesh.h"

using namespace Core;

#include "stb_image.h"
#include <assimp/GltfMaterial.h>

#include "pgr.h"

#include "Logger/Logger.h"
#include "ResourceManager.h"

const unsigned FACE_VERT_COUNT = 3; // triangles

Mesh::Mesh(Mesh::PrimitiveType primitiveType, Mesh::DrawType drawType, bool useNormals, bool useTexcoords,
           bool useTangents, bool useColors)
    : m_primitiveType(primitiveType), m_drawType(drawType), m_useNormals(useNormals), m_useTexcoords(useTexcoords),
      m_useTangents(useTangents), m_useColors(useColors)
{
	// Empty
}

void Mesh::dispose() const
{
	glDeleteBuffers(1, &m_vertex_vbo);
	if (m_useNormals)
		glDeleteBuffers(1, &m_normal_vbo);
	if (m_useTexcoords)
		glDeleteBuffers(1, &m_uv_vbo);
	if (m_useTangents)
		glDeleteBuffers(1, &m_tangent_vbo);
	if (m_useColors)
		glDeleteBuffers(1, &m_tangent_vbo);
	if (m_drawType == ELEMENTS)
	{
		glDeleteBuffers(1, &m_ebo);
	}
}

Mesh* Mesh::create(Mesh::PrimitiveType primitiveType, const float* verts, const unsigned int nVertices,
                   const float* colors, const unsigned int nColors)
{
	Mesh* mesh = new Mesh(primitiveType, ARRAYS, false, false, false, true);

	mesh->m_useNormals = false;
	mesh->m_useTexcoords = false;
	mesh->m_useTangents = false;
	mesh->m_useColors = true;

	mesh->m_nVertices = nVertices;

	// vertices
	glGenBuffers(1, &mesh->m_vertex_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, mesh->m_vertex_vbo);
	glBufferData(GL_ARRAY_BUFFER, nVertices * 3 * sizeof(float), verts, GL_STATIC_DRAW);

	// colors
	glGenBuffers(1, &mesh->m_color_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, mesh->m_color_vbo);
	glBufferData(GL_ARRAY_BUFFER, nColors * 3 * sizeof(float), colors, GL_STATIC_DRAW);

	createVaoAndBindAttribs(mesh);

	MeshPart meshPart;
	meshPart.startIndex = 0;
	meshPart.baseVertex = 0;
	meshPart.nIndices = nVertices;
	mesh->m_meshParts.push_back(meshPart);

	return mesh;
}

Mesh* Mesh::create(Mesh::PrimitiveType primitiveType, const float* verts, const unsigned int nVertices,
                   const unsigned int* indices, const unsigned int nIndices, const float* colors,
                   const unsigned int nColors)
{
	// TODO: (DR) Test
	Mesh* mesh = new Mesh(primitiveType, ARRAYS, false, false, false, true);

	mesh->m_useNormals = false;
	mesh->m_useTexcoords = false;
	mesh->m_useTangents = false;
	mesh->m_useColors = true;

	mesh->m_nVertices = nVertices;
	mesh->m_nIndices = nIndices;

	// vertices
	glGenBuffers(1, &mesh->m_vertex_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, mesh->m_vertex_vbo);
	glBufferData(GL_ARRAY_BUFFER, nVertices * 3 * sizeof(float), verts, GL_STATIC_DRAW);

	// colors
	glGenBuffers(1, &mesh->m_color_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, mesh->m_color_vbo);
	glBufferData(GL_ARRAY_BUFFER, nColors * 3 * sizeof(float), colors, GL_STATIC_DRAW);

	// EBO
	glGenBuffers(1, &mesh->m_ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->m_ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, nIndices * sizeof(unsigned int), indices, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	createVaoAndBindAttribs(mesh);

	MeshPart meshPart;
	meshPart.nIndices = nIndices;
	meshPart.startIndex = 0;
	meshPart.baseVertex = 0;
	meshPart.nIndices = nVertices;
	mesh->m_meshParts.push_back(meshPart);

	return mesh;
}

void Mesh::render() const
{
	glBindVertexArray(m_vao);
	for (auto& meshPart : m_meshParts)
	{
		renderMeshPart(meshPart);
	}
	glBindVertexArray(0);
}

void Mesh::renderMeshPart(const MeshPart& meshPart) const
{
	switch (m_drawType)
	{
	case ARRAYS:
		switch (m_primitiveType)
		{
		case LINES:
			glDrawArrays(GL_LINES, meshPart.startIndex, meshPart.nIndices);
			break;
		case TRIANGLES:
			glDrawArrays(GL_TRIANGLES, meshPart.startIndex, meshPart.nIndices);
			break;
		default:
			throw std::invalid_argument("Mesh: Invalid mesh primitive type!");
		}
		break;
	case ELEMENTS:
		switch (m_primitiveType)
		{
		case LINES:
			glDrawElementsBaseVertex(GL_LINES, meshPart.nIndices, GL_UNSIGNED_INT,
			                         (void*) (meshPart.startIndex * sizeof(unsigned int)), meshPart.baseVertex);
			break;
		case TRIANGLES:
			glDrawElementsBaseVertex(GL_TRIANGLES, meshPart.nIndices, GL_UNSIGNED_INT,
			                         (void*) (meshPart.startIndex * sizeof(unsigned int)), meshPart.baseVertex);
			break;
		default:
			throw std::invalid_argument("Mesh: Invalid mesh primitive type!");
		}
		break;
	default:
		throw std::invalid_argument("Mesh: Invalid mesh draw type!");
	}
}

Mesh* Mesh::load(const std::string& path, bool normalize, bool minimalLoad)
{
	Assimp::Importer importer;
	Mesh* mesh = new Mesh(TRIANGLES, ELEMENTS, true, true, true, false);
	mesh->m_normalized = normalize;

	fs::path modelRootPath = fs::path(path).parent_path();

	// Unitize object in size
	//	importer.SetPropertyInteger(AI_CONFIG_PP_PTV_NORMALIZE,
	//	                            1); // used by aiProcess_PreTransformVertices to
	//	                                // normalize vertices to fit into (-1..1)^3

	// Load asset from the file using assimp
	unsigned int pFlags = 0;
	if (!minimalLoad)
	{
		pFlags =
		    pFlags
		    //| aiProcess_SortByPType // Sort by primitive type into groups. Remove unrequested (as and polygons).
		    | aiProcess_Triangulate          // Triangulate polygons (if any).
		    | aiProcess_PreTransformVertices // Transforms scene hierarchy into one root with geometry-leafs only.
		    //                               // For more see Doc.
		    | aiProcess_GenSmoothNormals // Calculate normals per vertex.
		    | aiProcess_JoinIdenticalVertices
		    //| aiProcess_RemoveComponent // Removes above defined components
		    //| aiProcess_PreTransformVertices // Transforms scene hierarchy into one root with geometry-leafs only.
		    //     							   // For more see Doc.
		    //| aiProcess_ImproveCacheLocality
		    //| aiProcess_RemoveRedundantMaterials
		    //| aiProcess_FindDegenerates // Find degenerated polys and convert them into lines/points,
		    //                            // which are then removed.
		    //| aiProcess_FindInvalidData // In this step we can lost normals if they are incorrectly exported
		    //   						  // from the modeler. Thats why we generate them.
		    //| aiProcess_GenUVCoords // Force regenerate spherical/cylindrical/planar... coords into UVs.
		    | aiProcess_TransformUVCoords // Pretransform UV coords with texture matrix.
		    //                            // Like in vertex shaders using gl_TextureMatrix.
		    //| aiProcess_OptimizeMeshes
		    | aiProcess_CalcTangentSpace // Calculate tangents/bitangents per vertex.
		    | aiProcess_GenBoundingBoxes;
	}
	const aiScene* scn = importer.ReadFile(path.c_str(), pFlags);

	if (!scn)
	{
		LOG_ERROR("Mesh: Failed to load the scene '{}': {}", path, importer.GetErrorString());
		return nullptr;
	}

	if (scn->mNumMeshes < 1)
	{
		LOG_ERROR("Mesh: No meshes found in the scene '{}'", path);
		return nullptr;
	}

	// merge all sub-meshes to one big mesh

	unsigned int nVertices = 0; // vertex counter
	unsigned int nIndices = 0;  // indices counter

	// sum all vertices and indices for all meshes
	for (unsigned m = 0; m < scn->mNumMeshes; ++m)
	{
		// count the overall
		nVertices += scn->mMeshes[m]->mNumVertices;               //   number of vertices
		nIndices += scn->mMeshes[m]->mNumFaces * FACE_VERT_COUNT; //   and indices in all THE meshes
	}
	mesh->m_nIndices = nIndices;
	mesh->m_nVertices = nVertices;

	if ((nVertices == 0) || (nIndices < FACE_VERT_COUNT))
	{
		LOG_INFO("Mesh: No triangles found in the scene '{}'", path);
		return nullptr;
	}

	// create mesh parts for each mesh in the aiScene
	mesh->m_meshParts.resize(scn->mNumMeshes);

	if (!minimalLoad)
	{
		// Bounding box and misc
		for (unsigned m = 0; m < scn->mNumMeshes; ++m)
		{
			const aiMesh* aiMesh = scn->mMeshes[m];
			Mesh::MeshPart& meshPart = mesh->m_meshParts[m];

			meshPart.name = std::string(aiMesh->mName.C_Str());

			meshPart.boundingBoxMin = convertVec3(aiMesh->mAABB.mMin);
			meshPart.boundingBoxMax = convertVec3(aiMesh->mAABB.mMax);
			mesh->m_boundingBoxMin = vec3min(mesh->m_boundingBoxMin, meshPart.boundingBoxMin);
			mesh->m_boundingBoxMax = vec3max(mesh->m_boundingBoxMax, meshPart.boundingBoxMax);
		}

		// Normalization
		float normalizationFactor = 1.0f;
		if (normalize)
		{
			glm::vec3 min = mesh->m_boundingBoxMin;
			glm::vec3 max = mesh->m_boundingBoxMax;
			float largestBoundingBoxSideLength =
			    std::max(std::max(abs(min.x - max.x), abs(min.y - max.y)), abs(min.z - max.z));
			normalizationFactor = 2.0f / largestBoundingBoxSideLength;

			mesh->m_boundingBoxMin *= normalizationFactor;
			mesh->m_boundingBoxMax *= normalizationFactor;
		}

		// Load stuff
		loadGeometry(mesh, scn, normalizationFactor);
		loadIndices(mesh, scn);
		loadTextureCoordinates(mesh, scn);
		createVaoAndBindAttribs(mesh);
	}

	loadTextures(mesh, scn, modelRootPath, minimalLoad);

	return mesh;
}

void Mesh::loadGeometry(Mesh* mesh, const aiScene* scn, float scalingFactor)
{
	// vertices
	glGenBuffers(1, &mesh->m_vertex_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, mesh->m_vertex_vbo);
	glBufferData(GL_ARRAY_BUFFER, mesh->m_nVertices * 3 * sizeof(float), 0, GL_STATIC_DRAW); // xyz
	for (unsigned m = 0, offset = 0; m < scn->mNumMeshes; ++m)
	{
		unsigned size = scn->mMeshes[m]->mNumVertices * 3 * sizeof(float);
		if (scalingFactor != 1.0f)
		{
			for (int i = 0; i < scn->mMeshes[m]->mNumVertices; i++)
			{
				aiVector3D* vec = &scn->mMeshes[m]->mVertices[i];
				*vec *= scalingFactor;
			}
		}
		glBufferSubData(GL_ARRAY_BUFFER, offset, size, scn->mMeshes[m]->mVertices);
		offset += size;
	}

	// normal vectors
	glGenBuffers(1, &mesh->m_normal_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, mesh->m_normal_vbo);
	glBufferData(GL_ARRAY_BUFFER, mesh->m_nVertices * 3 * sizeof(float), 0, GL_STATIC_DRAW); // xyz
	for (unsigned m = 0, offset = 0; m < scn->mNumMeshes; ++m)
	{
		unsigned size = scn->mMeshes[m]->mNumVertices * 3 * sizeof(float);
		glBufferSubData(GL_ARRAY_BUFFER, offset, size, scn->mMeshes[m]->mNormals);
		offset += size;
	}

	// tangent vectors
	glGenBuffers(1, &mesh->m_tangent_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, mesh->m_tangent_vbo);
	glBufferData(GL_ARRAY_BUFFER, mesh->m_nVertices * 3 * sizeof(float), 0, GL_STATIC_DRAW); // xyz
	for (unsigned m = 0, offset = 0; m < scn->mNumMeshes; ++m)
	{
		unsigned size = scn->mMeshes[m]->mNumVertices * 3 * sizeof(float);
		glBufferSubData(GL_ARRAY_BUFFER, offset, size, scn->mMeshes[m]->mTangents);
		offset += size;
	}
}

void Mesh::loadIndices(Mesh* mesh, const aiScene* scn)
{
	unsigned* indices = new unsigned[mesh->m_nIndices]; // indices to the vertices of the faces

	unsigned startIndex = 0; // for face indexing - index in the array of indexes
	unsigned baseVertex = 0; // for vertices block (base vertex is added to rellative index in the
	                         // submesh to get absolute index in the array of vertices)

	for (unsigned m = 0; m < scn->mNumMeshes; ++m)
	{
		const aiMesh* aiMesh = scn->mMeshes[m];
		Mesh::MeshPart& meshPart = mesh->m_meshParts[m];

		// copy the face index triplets (we use triangular faces) to indices
		for (unsigned j = 0; j < aiMesh->mNumFaces; ++j)
		{
			memcpy(&indices[startIndex + j * FACE_VERT_COUNT], aiMesh->mFaces[j].mIndices,
			       FACE_VERT_COUNT * sizeof(unsigned));
		}

		// - indices to the element array
		meshPart.nIndices = aiMesh->mNumFaces * FACE_VERT_COUNT;
		meshPart.startIndex = startIndex;
		meshPart.baseVertex = baseVertex;

		startIndex += aiMesh->mNumFaces * FACE_VERT_COUNT;
		baseVertex += aiMesh->mNumVertices;
	}

	glGenBuffers(1, &mesh->m_ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->m_ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, mesh->m_nIndices * sizeof(unsigned int), indices, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	delete[] indices;
}

void Mesh::createVaoAndBindAttribs(Mesh* mesh)
{
	// Bind VAO and vertex attrib pointers
	// Vertex attrib locations are hardcoded and defined in the docs / defines

	glGenVertexArrays(1, &mesh->m_vao);
	glBindVertexArray(mesh->m_vao);

	glBindBuffer(GL_ARRAY_BUFFER, mesh->m_vertex_vbo);
	glEnableVertexAttribArray(aPOS);
	glVertexAttribPointer(aPOS, 3, GL_FLOAT, GL_FALSE, 0, 0);

	if (mesh->m_useNormals)
	{
		glBindBuffer(GL_ARRAY_BUFFER, mesh->m_normal_vbo);
		glEnableVertexAttribArray(aNORMAL);
		glVertexAttribPointer(aNORMAL, 3, GL_FLOAT, GL_FALSE, 0, 0);
	}

	if (mesh->m_useTexcoords)
	{
		glBindBuffer(GL_ARRAY_BUFFER, mesh->m_uv_vbo);
		glEnableVertexAttribArray(aTEXCOORD);
		glVertexAttribPointer(aTEXCOORD, 2, GL_FLOAT, GL_FALSE, 0, 0); //(str)
	}

	if (mesh->m_useTangents)
	{
		glBindBuffer(GL_ARRAY_BUFFER, mesh->m_tangent_vbo);
		glEnableVertexAttribArray(aTANGENT);
		glVertexAttribPointer(aTANGENT, 3, GL_FLOAT, GL_FALSE, 0, 0);
	}

	if (mesh->m_useColors)
	{
		glBindBuffer(GL_ARRAY_BUFFER, mesh->m_color_vbo);
		glEnableVertexAttribArray(aCOLOR);
		glVertexAttribPointer(aCOLOR, 3, GL_FLOAT, GL_FALSE, 0, 0);
	}

	if (mesh->m_drawType == ELEMENTS)
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->m_ebo);
	}

	glBindVertexArray(0);
}

void Mesh::loadTextureCoordinates(Mesh* mesh, const aiScene* scn)
{
	// TODO: just texture 0 for now
	float* textureCoords = new float[2 * mesh->m_nVertices]; // 2 floats per vertex (str)
	float* cur_textureCoord = textureCoords;

	for (unsigned m = 0; m < scn->mNumMeshes; ++m)
	{
		const aiMesh* aiMesh = scn->mMeshes[m];
		Mesh::MeshPart& meshPart = mesh->m_meshParts[m];

		// copy texture coordinates
		aiVector3D vect;
		for (unsigned i = 0; i < 1; ++i)
		{ // TODO> up to 4 textures can be loaded
			if (aiMesh->HasTextureCoords(i))
			{
				// we use 2D textures with 2 coordinates and ignore the third coordinate
				for (unsigned idx = 0; idx < aiMesh->mNumVertices; idx++)
				{
					// Y coordinate needs to be flipped, not too sure why, something
					// something different uv coords origin maybe?
					vect = (aiMesh->mTextureCoords[i])[idx];
					*cur_textureCoord++ = vect.x;
					*cur_textureCoord++ = 1.0f - vect.y;
				}
			}
		}
	}

	glGenBuffers(1, &mesh->m_uv_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, mesh->m_uv_vbo);
	glBufferData(GL_ARRAY_BUFFER, mesh->m_nVertices * 2 * sizeof(float), textureCoords, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	delete[] textureCoords;
}

void Mesh::loadTextures(Mesh* mesh, const aiScene* scn, const fs::path rootPath, bool minimalLoad)
{
	for (unsigned m = 0; m < scn->mNumMeshes; ++m)
	{
		const aiMesh* aiMesh = scn->mMeshes[m];
		Mesh::MeshPart& meshPart = mesh->m_meshParts[m];

		const aiMaterial* mat = scn->mMaterials[aiMesh->mMaterialIndex];
		loadMaterial(meshPart.material, mat);
		loadTextureSet(meshPart.textureSet, mat, scn, mesh, rootPath, minimalLoad);
	}
}

void Mesh::loadMaterial(Material& meshMaterial, const aiMaterial* material)
{
	Material defaultMaterial;

	aiString name;
	aiReturn retValue = AI_SUCCESS;

	// for(unsigned i=0; i< mat->mNumProperties; i++ )
	//{
	//  std::cout << "Property[" << i << "] = " << mat->mProperties[i]->mKey.data
	//  << " = " << mat->mProperties[i]->mData << std::endl;
	//}

	// TODO: Do we need material names?
	// Get returns: aiReturn_SUCCESS 0 | aiReturn_FAILURE -1 |
	// aiReturn_OUTOFMEMORY -3 mat->Get(AI_MATKEY_NAME, name); // may be "" after
	// the input mesh processing. Must be aiString type! subMesh_p->name =
	// name.data;

	aiColor4D color;
	if ((retValue = aiGetMaterialColor(material, AI_MATKEY_COLOR_DIFFUSE, &color)) != AI_SUCCESS)
		color = aiColor4D(defaultMaterial.diffuse.r, defaultMaterial.diffuse.g, defaultMaterial.diffuse.b, 1.0f);

	meshMaterial.diffuse[0] = color.r;
	meshMaterial.diffuse[1] = color.g;
	meshMaterial.diffuse[2] = color.b;

	if ((retValue = aiGetMaterialColor(material, AI_MATKEY_COLOR_AMBIENT, &color)) != AI_SUCCESS)
		color = aiColor4D(meshMaterial.diffuse.r * 0.1f, meshMaterial.diffuse.g * 0.1f, meshMaterial.diffuse.b * 0.1f,
		                  1.0f);

	meshMaterial.ambient[0] = color.r;
	meshMaterial.ambient[1] = color.g;
	meshMaterial.ambient[2] = color.b;

	if ((retValue = aiGetMaterialColor(material, AI_MATKEY_COLOR_SPECULAR, &color)) != AI_SUCCESS)
		color = aiColor4D(defaultMaterial.specular.r, defaultMaterial.specular.g, defaultMaterial.specular.b, 1.0f);

	meshMaterial.specular[0] = color.r;
	meshMaterial.specular[1] = color.g;
	meshMaterial.specular[2] = color.b;

	ai_real shininess, strength;
	unsigned int max; // changed: to unsigned
	max = 1;
	if ((retValue = aiGetMaterialFloatArray(material, AI_MATKEY_SHININESS, &shininess, &max)) != AI_SUCCESS)
		shininess = defaultMaterial.shininess;

	// Presumably we don't want shininess to be < 1.0, 0.0 seems to be default invalid
	if (shininess < 1.0f)
	{
		shininess = defaultMaterial.shininess;
	}
	else
	{
		shininess = std::max(1.0f, shininess);
	}

	max = 1;
	if ((retValue = aiGetMaterialFloatArray(material, AI_MATKEY_SHININESS_STRENGTH, &strength, &max)) != AI_SUCCESS)
		strength = 1.0;

	meshMaterial.shininess = shininess * strength;
}

void Mesh::loadTextureSet(TextureSet& textureSet, const aiMaterial* material, const aiScene* scene, Mesh* mesh,
                          const fs::path& rootPath, bool minimalLoad)
{
	// Diffuse texture
	textureSet.texture = loadTexture(aiTextureType_DIFFUSE, material, scene, mesh, rootPath, minimalLoad);
	if (!textureSet.texture)
		textureSet.texture = loadTexture(aiTextureType_BASE_COLOR, material, scene, mesh, rootPath, minimalLoad);

	// Normal texture
	textureSet.normalMap = loadTexture(aiTextureType_NORMALS, material, scene, mesh, rootPath, minimalLoad);
	// Normal map strength
	float normalStrength;
	if (aiGetMaterialFloat(material, AI_MATKEY_GLTF_TEXTURE_SCALE(aiTextureType_NORMALS, 0), &normalStrength) !=
	    AI_SUCCESS)
	{
		normalStrength = textureSet.normalStrength;
	}
	textureSet.normalStrength = normalStrength;

	// Specular or roughness
	textureSet.specularMap = loadTexture(aiTextureType_SPECULAR, material, scene, mesh, rootPath, minimalLoad);
	if (!textureSet.specularMap)
	{
		textureSet.specularMap =
		    loadTexture(aiTextureType_DIFFUSE_ROUGHNESS, material, scene, mesh, rootPath, minimalLoad);
	}

	// Ambient occlusion
	textureSet.aoMap = loadTexture(aiTextureType_LIGHTMAP, material, scene, mesh, rootPath, minimalLoad);
	// Ambient occlusion strength
	float occlusionStrength;
	if (aiGetMaterialFloat(material, AI_MATKEY_GLTF_TEXTURE_SCALE(aiTextureType_LIGHTMAP, 0), &occlusionStrength) !=
	    AI_SUCCESS)
	{
		occlusionStrength = textureSet.aoStrength;
	}
	textureSet.aoStrength = occlusionStrength;

	// we ignore AI_MATKEY OPACITY, REFRACTION, SHADING_MODEL
}

GLuint Mesh::loadTexture(aiTextureType type, const aiMaterial* material, const aiScene* scene, Mesh* mesh,
                         const fs::path& rootPath, bool minimalLoad)
{
	if (material->GetTextureCount(type) > 0)
	{
		aiString texPathString;
		aiReturn texFound = material->GetTexture(type, 0, &texPathString);
		if (texFound != AI_SUCCESS)
		{
			LOG_ERROR("Mesh: Failed to fetch a texture of type '{}'!", aiTextureTypeToString(type));
			return 0;
		}

		GLuint texId = 0;
		if (minimalLoad)
		{
			texId = -1;
		}
		const aiTexture* aiTex = scene->GetEmbeddedTexture(texPathString.C_Str());
		if (aiTex == nullptr)
		{
			// Regular file texture
			std::filesystem::path texPath(texPathString.C_Str());
			texPath.make_preferred();
			if (texPath.is_relative())
			{
				texPath = rootPath / texPath;
			}
			mesh->m_textureFileList.insert(texPath.string());
			// TODO: (DR) Pass resource manager instance to the load method to avoid App:: calls
			if (!minimalLoad)
			{
				texId = App::getModule<ResourceManager>().texture(texPath.string());
			}
		}
		else
		{
			// The texture is embedded
			if (aiTex->mHeight == 0)
			{
				if (!minimalLoad)
				{
					// Texture is compressed (As per assimp docs, mWidth contains the length)
					LOG_INFO("Mesh: Loading an embedded texture of type '{}'", aiTextureTypeToString(type));
					texId = loadEmbeddedTexture((unsigned char*) &*aiTex->pcData, aiTex->mWidth);
				}
			}
			else
			{
				LOG_ERROR(
				    "Mesh: Failed to load an embedded texture of type '{}': Uncompressed embedded textures are not "
				    "currently supported.",
				    aiTextureTypeToString(type));
			}
		}
		return texId;
	}
	else
	{
		return 0;
	}
}

GLuint Mesh::loadEmbeddedTexture(const unsigned char* data, int length, bool mipmap)
{
	// generate and bind one texture
	GLuint tex = 0;
	glGenTextures(1, &tex);
	glBindTexture(GL_TEXTURE_2D, tex);
	// set linear filtering
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, mipmap ? GL_LINEAR_MIPMAP_LINEAR : GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// upload our image data to OpenGL
	bool result = false;

	stbi_set_flip_vertically_on_load(false);

	int width, height, Bpp;
	unsigned char* imageData = stbi_load_from_memory(data, length, &width, &height, &Bpp, STBI_rgb_alpha);

	if (imageData)
	{
		// printf("%s,bpp %d\n",fileName.c_str(),Bpp);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, imageData);
		result = true;
	}
	else
	{
		const char* error = stbi_failure_reason();
		LOG_ERROR("Mesh: Failed to load an embedded texture: {}", error);
	}
	stbi_image_free(imageData);

	if (!result)
	{
		glBindTexture(GL_TEXTURE_2D, 0);
		glDeleteTextures(1, &tex);
		return 0;
	}
	// create mipmaps
	if (mipmap)
		glGenerateMipmap(GL_TEXTURE_2D);
	// unbind the texture (just in case someone will mess up with texture calls later)
	glBindTexture(GL_TEXTURE_2D, 0);
	CHECK_GL_ERROR();
	return tex;
}
