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
* List of types of matrix operators and transforms. First parameter of script function mat4oper or mat4 must be one of these values.
*/
struct Mat4types {
	const int trackball=1,inverse=2,transpose=3,determinant=4,matmulvec=7,vecmulmat=8,floatmulmat=9;
	const int scale = 10, translate=11,rotatex =12,rotatey=13,rotatez=14,axisangle=15,ortho=16,perspective=17,frustrum=18,lookAt=19;
	const int free = 20, uniscale = 21;
};
/**
* \struct VecOperators
* List of types of vector operators. First parameter of script function vec4oper or vec3oper must be one of these values.
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
* Can be passed as first parameter to functions mat4oper, vec4oper, vec3oper a floatoper
*/
struct ArithmeticOperators {
	const int add=300, sub=301, div=302, mul=303,show=304,mix=305;
};
struct Convertors {
	const int mat_tr = 400, tr_mat = 401, mat_vecs4 = 402,mat_quat=403,mat_scalars=404,vecs4_mat=405,vec4_vec3=406,vec4_scalars=407;
	const int vecs3_mat=408,vec3_vec4=409,vec3_scalars=410,quat_mat=411,quat_scalars=412,scalars_mat=413,scalars_vec3=414,scalars_vec4=415,scalars_quat=416;
	const int norm_quat=417;
};
/**
* \struct Node4LODs
* List of levels of detail of node display. Third parameter of script function confnode must be one of these values.
*/
struct NodeLODs {
	const int full=500,setvalues=501,label=502;
};
/**
* \struct ScriptingData
* Contains constant variables that are exposed to scripts, that are used as parameters of functions for creating various operators, telling which type of operator should be created.
* Contains data as vector of mat4, which were created by script. Data are then used by script to init storage of created nodes.
* 
*/
struct ScriptingData {
	Mat4types mat4Types;
	VecOperators vecOperators;
	FloatOperators floatOperators;
	ArithmeticOperators arithmeticOperators;
	Convertors convertors;
	NodeLODs nodeLODs;
	std::vector<glm::mat4>nodeData;///<Vector of data as mat4, that were created by script. Serves as temporary static storage. Not needed after script is executed.
	std::vector<Ptr<WorkspaceNodeWithCoreData>>* workspace;
};

ScriptingData*getScriptingData();
/**
* \fn void clearScriptingData()
* Clears data created by script.
*/
void clearScriptingData();
/**
* \fn void platformLibraryInitI3T(Picoc* pc);
* Adds library for manipulating workspace to picoc interpreter
*/
void platformLibraryInitI3T(Picoc* pc);