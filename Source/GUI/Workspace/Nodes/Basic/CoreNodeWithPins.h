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
#pragma once

#include "Core/Nodes/GraphManager.h"

#include "CoreNode.h"
#include "CorePins.h"
#include "DIWNE/Core/Layout/VStack.h"

namespace Workspace
{
// TODO: Make this node part of DIWNE (BasicNodeWithPins or something)
class CoreNodeWithPins : public CoreNode
{
	using Super = CoreNode;

private:
	float m_minRightAlignOfRightPins{0};

protected:
	std::vector<Ptr<CorePin>> m_workspaceInputs;
	std::vector<Ptr<CorePin>> m_workspaceOutputs;

	std::vector<CorePin*> m_leftPins;
	std::vector<CorePin*> m_rightPins;

	bool m_showDataOnPins; //< default true, false for Camera and Sequence - they do not show data on their output pins

	DIWNE::VStack m_outputPinsVstack{diwne, &m_right};

public:
	CoreNodeWithPins(DIWNE::NodeEditor& diwne, Ptr<Core::Node> nodebase, bool showDataOnPins = true);

	// Lifecycle
	// =============================================================================================================
	void begin(DIWNE::DrawInfo& context) override;
	void leftContent(DIWNE::DrawInfo& context) override;
	void rightContent(DIWNE::DrawInfo& context) override;
	void afterDraw(DIWNE::DrawInfo& context) override;

	void onDestroy(bool logEvent) override;

	// Interaction
	// =============================================================================================================
	bool allowPress(const DIWNE::DrawInfo& context) const override;

protected:
	/**
	 * Inspects mouse position relative to all pins within the node and forces a pin hover operation on the nearest pin
	 * of the mouse is close enough, this allows pins to be (un)plugged more easily, not requiring the mouse to be
	 * directly over them.
	 * @return Whether hover event was forced on one of the pins.
	 */
	bool processPinDragAssist();

public:
	// Pins
	// =============================================================================================================
	/**
	 * @brief get vector of input pins
	 * @note Input pins don't always have to be on the left side of the node!
	 * @return m_workspaceInputs
	 */
	const std::vector<Ptr<CorePin>>& getInputs() const
	{
		return m_workspaceInputs;
	};
	/**
	 * @brief get vector of output pins
	 * @note Output pins don't always have to be on the right side of the node!
	 * @return m_workspaceOutputs
	 */
	const std::vector<Ptr<CorePin>>& getOutputs() const
	{
		return m_workspaceOutputs;
	}

	void unplugAll();

	virtual void drawInputPins(DIWNE::DrawInfo& context);
	virtual void drawOutputPins(DIWNE::DrawInfo& context);

	void updatePinStyle(CorePin& pin);

	// =============================================================================================================

	void translate(const ImVec2& vec) override;

private:
	CorePin* findPinClosestToTheMouse(const std::vector<Ptr<CorePin>>& pins, float& minDistance);
};
} // namespace Workspace
