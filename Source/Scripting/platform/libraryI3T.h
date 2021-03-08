#pragma once
#include "GUI/Elements/Nodes/WorkspaceElements.h"
#include "GUI/Elements/Nodes/WorkspaceMatrix4x4.h"
#include <vector>
#include <memory>
struct NodeMatrix4x4 {
	Core::Transform::DataMap type;
	glm::mat4 data;
	int x, y;
	NodeMatrix4x4(Core::Transform::DataMap _type,glm::mat4 _data, int _x, int _y) {type=_type;data=_data;x=_x;y=_y;}
};
struct NodePlug {
	int indexA,indexB;
	int indexPinA,indexPinB;
	NodePlug(int _a, int _b,int _pina,int _pinb ){ indexA =_a; indexB =_b; indexPinA =_pina; indexPinB =_pinb;}
};
struct WorkspaceLayout {
	std::vector<NodeMatrix4x4>matrix4x4Nodes;
	std::vector<NodePlug>nodePlugs;
	std::vector<glm::mat4>nodeData;
};

WorkspaceLayout*getWorkspaceLayout();
void clearWorkspaceLayout();