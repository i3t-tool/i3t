#pragma once

#include "Core/Nodes/Id.h"
#include "State/NodeVisitor.h"

GuiNodePtr findNodeById(const std::vector<Ptr<GuiNode>>& nodes, Core::ID id);

void copyNodes(const std::vector<Ptr<GuiNode>>& nodes);
