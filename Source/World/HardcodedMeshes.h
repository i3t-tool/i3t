//----------------
/**
 * \file HardcodedMeshes.h
 * \author Daniel Gruncl
 * \brief Meshes hardcoded in source.
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
extern pgr::MeshData scalearrowMesh; //scale arrow handle
extern pgr::MeshData unitquadMesh;   //unit quad
extern pgr::MeshData quadMesh;       //quad handle
extern pgr::MeshData arrowMesh;      //arrow handle
extern pgr::MeshData lineMesh;       //line mesh
extern pgr::MeshData unitcircleMesh; //ring handle
extern pgr::MeshData unitcubeMesh;   //unit cube
extern pgr::MeshData cubelinesMesh;  //unit cube line representation
extern pgr::MeshData cameraicoMesh;  //camera icon handle
extern pgr::MeshData three_axisMesh; //x,y,z lines
extern pgr::MeshData gridMesh;       //line grid
