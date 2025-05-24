/**
 * \file
 * \brief
 * \author Jaroslav Holeček <holecek.jaroslav@email.cz>, Dan Rakušan <rakusan.dan@gmail.com>
 * \copyright Copyright (C) 2016-2023 I3T team, Department of Computer Graphics
 * and Interaction, FEE, Czech Technical University in Prague, Czech Republic
 *
 * This file is part of I3T - An Interactive Tool for Teaching Transformations
 * http://www.i3t-tool.org
 *
 * GNU General Public License v3.0 (see LICENSE.txt or https://www.gnu.org/licenses/gpl-3.0.txt)
 */

#pragma once

#include "GUI/Workspace/Nodes/Basic/CoreNodeWithPins.h"

namespace Workspace
{
class Cycle : public CoreNodeWithPins
{
	using Super = CoreNodeWithPins;

public:
	//===-- Double dispatch
	//---------------------------------------------------===//
	void accept(NodeVisitor& visitor) override
	{
		visitor.visit(std::static_pointer_cast<Cycle>(shared_from_this()));
	}
	//===----------------------------------------------------------------------===//

	Cycle(DIWNE::NodeEditor& diwne, Ptr<Core::Node> nodebase = Core::GraphManager::createCycle(),
	      bool showDataOnPins = true);
	bool isCycle();

	bool buttonPlayPause();
	bool buttonStopAndReset();
	bool buttonRewind(); ///< Set the current value to \a From
	bool buttonWind();   ///< Set the current value to \a To
	bool bigButton(const std::string& str, DIWNE::IconType iconTypeBg, DIWNE::IconType iconTypeFg);

	bool buttonStepNext();
	bool buttonStepBack();

	void begin(DIWNE::DrawInfo& context) override;
	void centerContent(DIWNE::DrawInfo& context) override;
	void leftContent(DIWNE::DrawInfo& context) override;
	void rightContent(DIWNE::DrawInfo& context) override;

	void drawMenuLevelOfDetail() override;
	LevelOfDetail switchLevelOfDetail(LevelOfDetail oldLevel) override;
	int getLODCount() override;

	int maxLengthOfData() override;

	/**
	 * \brief Radio button - used for cycle mode selection
	 * \param label Button text
	 * \param v_button Button value (button number) - cycle mode assigned to this button
	 * \param v Pressed button value \a v_button - return value of the selected button
	 * \return true if this button was pressed - as a side effect, replace the current mode in \a *v by \a v_button
	 */
	bool myRadioButton(const char* label, int* v, int v_button);
};
} // namespace Workspace