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
* List of types of matrix operators. First parameter of script function mat4oper or mat4 must be one of these values.
*/
struct Mat4types {
	const int matrix=0,trackball=1,inverse=2,transpose=3,determinant=4,matmulvec=7,vecmulmat=8,floatmulmat=9;
	const int scale = 10, translate=11,rotatex =12,rotatey=13,rotatez=14,axisangle=15,ortho=16,perspective=17,frustrum=18,lookAt=19;
	const int free = 20, uniscale = 21;
};
/**
* \struct VecOperators
* List of types of vector operators. First parameter of script function vec4oper must be one of these values.
*/
struct VecOperators {
	const int cross=100,dot=101,norm=102,length=103,vecmulfloat=104,perspdiv=105;
};
/**
* \struct FloatOperators
* List of types of float operators. First parameter of script function float4oper (not implemented yet) must be one of these values.
*/
struct FloatOperators {
	const int clamp=200,cycle=201,pow=205,sincos=207,asinacos=208,signum=209;
};
/**
* \struct ArithmeticOperators
* Must not collide with other types.
*/
struct ArithmeticOperators {
	const int add=300, substract=301, div=302, mul=303,show=304,mix=305;
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
	Mat4types mat4Types;
	VecOperators vecOperators;
	FloatOperators floatOperators;
	ArithmeticOperators arithmeticOperators;
	NodeLODs nodeLODs;
	std::vector<glm::mat4>nodeData;///<Vector of data as mat4, that were created by script. Serves as temporary static storage. Not needed after script is executed.
};

ScriptingData*getScriptingData();
/**
* \fn void clearScriptingData()
* Clears data created by script.
*/
void clearScriptingData();

void platformLibraryInitI3T(Picoc* pc);