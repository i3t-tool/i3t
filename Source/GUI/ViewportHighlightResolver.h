#pragma once

#include "Core/Defs.h"

namespace Core
{
class Node;
}

class WorkspaceDiwne;
class WorkspaceModel;
class WorkspaceSequence;
class WorkspaceCamera;

/**
 * Manager class responsible for updating viewport model highlights based on current workspace node selection.
 * A model is highlighted if any currently selected node is directly or indirectly connected to it, potentially
 * influencing it.
 */
class ViewportHighlightResolver
{
private:
	bool m_runResolveThisFrame{false};
	WorkspaceDiwne* m_diwne{nullptr};

public:
	explicit ViewportHighlightResolver(WorkspaceDiwne* diwne);

	/**
	 * Trigger a highlight resolve during resolve() call.
	 * Should be called when selection or structure of the node tree changes.
	 */
	void resolveNeeded()
	{
		m_runResolveThisFrame = true;
	}

	/**
	 * If needed, updates highlight status of all model nodes by starting a breadth-first search for selected nodes at
	 * each one of them. Thus highlighting any models that the current node selection may have an effect on.
	 */
	void resolve();

	/**
	 * Register callbacks with a node.
	 * Makes resolveNeeded() calls when resolve is needed (during node deletion and plug and unplug operations).
	 */
	void registerNodeCallbacks(Core::Node* node);

private:
	/**
	 * Check if a model is connected to any selected nodes.
	 * @param model Model node to start search from
	 */
	bool searchForSelectedNode(Ptr<WorkspaceModel>& model);

	/**
	 * Check if any nodes within a sequence are selected.
	 */
	bool anyInSequenceSelected(WorkspaceSequence* sequence);

	/**
	 * Check if any nodes within a camera are selected.
	 */
	bool anyInCameraSelected(WorkspaceCamera* camera);
};
