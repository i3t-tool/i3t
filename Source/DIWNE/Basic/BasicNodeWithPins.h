/**
 * \file
 * \brief
 * \author Dan Rakušan <rakusan.dan@gmail.com>, Jaroslav Holeček <holecek.jaroslav@email.cz>
 * \copyright Copyright (C) 2016-2023 I3T team, Department of Computer Graphics
 * and Interaction, FEE, Czech Technical University in Prague, Czech Republic
 *
 * This file is part of I3T - An Interactive Tool for Teaching Transformations
 * http://www.i3t-tool.org
 *
 * GNU General Public License v3.0 (see LICENSE.txt or https://www.gnu.org/licenses/gpl-3.0.txt)
 */
#pragma once

#include "BasicNode.h"
#include "Core/Nodes/GraphManager.h"

#include "DIWNE/Core/Layout/VStack.h"

namespace DIWNE
{
class BasicNodeWithPins : public BasicNode
{
	using Super = BasicNode;

	float m_minRightAlignOfRightPins{0};

protected:
	std::vector<std::shared_ptr<Pin>> m_inputs;
	std::vector<std::shared_ptr<Pin>> m_outputs;

	std::vector<Pin*> m_leftPins;
	std::vector<Pin*> m_rightPins;

	VStack m_outputPinsVstack{diwne, &m_right};

public:
	BasicNodeWithPins(NodeEditor& diwne, std::string label);

	// Lifecycle
	// =============================================================================================================
	void begin(DrawInfo& context) override;
	void leftContent(DrawInfo& context) override;
	void rightContent(DrawInfo& context) override;
	void afterDraw(DrawInfo& context) override;

	void onDestroy(bool logEvent) override;

	// Interaction
	// =============================================================================================================
	bool allowPress(const DrawInfo& context) const override;

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
	 * @return m_inputs
	 */
	const std::vector<std::shared_ptr<Pin>>& getInputs() const
	{
		return m_inputs;
	};
	/**
	 * @brief get vector of output pins
	 * @note Output pins don't always have to be on the right side of the node!
	 * @return m_outputs
	 */
	const std::vector<std::shared_ptr<Pin>>& getOutputs() const
	{
		return m_outputs;
	}

	void unplugAll();

	virtual void drawInputPins(DrawInfo& context);
	virtual void drawOutputPins(DrawInfo& context);

	// =============================================================================================================

	void translate(const ImVec2& vec) override;

private:
	/// Pin drag assist helper
	Pin* findPinClosestToTheMouse(const std::vector<std::shared_ptr<Pin>>& pins, float& minDistance);
};
} // namespace DIWNE
