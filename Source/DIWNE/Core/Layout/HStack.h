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
