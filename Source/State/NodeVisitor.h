#pragma once

#include "Core/Defs.h"

/// GUI nodes.
class WorkspaceCamera;
class WorkspaceCycle;
class WorkspaceNodeWithCoreData;
class WorkspaceSequence;
class WorkspaceTransformation;
class WorkspaceScreen;
class WorkspaceModel;

using GuiNode = WorkspaceNodeWithCoreData;
using GuiCamera = WorkspaceCamera;
using GuiCycle = WorkspaceCycle;
using GuiOperator = WorkspaceNodeWithCoreData; /* JH WorkspaceOperator can be here? */
using GuiTransform = WorkspaceTransformation;
using GuiSequence = WorkspaceSequence;
using GuiScreen = WorkspaceScreen;
using GuiModel = WorkspaceModel;

using GuiNodePtr = std::shared_ptr<GuiNode>;

using GuiNodes = std::vector<Ptr<GuiNode>>;

class NodeVisitor
{
public:
	virtual void visit(const Ptr<GuiCamera>& node){};
	virtual void visit(const Ptr<GuiCycle>& node){};
	virtual void visit(const Ptr<GuiOperator>& node){};
	virtual void visit(const Ptr<GuiSequence>& node){};
	virtual void visit(const Ptr<GuiTransform>& node){};
	virtual void visit(const Ptr<GuiScreen>& node){};
	virtual void visit(const Ptr<GuiModel>& node){};
};

class IVisitable
{
public:
	virtual void accept(NodeVisitor& visitor) = 0;
};
