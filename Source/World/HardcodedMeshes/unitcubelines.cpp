#include "../HardcodedMeshes.h"
const unsigned int cubelinesNAttribsPerVertex = 8;
const unsigned int cubelinesNVertices         = 24;
const unsigned int cubelinesNTriangles        = 12;
const float        cubelinesVertices[]        = {
    1.0f,  1.0f,  -1.0f,     0.0f,      0.0f,  -1.0f, 0.0f,      0.666667f, 1.0f,  -1.0f, -1.0f,     0.0f,
    0.0f,  -1.0f, 0.333333f, 0.666667f, -1.0f, -1.0f, -1.0f,     0.0f,      0.0f,  -1.0f, 0.333333f, 1.0f,
    -1.0f, 1.0f,  -1.0f,     0.0f,      0.0f,  -1.0f, 0.0f,      1.0f,

    1.0f,  1.0f,  1.0f,      0.0f,      -0.0f, 1.0f,  0.333333f, 0.333333f, -1.0f, 1.0f,  1.0f,      0.0f,
    -0.0f, 1.0f,  0.333333f, 0.666667f, -1.0f, -1.0f, 1.0f,      0.0f,      -0.0f, 1.0f,  0.0f,      0.666667f,
    1.0f,  -1.0f, 1.0f,      0.0f,      -0.0f, 1.0f,  0.0f,      0.333333f,

    1.0f,  1.0f,  -1.0f,     1.0f,      -0.0f, 0.0f,  0.666667f, 0.333333f, 1.0f,  1.0f,  1.0f,      1.0f,
    -0.0f, 0.0f,  0.333333f, 0.333333f, 1.0f,  -1.0f, 1.0f,      1.0f,      -0.0f, 0.0f,  0.333333f, 0.0f,
    1.0f,  -1.0f, -1.0f,     1.0f,      -0.0f, 0.0f,  0.666667f, 0.0f,      1.0f,  -1.0f, -1.0f,     -0.0f,
    -1.0f, -0.0f, 0.0f,      0.333333f, 1.0f,  -1.0f, 1.0f,      -0.0f,     -1.0f, -0.0f, 0.0f,      0.0f,
    -1.0f, -1.0f, 1.0f,      -0.0f,     -1.0f, -0.0f, 0.333333f, 0.0f,      -1.0f, -1.0f, -1.0f,     -0.0f,
    -1.0f, -0.0f, 0.333333f, 0.333333f, -1.0f, -1.0f, -1.0f,     -1.0f,     0.0f,  -0.0f, 1.0f,      0.333333f,
    -1.0f, -1.0f, 1.0f,      -1.0f,     0.0f,  -0.0f, 0.666667f, 0.333333f, -1.0f, 1.0f,  1.0f,      -1.0f,
    0.0f,  -0.0f, 0.666667f, 0.0f,      -1.0f, 1.0f,  -1.0f,     -1.0f,     0.0f,  -0.0f, 1.0f,      0.0f,
    1.0f,  1.0f,  1.0f,      0.0f,      1.0f,  0.0f,  0.333333f, 0.333333f, 1.0f,  1.0f,  -1.0f,     0.0f,
    1.0f,  0.0f,  0.666667f, 0.333333f, -1.0f, 1.0f,  -1.0f,     0.0f,      1.0f,  0.0f,  0.666667f, 0.666667f,
    -1.0f, 1.0f,  1.0f,      0.0f,      1.0f,  0.0f,  0.333333f, 0.666667f,
}; // end unitcubeVertices

const unsigned cubelinesTriangles[] = {
	/*0, 1, 2,
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
	20, 22, 23,*/ 		//3*12=36
	0,1,1,2,2,3,3,0,
	4,5,5,6,6,7,7,4,
	0,4,1,7,2,6,3,5,
	0,0,0,0,0,0,0,0,
	0,0,0,0,			//4*8+4=36
}; // end
pgr::MeshData cubelinesMesh = {cubelinesNVertices, cubelinesNTriangles, cubelinesNAttribsPerVertex, cubelinesVertices,
                               cubelinesTriangles};
