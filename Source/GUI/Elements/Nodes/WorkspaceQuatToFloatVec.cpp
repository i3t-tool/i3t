//
// Created by Sofie on 05.05.2021.
//

#include "WorkspaceQuatToFloatVec.h"

WorkspaceQuatToFloatVec::WorkspaceQuatToFloatVec()
    : WorkspaceFloatVec3(Core::Builder::createNode<ENodeType::QuatToFloatVec>())
{}

bool WorkspaceQuatToFloatVec::isQuatToFloatVec(){
	return true;
}

