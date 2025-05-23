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
class VStack : public Stack
{
public:
	/// Create a vertical item stack
	explicit VStack(NodeEditor& editor) : Stack(editor, Stack::VERTICAL) {}
	/// Create a vertical item stack for a specified DIWNE panel, this allows the usage of the spring() method.
	explicit VStack(NodeEditor& editor, DiwnePanel* panel) : Stack(editor, Stack::VERTICAL, panel) {}

	/**
	 * Begins a new row.
	 * Starting position of all rows is locked on the X coordinate.
	 * All rows are laid out to have the same width.
	 * @return Panel corresponding to the new row.
	 */
	DiwnePanel* beginRow()
	{
		return Stack::beginItem();
	}
	void endRow()
	{
		Stack::endItem();
	}

	/// Adds a vertical spring between rows.
	void spring(float relSize)
	{
		Stack::spring(relSize);
	}

private:
	DiwnePanel* beginItem() = delete;
	void endItem() = delete;
};
} // namespace DIWNE
