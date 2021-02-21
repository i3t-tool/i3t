//----------------
/**
 * \file HardcodedMeshes.h
 * \author Daniel Gruncl
 * \brief meshes hardcoded in source
 */
//---------------
#pragma once
#include "pgr.h"

/*struct pgr::MeshData {
  unsigned nVertices; ///< number of vertices/normals/texCoords
  unsigned nTriangles; ///< number of triangles
  unsigned nAttribsPerVertex; ///< all built-in have 8 -> 3 position, 3 normal, 2 texCoord

  const float* verticesInterleaved; ///< vertex data for each vertex 3pos, 3nor, 2tex
  const unsigned* triangles; ///< for each triangle 3 indices to the vertex array
};*/
extern pgr::MeshData scalearrowMesh;
extern pgr::MeshData unitquadMesh;
extern pgr::MeshData quadMesh;
extern pgr::MeshData arrowMesh;
extern pgr::MeshData lineMesh;
extern pgr::MeshData unitcircleMesh;
// UNIT CUBE \/
extern pgr::MeshData unitcubeMesh;
extern pgr::MeshData cubelinesMesh;
extern pgr::MeshData cameraicoMesh;

// TERRAIN_GRASS \/
extern pgr::MeshData plane_grassMesh;
// TERRAIN_ROCKS \/
extern pgr::MeshData plane_rocksMesh;
// SKYBOX \/
extern pgr::MeshData skyboxMesh;
// PLANE \/
extern pgr::MeshData planeMesh;
