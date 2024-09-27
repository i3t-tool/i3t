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

namespace Core
{
class Node;
}

namespace Workspace
{
class WorkspaceDiwne;
class Model;
class Sequence;
class Camera;

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
	bool searchForSelectedNode(Ptr<Model>& model);

	/**
	 * Check if any nodes within a sequence are selected.
	 */
	bool anyInSequenceSelected(Sequence* sequence);

	/**
	 * Check if any nodes within a camera are selected.
	 */
	bool anyInCameraSelected(Camera* camera);
};
} // namespace Workspace