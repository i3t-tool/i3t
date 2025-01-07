/**
 * \file
 * \brief
 * \author Martin Herich <martin.herich@phire.cz>
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
namespace Workspace
{
class Camera;
class Cycle;
class CoreNode;
class CoreNodeWithPins;
class Sequence;
class TransformationBase;
class Screen;
class Model;
class ScriptingNode;
} // namespace Workspace

using GuiNode = Workspace::CoreNode;
using GuiCamera = Workspace::Camera;
using GuiCycle = Workspace::Cycle;
using GuiOperator = Workspace::CoreNodeWithPins;
using GuiTransform = Workspace::TransformationBase;
using GuiSequence = Workspace::Sequence;
using GuiScreen = Workspace::Screen;
using GuiModel = Workspace::Model;

class NodeVisitor
{
public:
	virtual void visit(const Ptr<GuiCamera>& node) = 0;
	virtual void visit(const Ptr<GuiCycle>& node) = 0;
	virtual void visit(const Ptr<GuiOperator>& node) = 0;
	virtual void visit(const Ptr<GuiSequence>& node) = 0;
	virtual void visit(const Ptr<GuiTransform>& node) = 0;
	virtual void visit(const Ptr<GuiScreen>& node) = 0;
	virtual void visit(const Ptr<GuiModel>& node) = 0;
	virtual void visit(const Ptr<Workspace::ScriptingNode>& node) = 0;
};

class IVisitable
{
public:
	virtual void accept(NodeVisitor& visitor) = 0;
};
