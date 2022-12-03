#include "../HardcodedMeshes.h"
const int lineNAttribsPerVertex = 8;
const int lineNVertices = 24;
const int lineNTriangles = 20;
const float lineVertices[] = {
    1.86264e-11f,  0.0128f,  0.0f, 0.0f,       0.792383f,  -0.609973f, 0.5f, 0.5f,
    1.86264e-11f,  0.0128f,  1.0f, 0.0f,       0.792383f,  0.609973f,  0.5f, 0.5f,
    0.0110851f,    0.0064f,  1.0f, 0.686239f,  0.396191f,  0.609973f,  0.5f, 0.5f,
    0.0110851f,    0.0064f,  0.0f, 0.686239f,  0.396191f,  -0.609973f, 0.5f, 0.5f,
    0.0110851f,    -0.0064f, 1.0f, 0.686239f,  -0.396191f, 0.609973f,  0.5f, 0.5f,
    0.0110851f,    -0.0064f, 0.0f, 0.686239f,  -0.396191f, -0.609973f, 0.5f, 0.5f,
    -1.10039e-09f, -0.0128f, 1.0f, 0.0f,       -0.792383f, 0.609973f,  0.5f, 0.5f,
    -1.10039e-09f, -0.0128f, 0.0f, 0.0f,       -0.792383f, -0.609973f, 0.5f, 0.5f,
    -0.0110851f,   -0.0064f, 1.0f, -0.686239f, -0.396191f, 0.609973f,  0.5f, 0.5f,
    -0.0110851f,   -0.0064f, 0.0f, -0.686239f, -0.396191f, -0.609973f, 0.5f, 0.5f,
    0.0110851f,    -0.0064f, 1.0f, 0.0f,       0.0f,       1.0f,       0.5f, 0.5f,
    0.0110851f,    0.0064f,  1.0f, 0.0f,       0.0f,       1.0f,       0.5f, 0.5f,
    1.86264e-11f,  0.0128f,  1.0f, 0.0f,       0.0f,       1.0f,       0.5f, 0.5f,
    -0.0110851f,   0.0064f,  1.0f, 0.0f,       0.0f,       1.0f,       0.5f, 0.5f,
    -0.0110851f,   -0.0064f, 1.0f, 0.0f,       0.0f,       1.0f,       0.5f, 0.5f,
    -1.10039e-09f, -0.0128f, 1.0f, 0.0f,       0.0f,       1.0f,       0.5f, 0.5f,
    -0.0110851f,   0.0064f,  1.0f, -0.686239f, 0.396191f,  0.609973f,  0.5f, 0.5f,
    -0.0110851f,   0.0064f,  0.0f, -0.686239f, 0.396191f,  -0.609973f, 0.5f, 0.5f,
    -0.0110851f,   0.0064f,  0.0f, -0.0f,      0.0f,       -1.0f,      0.5f, 0.5f,
    1.86264e-11f,  0.0128f,  0.0f, -0.0f,      0.0f,       -1.0f,      0.5f, 0.5f,
    0.0110851f,    0.0064f,  0.0f, -0.0f,      0.0f,       -1.0f,      0.5f, 0.5f,
    0.0110851f,    -0.0064f, 0.0f, 0.0f,       0.0f,       -1.0f,      0.5f, 0.5f,
    -1.10039e-09f, -0.0128f, 0.0f, 0.0f,       0.0f,       -1.0f,      0.5f, 0.5f,
    -0.0110851f,   -0.0064f, 0.0f, 0.0f,       0.0f,       -1.0f,      0.5f, 0.5f,
}; // end cubeVertices

const unsigned lineTriangles[] = {
    0,  1,  2,  0,  2,  3,  3, 2, 4,  3, 4,  5,  5, 4, 6,  5, 6,  7,  7,  6,  8,  7,  8,  9,  10, 11, 12, 12, 13, 14,
    14, 15, 10, 12, 14, 10, 9, 8, 16, 9, 16, 17, 1, 0, 17, 1, 17, 16, 18, 19, 20, 20, 21, 22, 22, 23, 18, 20, 22, 18,
}; // end cubeTriangles
pgr::MeshData lineMesh = {lineNVertices, lineNTriangles, lineNAttribsPerVertex, lineVertices, lineTriangles};
