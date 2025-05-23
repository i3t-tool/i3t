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

#include "DIWNE/Core/diwne_common.h"

namespace DIWNE
{
class Node;
template <typename NodeType = Node>
class NodeRange;

/**
 * Elementary node container interface defining access to child nodes.
 * @see NodeContainer
 */
class INodeContainer
{
public:
	virtual ~INodeContainer() = default;

	virtual NodeRange<> getNodes() const = 0;
	virtual NodeList& getNodeList() = 0;
};

} // namespace DIWNE
