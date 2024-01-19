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
	bool buttonRewind(); ///< Set the current value to \a From
	bool buttonWind();   ///< Set the current value to \a To

	bool topContent() override;
	bool middleContent() override;
	bool leftContent() override;
	bool rightContent() override;

	void drawMenuLevelOfDetail() override;

	int maxLengthOfData() override;

	bool myRadioButton(const char* label, int* v, int v_button);
	std::vector<Ptr<WorkspaceCoreOutputPin>> const getOutputsToShow() const override
	{
		if (m_levelOfDetail == WorkspaceLevelOfDetail::SetValues ||
		    m_levelOfDetail == WorkspaceLevelOfDetail::LightCycle)
			return {getOutputs()[0]}; // output value only
		else
			return {getOutputs()}; // value and all Pulse outputs
	};
};
