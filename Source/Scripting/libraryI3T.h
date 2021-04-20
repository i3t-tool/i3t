#pragma once
#include <vector>
#include <memory>
#include "pgr.h"

struct Mat4Types {
	const int free = 0, scale = 1, uniscale = 2, rotatex = 3, rotatey = 4, rotatez = 5, translate = 6;
};
struct Mat4Operators {
	const int matrix=0,trackball=1,inverse=2,transpose=3,determinant=4,matmul=5,matadd=6;
};
struct Vec4Operators {
	const int norm=0;
};
struct FloatOperators {
	const int clamp=0,cycle=1,mul=2,add=3,div=4,pow=5,mix=6,sincos=7,asinacos=8,signum=9;
};
struct NodeLODs {
	const int full=0,setvalues=1,label=2;
};
struct ScriptingData {
	Mat4Types mat4Types;
	Mat4Operators mat4Operators;
	Vec4Operators vec4Operators;
	FloatOperators floatOperators;
	NodeLODs nodeLODs;
	std::vector<glm::mat4>nodeData;
};

ScriptingData*getScriptingData();
void clearScriptingData();

void PlatformLibraryInitI3T(Picoc* pc);