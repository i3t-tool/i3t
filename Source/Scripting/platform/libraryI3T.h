#pragma once
#include "GUI/Elements/Nodes/WorkspaceElements.h"
#include "GUI/Elements/Nodes/WorkspaceMatrix4x4.h"
#include <vector>
#include <memory>
struct NodeMat4 {
	Core::Transform::DataMap type;
	glm::mat4 data;
	int x, y;
	NodeMat4(Core::Transform::DataMap _type,glm::mat4 _data, int _x, int _y) {type=_type;data=_data;x=_x;y=_y;}
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
	std::vector<NodeMat4>mat4Nodes;
	std::vector<NodeNormVec4>normVec4Nodes;
	std::vector<NodePlug>nodePlugs;
	std::vector<glm::mat4>nodeData;
};

WorkspaceLayout*getWorkspaceLayout();
void clearWorkspaceLayout();