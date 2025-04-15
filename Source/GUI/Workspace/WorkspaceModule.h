/**
 * \file
 * \brief
 * \author Dan Rakušan <rakusan.dan@gmail.com>
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

#include "Core/Module.h"
#include "State/Stateful.h"

#include "GUI/Workspace/WorkspaceDiwne.h"
#include "WorkspaceSettings.h"

namespace Workspace
{
class CoreNode;
}

/**
 * Object managing the I3T workspace node editor.
 * Separate from the WorkspaceWindow which merely offers a view upon it.
 */
class WorkspaceModule : public Module, public IStateful
{
public:
	static Ptr<Workspace::WorkspaceDiwne> g_editor;
	static WorkspaceSettings g_settings;

	static bool g_createNodesOnInit;
	static int g_pinStyle;
	static int g_pinStyleMul;
	static int g_pinStylePulse;
	static int g_pinStyleScreen;
	static int g_pinStyleModelMatrix;

	WorkspaceModule();

	Workspace::WorkspaceDiwne& getNodeEditor();

	// NodeEditor management
	// =============================================================================================================

	/// This function takes snapshot of current state.
	template <typename T>
	static auto addNodeToNodeEditor(ImVec2 const position = ImVec2(0, 0))
	{
		auto result = g_editor->createNode<T>(position);

		App::getModule<StateManager>().requestSnapshot();

		return result;
	}

	template <typename T>
	static auto addNodeToNodeEditorNoSave(ImVec2 const position = ImVec2(0, 0))
	{
		return g_editor->createNode<T>(position);
	}

	static bool connectNodesNoSave(Ptr<Workspace::CoreNode> lhs, Ptr<Workspace::CoreNode> rhs, int lhsPin, int rhsPin);
	static bool connectNodes(Ptr<Workspace::CoreNode> lhs, Ptr<Workspace::CoreNode> rhs, int lhsPin, int rhsPin);

	// State save/load
	// =============================================================================================================
public:
	Memento saveScene(Scene* scene) override;
	void loadScene(const Memento& memento, Scene* scene) override;
	void appendScene(const Memento& memento, Scene* scene) override;
	void clearScene(bool newScene) override;

	Memento saveGlobal() override;
	void loadGlobal(const Memento& memento) override;
	void clearGlobal() override;
};
