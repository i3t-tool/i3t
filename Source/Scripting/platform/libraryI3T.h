#pragma once
#include "GUI/Elements/Nodes/WorkspaceElements.h"
#include "GUI/Elements/Nodes/WorkspaceMatrix4x4.h"
#include <vector>
#include <memory>
struct NodeMatrix4x4 {
	WorkspaceMatrix4x4* node;
	int x, y;
	NodeMatrix4x4(WorkspaceMatrix4x4* _node, int _x, int _y) {node=_node;x=_x;y=_y;}
};
struct NodePlug {
	int indexA,indexB;
	int indexPinA,indexPinB;
	NodePlug(int _a, int _b,int _pina,int _pinb ){ indexA =_a; indexB =_b; indexPinA =_pina; indexPinB =_pinb;}
};
struct WorkspaceLayout {
	std::vector<NodeMatrix4x4>matrix4x4Nodes;
	std::vector<NodePlug>nodePlugs;
};

WorkspaceLayout*getWorkspaceLayout();
void clearWorkspaceLayout();