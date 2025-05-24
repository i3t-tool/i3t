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

#include "DiwnePanel.h"
#include "Stack.h"

namespace DIWNE
{
class HStack : public Stack
{
public:
	/// Create a horizontal item stack
	explicit HStack(NodeEditor& editor) : Stack(editor, Stack::HORIZONTAL) {}
	/// Create a horizontal item stack for a specified DIWNE panel, this allows the usage of the spring() method.
	explicit HStack(NodeEditor& editor, DiwnePanel* panel) : Stack(editor, Stack::HORIZONTAL, panel) {}

	/**
	 * Begins a new column.
	 * Starting position of all columns is locked on the Y coordinate.
	 * All columns are laid out to have the same height.
	 * @return Panel corresponding to the new column.
	 */
	DiwnePanel* beginCol()
	{
		return Stack::beginItem();
	}
	void endCol()
	{
		Stack::endItem();
	}

	/// Adds a horizontal spring between columns.
	void spring(float relSize)
	{
		Stack::spring(relSize);
	}

private:
	DiwnePanel* beginItem() = delete;
	void endItem() = delete;
};
} // namespace DIWNE
