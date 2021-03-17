#pragma once
#include <vector>
#include <memory>
#include "pgr.h"

struct Mat4Types {
	const int free = 0, scale = 1, uniscale = 2, rotatex = 3, rotatey = 4, rotatez = 5, translate = 6;
};
struct NodeMat4 {
	int type;
	glm::mat4 data;
	int x, y;
	NodeMat4(int _type,glm::mat4 _data, int _x, int _y) {type=_type;data=_data;x=_x;y=_y;}
};
struct NodeNormVec4 {
	glm::vec4 data;
	int x, y;
	NodeNormVec4(glm::vec4 _data, int _x, int _y) {data = _data; x = _x; y = _y;}
};
struct NodePlug {
	int indexA,indexB;
	int indexPinA,indexPinB;
	NodePlug(int _a, int _b,int _pina,int _pinb ){ indexA =_a; indexB =_b; indexPinA =_pina; indexPinB =_pinb;}
};
struct WorkspaceLayout {
	Mat4Types mat4Types;
	std::vector<NodeMat4>mat4Nodes;
	std::vector<NodeNormVec4>normVec4Nodes;
	std::vector<NodePlug>nodePlugs;
	std::vector<glm::mat4>nodeData;
};

WorkspaceLayout*getWorkspaceLayout();
void clearWorkspaceLayout();

void PlatformLibraryInitI3T(Picoc* pc);