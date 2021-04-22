#pragma once
/**
 * \file libraryI3T.h
 * \author Daniel Gruncl
 * \date 12.3.2021
 */
#include <vector>
#include <memory>
#include "pgr.h"

/**
* \struct Mat4types
* List of types of matrix transformation. First parameter of script function mat4 must be one of these values.
*/
struct Mat4Types {
	const int free = 0, scale = 1, uniscale = 2, rotatex = 3, rotatey = 4, rotatez = 5, translate = 6;
};
/**
* \struct Mat4Operators
* List of types of matrix operators. First parameter of script function mat4oper must be one of these values.
*/
struct Mat4Operators {
	const int matrix=0,trackball=1,inverse=2,transpose=3,determinant=4,matmul=5,matadd=6,matmulvec=7,vecmulmat=8,floatmulmat=9;
};
/**
* \struct Vec4Operators
* List of types of vector operators. First parameter of script function vec4oper must be one of these values.
*/
struct Vec4Operators {
	const int norm=0;
};
/**
* \struct Mat4Operators
* List of types of float operators. First parameter of script function float4oper (not implemented yet) must be one of these values.
*/
struct FloatOperators {
	const int clamp=0,cycle=1,mul=2,add=3,div=4,pow=5,mix=6,sincos=7,asinacos=8,signum=9;
};
/**
* \struct Node4LODs
* List of levels of detail of node display. Third parameter of script function confnode must be one of these values.
*/
struct NodeLODs {
	const int full=0,setvalues=1,label=2;
};
/**
* \struct ScriptingData
* Contains constant variables that are exposed to scripts, that are used as parameters of functions for creating various operators, telling which type of operator should be created.
* Contains data as mat4, which were created by script. Data are then used by script to init storage of created nodes.
* 
*/
struct ScriptingData {
	Mat4Types mat4Types;
	Mat4Operators mat4Operators;
	Vec4Operators vec4Operators;
	FloatOperators floatOperators;
	NodeLODs nodeLODs;
	std::vector<glm::mat4>nodeData;///<Vector of data as mat4, that were created by script. Serves as temporary static storage. Not needed after script is executed.
};

ScriptingData*getScriptingData();
/**
* \fn void clearScriptingData()
* Clears data created by script.
*/
void clearScriptingData();

void PlatformLibraryInitI3T(Picoc* pc);