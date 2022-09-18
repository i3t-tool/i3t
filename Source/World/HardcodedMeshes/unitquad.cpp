#include "../HardcodedMeshes.h"
const int unitquadNAttribsPerVertex = 8;
const int unitquadNVertices = 4;
const int unitquadNTriangles = 2;
const float unitquadVertices[] = {
    1.0f, 0.0f,  0.0f,  0.0f, -0.0f, -1.0f, 1.0f,  0.0f,  0.0f, 0.0f,  0.0f,
    0.0f, -0.0f, -1.0f, 0.0f, 0.0f,  0.0f,  1.0f,  0.0f,  0.0f, -0.0f, -1.0f,
    0.0f, 1.0f,  1.0f,  1.0f, 0.0f,  0.0f,  -0.0f, -1.0f, 1.0f, 1.0f,
}; // end cubeVertices

const unsigned unitquadTriangles[] = {0, 1, 2, 0, 2, 3}; // end cubeTriangles

pgr::MeshData unitquadMesh = {unitquadNVertices, unitquadNTriangles,
                              unitquadNAttribsPerVertex, unitquadVertices,
                              unitquadTriangles};
