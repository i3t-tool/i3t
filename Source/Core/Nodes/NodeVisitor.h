#pragma once

#include "Core/Nodes/Node.h"
#include "Core/Nodes/Transform.h"

namespace Core
{
class NodeVisitor
{
public:
	virtual void visit(const NodePtr& node) {};
	virtual void visit(const TransformPtr& node) {};
};
}
