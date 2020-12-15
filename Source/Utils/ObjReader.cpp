#include "ObjReader.h"

#include "pgr.h"

#include <iostream>
#include <vector>

#include "Rendering/Geometry.h"

Geometry* ObjReader::geometryFromOBJ(const char* filename)
{
  return geometryFromOBJ(filename, 1.0f);
}

Geometry* ObjReader::geometryFromOBJ(const char* filename, float scale)
{
  std::cout << "reading OBJ file: " << filename << std::endl;

  Assimp::Importer importer;

  // Unitize object in size (scale the model to fit into (-1..1)^3)
  // importer.SetPropertyInteger(AI_CONFIG_PP_PTV_NORMALIZE, 1);

  // Load asset from the file - you can play with various processing steps
  const aiScene* scn = importer.ReadFile(
      filename, 0 | aiProcess_Triangulate // Triangulate polygons (if any).
                    | aiProcess_PreTransformVertices
                    // Transforms scene hierarchy into one root with geometry-leafs only. For more see Doc.
                    | aiProcess_GenSmoothNormals // Calculate normals per vertex.
                    | aiProcess_JoinIdenticalVertices);

  // abort if the loader fails
  if (scn == NULL)
  {
    std::cout << "assimp error: " << importer.GetErrorString() << std::endl;
    return NULL;
  }

  // some formats store whole scene (multiple meshes and materials, lights, cameras, ...) in one file, we cannot
  // handle that in our simplified example
  if (scn->mNumMeshes != 1)
  {
    std::cout << "this simplified loader can only process files with only one mesh" << std::endl;
    return NULL;
  }

  // in this phase we know we have one mesh in our loaded scene, we can directly copy its data to opengl ...
  const aiMesh* mesh = scn->mMeshes[0];

  if (!mesh->HasPositions())
  {
    std::cout << "mesh must contain vertices positions" << std::endl;
    return NULL;
  }

  if (!mesh->HasNormals())
  {
    std::cout << "mesh must contain vertices normals" << std::endl;
    return NULL;
  }

  if (!mesh->HasFaces())
  {
    std::cout << "mesh must contain faces" << std::endl;
    return NULL;
  }

  // verices
  int attribsPerVertex = 6;
  if (mesh->HasTextureCoords(0))
    attribsPerVertex += 2;

  float* vertices = new float[mesh->mNumVertices * attribsPerVertex];
  for (unsigned int i = 0; i < mesh->mNumVertices; i++)
  {
    int p = i * attribsPerVertex;

    vertices[p] = mesh->mVertices[i][0] * scale;
    vertices[p + 1] = mesh->mVertices[i][1] * scale;
    vertices[p + 2] = mesh->mVertices[i][2] * scale;
    vertices[p + 3] = mesh->mNormals[i][0];
    vertices[p + 4] = mesh->mNormals[i][1];
    vertices[p + 5] = mesh->mNormals[i][2];
  }

  if (mesh->HasTextureCoords(0))
  {
    for (unsigned int i = 0; i < mesh->mNumVertices; i++)
    {
      int p = i * attribsPerVertex;

      vertices[p + 6] = mesh->mTextureCoords[0][i][0];
      vertices[p + 7] = mesh->mTextureCoords[0][i][1];
    }
  }

  // copy all mesh faces into one big array (assimp supports faces with ordinary number of vertices, we use only 3 ->
  // triangles)
  unsigned int* indices = new unsigned int[mesh->mNumFaces * 3];
  for (unsigned int f = 0; f < mesh->mNumFaces; ++f)
  {
    indices[f * 3 + 0] = mesh->mFaces[f].mIndices[0];
    indices[f * 3 + 1] = mesh->mFaces[f].mIndices[1];
    indices[f * 3 + 2] = mesh->mFaces[f].mIndices[2];
  }

  return new Geometry(vertices, indices, mesh->mNumVertices, mesh->mNumFaces * 3, attribsPerVertex);
}
