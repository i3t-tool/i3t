/**
 * \file
 * \brief
 * \author Martin Herich
 * \copyright Copyright (C) 2016-2023 I3T team, Department of Computer Graphics
 * and Interaction, FEE, Czech Technical University in Prague, Czech Republic
 *
 * This file is part of I3T - An Interactive Tool for Teaching Transformations
 * http://www.i3t-tool.org
 *
 * GNU General Public License v3.0 (see LICENSE.txt or https://www.gnu.org/licenses/gpl-3.0.txt)
 */
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
