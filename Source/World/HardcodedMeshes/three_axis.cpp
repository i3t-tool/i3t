#include "../HardcodedMeshes.h"
const unsigned int three_axisNAttribsPerVertex = 8;
const unsigned int three_axisNVertices         = 6;
const unsigned int three_axisNTriangles        = 2;
const float        three_axisVertices[]        = {
    0.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.2f, 0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.2f, 0.5f,

    0.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.5f, 0.5f,

    0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f,  0.8f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,  0.8f, 0.5f,
}; // end unitcubeVertices

const unsigned three_axisTriangles[] = {
    0, 1, 2, 3, 4, 5,
}; // end
pgr::MeshData three_axisMesh = {three_axisNVertices, three_axisNTriangles, three_axisNAttribsPerVertex,
                                three_axisVertices, three_axisTriangles};
