/**
 * \file
 * \brief
 * \author Dan Rakušan <rakusan.dan@gmail.com>
 * \copyright Copyright (C) 2016-2025 I3T team, Department of Computer Graphics
 * and Interaction, FEE, Czech Technical University in Prague, Czech Republic
 *
 * This file is part of I3T - An Interactive Tool for Teaching Transformations
 * http://www.i3t-tool.org
 *
 * GNU General Public License v3.0 (see LICENSE.txt or https://www.gnu.org/licenses/gpl-3.0.txt)
 */
#pragma once

#include "InputAdapter.h"

namespace DIWNE
{
class NodeEditor;

class NodeEditorInputAdapter : public InputAdapter
{
protected:
	NodeEditor& editor;

public:
	NodeEditorInputAdapter(NodeEditor& editor);

	virtual bool selectionRectangleKeyDown();
	virtual bool selectionRectangleKeyPressed();
	virtual bool selectionRectangleDragging();
	virtual ImVec2 selectionRectangleStartPosition();

	virtual bool panKeyDown();
	virtual bool panKeyPressed();
	virtual bool panDragging();

	virtual bool selectAllNodes();
	virtual bool invertSelection();
	virtual bool deleteSelectedNodes();

	/**
	 * Multi selection allows multiple items to be selected.
	 * Depending on the context is either allows other items to be selected, or it simply toggles the selection state
	 * on individual objects.
	 */
	virtual bool multiSelectionActive();
	/**
	 * Subtractive selection version of the multi selection. Deselects a portion of items.
	 */
	virtual bool multiDeselectionActive();

	virtual float getZoomDelta();

	virtual bool bypassIsMouseDragging0() override;
	virtual bool bypassIsMouseDragging1() override;
	virtual bool bypassIsMouseDragging2() override;
};
} // namespace DIWNE
