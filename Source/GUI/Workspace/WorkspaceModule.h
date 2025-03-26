#pragma once

#include "Core/Defs.h"

#include "Core/Module.h"
#include "State/Stateful.h"

#include "GUI/Workspace/WorkspaceDiwne.h"

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

	WorkspaceModule();

	Workspace::WorkspaceDiwne& getNodeEditor();

	// NodeEditor management
	// =============================================================================================================

	/// This function takes snapshot of current state.
	template <typename T>
	static auto addNodeToNodeEditor(ImVec2 const position = ImVec2(0, 0))
	{
		auto result = g_editor->createNode<T>(position);

		App::getModule<StateManager>().takeSnapshot();

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
private:
	Memento saveScene(Scene* scene) override;
	void loadScene(const Memento& memento, Scene* scene) override;
	void clearScene() override;

public:
	Memento saveGlobal() override;
	void loadGlobal(const Memento& memento) override;
	void clearGlobal() override;
};
