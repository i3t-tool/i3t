/**
 * \file
 * \brief
 * \author Jaroslav Holeček <holecek.jaroslav@email.cz>
 * \copyright Copyright (C) 2016-2023 I3T team, Department of Computer Graphics
 * and Interaction, FEE, Czech Technical University in Prague, Czech Republic
 *
 * This file is part of I3T - An Interactive Tool for Teaching Transformations
 * http://www.i3t-tool.org
 *
 * GNU General Public License v3.0 (see LICENSE.txt or https://www.gnu.org/licenses/gpl-3.0.txt)
 */
//
// Created by Sofie on 07.05.2021.
//

#pragma once
#include "WorkspaceElementsWithCoreData.h"
class WorkspaceCycle : public WorkspaceNodeWithCoreDataWithPins
{
public:
	//===-- Double dispatch
	//---------------------------------------------------===//
	void accept(NodeVisitor& visitor) override
	{
		visitor.visit(std::static_pointer_cast<WorkspaceCycle>(shared_from_this()));
	}
	//===----------------------------------------------------------------------===//

	WorkspaceCycle(DIWNE::Diwne& diwne, Ptr<Core::Node> nodebase = Core::GraphManager::createCycle(),
	               bool showDataOnPins = true);
	bool isCycle();

	bool buttonStepNext();
	bool buttonStepBack();
	bool buttonStopAndReset();
	bool buttonPlayPause();

	bool topContent();
	bool middleContent();
	bool leftContent();
	bool rightContent();

	void drawMenuLevelOfDetail();

	int maxLengthOfData();
};
