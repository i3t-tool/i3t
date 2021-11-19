#pragma once

#include "Core/Defs.h"

/// GUI nodes.
class WorkspaceCamera;
class WorkspaceCycle;
class WorkspaceNodeWithCoreData;
class WorkspaceSequence;
class WorkspaceTransformation;

using GuiCamera    = WorkspaceCamera;
using GuiCycle     = WorkspaceCycle;
using GuiOperator  = WorkspaceNodeWithCoreData;
using GuiTransform = WorkspaceTransformation;
using GuiSequence  = WorkspaceSequence;

class NodeVisitor
{
public:
	virtual void visit(const Ptr<GuiCamera>& node) {};
	virtual void visit(const Ptr<GuiCycle>& node) {};
	virtual void visit(const Ptr<GuiOperator>& node) {};
	virtual void visit(const Ptr<GuiSequence>& node) {};
	virtual void visit(const Ptr<GuiTransform>& node) {};
};

class IVisitable
{
public:
	virtual void accept(NodeVisitor& visitor) = 0;
};
