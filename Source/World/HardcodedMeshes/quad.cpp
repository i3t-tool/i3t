#include "../HardcodedMeshes.h"
const int   quadNAttribsPerVertex = 8;
const int   quadNVertices         = 4;
const int   quadNTriangles        = 4;
const float quadVertices[]        = {
    0.4f, 0.0f, 0.0f, 0.0f, -0.0f, -1.0f, 0.5f, 0.5f, 0.0f, 0.0f, 0.0f, 0.0f, -0.0f, -1.0f, 0.5f, 0.5f,
    0.0f, 0.4f, 0.0f, 0.0f, -0.0f, -1.0f, 0.5f, 0.5f, 0.4f, 0.4f, 0.0f, 0.0f, -0.0f, -1.0f, 0.5f, 0.5f,
}; // end cubeVertices

const unsigned quadTriangles[] = {0, 1, 2, 0, 2, 3, 2, 1, 0, 3, 2, 0}; // end cubeTriangles

pgr::MeshData quadMesh = {quadNVertices, quadNTriangles, quadNAttribsPerVertex, quadVertices, quadTriangles};
