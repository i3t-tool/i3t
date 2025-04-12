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

#include <optional>

#include "rapidjson/document.h"

#include "DIWNE/Core/Elements/Node.h"
#include "NodeVisitor.h"
#include "Stateful.h"

namespace NodeDeserializer
{
/// @pre DIWNE and WorkspaceWindow are initialized.
/// @return Vector of Workspace::CoreNodes (casted to base type DIWNE::Node for perf reasons)
std::vector<Ptr<DIWNE::Node>> createFrom(const Memento& memento, bool selectAll = false);

std::optional<Ptr<GuiOperator>> createOperator(const rapidjson::Value& value, bool select);
Ptr<GuiSequence> createSequence(const rapidjson::Value& value, bool select);
std::optional<Ptr<GuiTransform>> createTransform(const rapidjson::Value& value, bool select);

void assignCommon(const rapidjson::Value& value, Ptr<GuiNode> node, bool select);
void assignSequence(const rapidjson::Value& value, Ptr<GuiSequence> sequence, bool select);
} // namespace NodeDeserializer
