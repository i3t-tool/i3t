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

#include <array>
#include <string>
#include <unordered_set>
#include <vector>

#include "imgui.h"

#include "Core/Nodes/Node.h"
#include "Core/Nodes/NodeData.h"
#include "State/NodeVisitor.h"
#include "State/Stateful.h"

class SerializationVisitor : public NodeVisitor
{
public:
	SerializationVisitor(Memento& memento) : m_memento(memento) {}

	void dump(const std::vector<Ptr<GuiNode>>& nodes);

private:
	/**
	 * Visit node and save its data to m_visitedNodesData member variable.
	 * \param node
	 */
	void visit(const Ptr<GuiCamera>& node) override;
	void visit(const Ptr<GuiCycle>& node) override;
	void visit(const Ptr<GuiOperator>& node) override;
	void visit(const Ptr<GuiSequence>& node) override;
	void visit(const Ptr<GuiTransform>& node) override;
	void visit(const Ptr<GuiScreen>& node) override;
	void visit(const Ptr<GuiModel>& node) override;
	void visit(const Ptr<Workspace::ScriptingNode>& node) override;

	/// id and position
	///
	/// \param target
	/// \param node
	///
	/// \pre target is JSON object
	void dumpCommon(rapidjson::Value& target, const Ptr<GuiNode>& node);

	void dumpSequence(rapidjson::Value& target, const Ptr<GuiSequence>& node);
	void dumpTransform(rapidjson::Value& target, const Ptr<GuiTransform>& node);

	void addData(rapidjson::Value& target, const char* key, const Core::Data& data);

	/// \param target document["edges"] or any JSON array.
	void addEdges(rapidjson::Value& target, const Ptr<Core::Node>& node);

	Memento& m_memento;
};
