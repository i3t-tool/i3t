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
namespace DIWNE
{
class DiwnePanel;

// TODO: Not sure if this class is necessary
//  Question still remains, should layouts *not* be panels?
//  And another thing, how to put existing named panel references into a layout?
//  Then I can replace Gravity in BasicNode
class Layout
{
protected:
	DiwnePanel* m_panel{nullptr}; ///< Panel associated with this layout

public:
	Layout() = default;
	explicit Layout(DiwnePanel* panel) : m_panel(panel) {}
};

} // namespace DIWNE
