/**
 * \file MeshData.h
 * \author Tomas Barak
 * \brief this includes all pgr built-in mesh data
 * \date 2013
 */

#ifndef MESHDATA_H
#define MESHDATA_H

namespace pgr {

/// we use this to hold built-in meshes (like teapot)
struct MeshData {
  unsigned nVertices; ///< number of vertices/normals/texCoords
  unsigned nTriangles; ///< number of triangles
  unsigned nAttribsPerVertex; ///< all built-in have 8 -> 3 position, 3 normal, 2 texCoord

  const float * verticesInterleaved; ///< vertex data for each vertex 3pos, 3nor, 2tex
  const unsigned * triangles; ///< for each triangle 3 indices to the vertex array
};

// all meshes are stored including normals and texture coordinates

/// plain cube (all faces have the same uv mapping)
extern const MeshData cubeData;
/// icosphere with 3 subdivisions
extern const MeshData icosphere3Data;
/// Utah teapot
extern const MeshData teapotData;
/// Blender Monkey
extern const MeshData monkeyData;

} // end namespace pgr

#endif
