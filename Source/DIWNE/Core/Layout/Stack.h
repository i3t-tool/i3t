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

#include <vector>

#include "DiwnePanel.h"
#include "Layout.h"

namespace DIWNE
{
/**
 * Stack is a helper layout class that manages a set of vertically or horizontally aligned DIWNE panels.
 * Depending on the mode, the width or height of the panels is synced together, and they are aligned to be next to, or
 * above each other.
 * The stack can be used standalone to align items with fixed spacings, or it can be used within another enclosing
 * DIWNE panel so that springs can be used as spacing.
 */
class Stack : public Layout
{
public:
	enum Mode
	{
		HORIZONTAL,
		VERTICAL
	};

private:
	NodeEditor& m_editor;
	Mode m_mode;
	std::vector<DiwnePanel> m_children;

	bool m_buildingItem = false;  // Make this a DIWNE_DEBUG variable
	bool m_buildingStack = false; // Make this a DIWNE_DEBUG variable

	int m_currentIndex = 0;
	float m_maxFixedSize = 0;
	float m_spacing = -1;
	bool m_springAdded = false;

	ImVec2 origin;

public:
	/// Create a stack
	explicit Stack(NodeEditor& editor, Mode mode) : m_editor(editor), m_mode(mode) {}
	/// Create a stack for a specified DIWNE panel, this allows the usage of the spring() method.
	explicit Stack(NodeEditor& editor, Mode mode, DiwnePanel* panel) : m_editor(editor), m_mode(mode), Layout(panel) {}

	/// Begin the stack, should be called before beginItem().
	void begin();
	/// End the stack, called after begin().
	void end();

	/**
	 * Begins the next stack item.
	 * In vertical mode, the starting X coordinate of items is locked and vertical springs can be used between items.
	 * It is the opposite for horizontal mode.
	 *
	 * @return Panel managing the new item.
	 */
	DiwnePanel* beginItem();
	void endItem();

	/**
	 * Adds a spring corresponding to the layout to the enclosing DIWNE panel.
	 * Should be called between items and requires the Stack to have a reference to the enclosing panel.
	 * @return Whether the spring was truly added.
	 * @see DiwnePanel::spring()
	 */
	bool spring(float relSize);

	void setSpacing(float spacing)
	{
		m_spacing = spacing;
	}

private:
	void layout();
};

} // namespace DIWNE
