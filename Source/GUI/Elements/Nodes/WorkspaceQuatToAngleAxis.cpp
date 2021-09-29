//
// Created by Sofie on 05.05.2021.
//

#include "WorkspaceQuatToAngleAxis.h"

WorkspaceQuatToAngleAxis::WorkspaceQuatToAngleAxis()
    : WorkspaceFloatVec3(Core::Builder::createNode<ENodeType::QuatToAngleAxis>())
{}


bool WorkspaceQuatToAngleAxis::isQuatToAngleAxis(){
	return true;
}


