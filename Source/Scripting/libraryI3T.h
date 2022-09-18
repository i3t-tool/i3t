/**
 * \file libraryI3T.h
 * \author Daniel Gruncl
 * \date 12.3.2021
 */
#pragma once

#include <memory>
#include <vector>

#include "pgr.h"
#include "picoc.h"
/**
*
* LOADING, ADDING NEW FUNCTIONS
*	libraryI3T.cpp ->
*		Create function with following prototype
*			void f(struct ParseState* parser, struct Value* returnValue, struct
Value** param, int numArgs) *		Add pair (example, you may define any function
prototype you want, however, number of parameters is limited to PARAMETER_MAX
(Dependencies/picoc/platform.h)) *			{ f,     "int f(int,int,int,char*);" } *
to existing array *			struct LibraryFunction platformLibraryI3T[] *		Function
*			void platformLibraryInitI3T(Picoc *pc)
*		is called on picoc init functions (Scripting.h, Scripting.cpp). This
function begins with *			IncludeRegister(pc, "I3T.h", nullptr,
platformLibraryI3T, defs) *		which adds the array to picoc *		Then definitions
of constats, accessible in scripting, follow: * VariableDefinePlatformVar(pc,
nullptr, "free", &pc->IntType, (union AnyValue
*)&scriptingData.mat4Types.free, false); *		This call defines variable const
int free=scriptingData.mat4Types.free; accessible in script. (last parameter
writeable=false) *	libraryI3T.h -> *		Here you may define custom constants and
global variables, accessible in script *		struct ScriptingData contains all of
these variables and also vector of mat4, *		which is only used on script
execution, during which the vector is filled by script (functions datavec3,
datascalar, etc...) and then *		created matricies can accessed by other
functions of the script.
* SAVING
*   Scripting.cpp ->
*		There are two similar functions, but their purpose is different
*			bool saveWorkspace(const char* filename,
std::vector<Ptr<WorkspaceNodeWithCoreData>>* _workspace) *			bool
saveWorkspace(FILE* f, std::vector<Ptr<WorkspaceNodeWithCoreData>> *
_workspace,int at) *		These functions fprintf's c script directly into target
file on disc. *		The first one calls the second, and the second may also call
itself recursively. The second serves for saving nodes only - *		nodes may
contain other nodes (Sequence, camera), thus the recursive calling. *		If you
want to save non-node related stuff, do it in the first functions, because the
second may be called multiple times during * saving process. *		If you want to
save nodes, do it in the second function. The functions consist form huge
if-else structure, which switches by *		node keyword
(WorkspaceNodeWithCoreData->Core::NodeBase->Operation->keyWord), in each branch
functions for saving given node and *		its inner state are fprintfed.
*
*
*
*
*/
#include "GUI/Elements/Nodes/WorkspaceElementsWithCoreData.h"

/**
 * \struct Mat4types
 * List of types of matrix operators and transforms. First parameter of script
 * function mat4oper or mat4 must be one of these values.
 */
struct Mat4types
{
	const int trackball = 1, transpose = 3, determinant = 4, matmulvec = 7,
	          vecmulmat = 8, floatmulmat = 9;
	const int scale = 10, translate = 11, rotatex = 12, rotatey = 13,
	          rotatez = 14, axisangle = 15, ortho = 16, perspective = 17,
	          frustrum = 18, lookAt = 19;
	const int free = 20, uniscale = 21;
};
/**
 * \struct VecOperators
 * List of types of vector operators. First parameter of script function
 * vec4oper or vec3oper must be one of these values.
 */
struct VecOperators
{
	const int cross = 100, dot = 101, vecmulfloat = 104, perspdiv = 105;
};
/**
 * \struct FloatOperators
 * List of types of float operators. First parameter of script function
 * float4oper (not implemented yet) must be one of these values.
 */
struct FloatOperators
{
	//	const int clamp=200,cycle=201,pow=205,sincos=207,asinacos=208,signum=209;
	const int clamp = 200, pow = 205, sincos = 207, asinacos = 208, signum = 209;
};
/**
 * \struct ArithmeticOperators
 * Must not collide with other types.
 * Can be passed as first parameter to functions mat4oper, vec4oper, vec3oper a
 * floatoper
 */
struct ArithmeticOperators
{
	const int add = 300, sub = 301, div = 302, mul = 303, show = 304, mix = 305,
	          inverse = 306, norm = 307, length = 308;
};
struct Convertors
{
	const int mat_tr = 400, tr_mat = 401, mat_vecs4 = 402, mat_quat = 403,
	          mat_scalars = 404, vecs4_mat = 405, vec4_vec3 = 406,
	          vec4_scalars = 407;
	const int vecs3_mat = 408, vec3_vec4 = 409, vec3_scalars = 410,
	          quat_mat = 411, quat_scalars = 412, scalars_mat = 413,
	          scalars_vec3 = 414, scalars_vec4 = 415, scalars_quat = 416;
};
struct QuatOperators
{
	const int scalarvec3_quat = 501, angleaxis_quat = 502, vec3vec3_quat = 503,
	          quat_scalarvec3 = 504, quat_angleaxis = 505, scalarmulquat = 506;
	const int quat_euler = 507, euler_quat = 508, slerp = 509, longslerp = 510,
	          lerp = 511, conjugate = 512, qvq = 513;
};
/**
 * \struct Node4LODs
 * List of levels of detail of node display. Third parameter of script function
 * confnode must be one of these values.
 */
struct NodeLODs
{
	const int full = 600, setvalues = 601, label = 602;
};
struct CamAddModes
{
	const int proj = 700, view = 701;
};
/**
 * \struct ScriptingData
 * Contains constant variables that are exposed to scripts, that are used as
 * parameters of functions for creating various operators, telling which type of
 * operator should be created. Contains data as vector of mat4, which were
 * created by script. Data are then used by script to init storage of created
 * nodes.
 *
 */
struct ScriptingData
{
	Mat4types mat4Types;
	VecOperators vecOperators;
	FloatOperators floatOperators;
	ArithmeticOperators arithmeticOperators;
	Convertors convertors;
	QuatOperators quatOperators;
	NodeLODs nodeLODs;
	CamAddModes camAddModes;
	std::vector<glm::mat4>
	    nodeData; ///< Vector of data as mat4, that were created by script. Serves
	              ///< as temporary static storage. Not needed after script is
	              ///< executed.
	std::vector<Ptr<WorkspaceNodeWithCoreData>>* workspace;
};

ScriptingData* getScriptingData();
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
