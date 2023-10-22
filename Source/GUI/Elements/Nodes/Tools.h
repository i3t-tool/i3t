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
#include "Core/Nodes/Tracking.h"
#include "State/NodeVisitor.h"
#include "State/Stateful.h"

std::optional<Ptr<GuiNode>> findNodeById(const std::vector<Ptr<GuiNode>>& nodes, Core::ID id);

Memento* copyNodes(const std::vector<Ptr<GuiNode>>& nodes, float offset = 0);

void duplicateNode(const Ptr<GuiNode>& node, float offset = 0);

void pasteNodes(const Memento& memento);

//------------------------------------------------------------------------------------------------//

class WorkspaceModelProxy : public Core::IModelProxy
{
public:
	WorkspaceModelProxy(Ptr<WorkspaceModel> model);
	~WorkspaceModelProxy() override;

	void update(const glm::mat4& transform) override;
	Ptr<Core::Model> getModel() override;

private:
	Ptr<WorkspaceModel> m_model;
};
