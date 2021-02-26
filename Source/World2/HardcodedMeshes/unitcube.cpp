#include "../HardcodedMeshes.h"
const unsigned int unitcubeNAttribsPerVertex = 8;
const unsigned int unitcubeNVertices = 24;
const unsigned int unitcubeNTriangles = 12;
const float unitcubeVertices[] = {
  1.0f,1.0f,-1.0f,0.0f,0.0f,-1.0f,0.0f,0.666667f,
  1.0f,-1.0f,-1.0f,0.0f,0.0f,-1.0f,0.333333f,0.666667f,
  -1.0f,-1.0f,-1.0f,0.0f,0.0f,-1.0f,0.333333f,1.0f,
  -1.0f,1.0f,-1.0f,0.0f,0.0f,-1.0f,0.0f,1.0f,
  1.0f,0.999999f,1.0f,0.0f,-0.0f,1.0f,0.333333f,0.333333f,
  -1.0f,1.0f,1.0f,0.0f,-0.0f,1.0f,0.333333f,0.666667f,
  -1.0f,-1.0f,1.0f,0.0f,-0.0f,1.0f,0.0f,0.666667f,
  0.999999f,-1.0f,1.0f,0.0f,-0.0f,1.0f,0.0f,0.333333f,
  1.0f,1.0f,-1.0f,1.0f,-0.0f,0.0f,0.666667f,0.333333f,
  1.0f,0.999999f,1.0f,1.0f,-0.0f,0.0f,0.333333f,0.333333f,
  0.999999f,-1.0f,1.0f,1.0f,-0.0f,0.0f,0.333333f,0.0f,
  1.0f,-1.0f,-1.0f,1.0f,-0.0f,0.0f,0.666667f,0.0f,
  1.0f,-1.0f,-1.0f,-0.0f,-1.0f,-0.0f,0.0f,0.333333f,
  0.999999f,-1.0f,1.0f,-0.0f,-1.0f,-0.0f,0.0f,0.0f,
  -1.0f,-1.0f,1.0f,-0.0f,-1.0f,-0.0f,0.333333f,0.0f,
  -1.0f,-1.0f,-1.0f,-0.0f,-1.0f,-0.0f,0.333333f,0.333333f,
  -1.0f,-1.0f,-1.0f,-1.0f,0.0f,-0.0f,1.0f,0.333333f,
  -1.0f,-1.0f,1.0f,-1.0f,0.0f,-0.0f,0.666667f,0.333333f,
  -1.0f,1.0f,1.0f,-1.0f,0.0f,-0.0f,0.666667f,0.0f,
  -1.0f,1.0f,-1.0f,-1.0f,0.0f,-0.0f,1.0f,0.0f,
  1.0f,0.999999f,1.0f,0.0f,1.0f,0.0f,0.333333f,0.333333f,
  1.0f,1.0f,-1.0f,0.0f,1.0f,0.0f,0.666667f,0.333333f,
  -1.0f,1.0f,-1.0f,0.0f,1.0f,0.0f,0.666667f,0.666667f,
  -1.0f,1.0f,1.0f,0.0f,1.0f,0.0f,0.333333f,0.666667f,
}; // end unitcubeVertices

const unsigned unitcubeTriangles[] = {
  0, 1, 2,
  0, 2, 3,
  4, 5, 6,
  4, 6, 7,
  8, 9, 10,
  8, 10, 11,
  12, 13, 14,
  12, 14, 15,
  16, 17, 18,
  16, 18, 19,
  20, 21, 22,
  20, 22, 23,
}; // end unitcubeTriangles
pgr::MeshData unitcubeMesh = { unitcubeNVertices,unitcubeNTriangles,unitcubeNAttribsPerVertex,unitcubeVertices,unitcubeTriangles };
