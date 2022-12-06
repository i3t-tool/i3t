#pragma once

#include "Core/Nodes/Id.h"
#include "State/NodeVisitor.h"
#include "State/Stateful.h"

GuiNodePtr findNodeById(const std::vector<Ptr<GuiNode>>& nodes, Core::ID id);

Memento copyNodes(const std::vector<Ptr<GuiNode>>& nodes);

void pasteNodes(const Memento& memento);
