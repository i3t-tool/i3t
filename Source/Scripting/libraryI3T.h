#pragma once
#include <vector>
#include <memory>
#include "pgr.h"

struct Mat4Types {
	const int free = 0, scale = 1, uniscale = 2, rotatex = 3, rotatey = 4, rotatez = 5, translate = 6;
};
struct ScriptingData {
	Mat4Types mat4Types;
	std::vector<glm::mat4>nodeData;
};

ScriptingData*getScriptingData();
void clearScriptingData();

void PlatformLibraryInitI3T(Picoc* pc);