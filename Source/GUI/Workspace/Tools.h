/**
 * \file
 * \brief
 * \author Martin Herich <martin.herich@phire.cz>
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
#include "Core/Nodes/GraphManager.h"
#include "Core/Nodes/Id.h"
#include "State/NodeVisitor.h"
#include "State/Stateful.h"

namespace Workspace::Tools
{
std::optional<Ptr<GuiNode>> findNodeById(const std::vector<Ptr<GuiNode>>& nodes, Core::ID id);

Memento* copyNodes(const std::vector<Ptr<GuiNode>>& nodes, float offset = 0);

void pasteNodes(const Memento& memento);

bool plug(Core::ID from, int fromIdx, Core::ID to, int toIdx);

} // namespace Workspace::Tools