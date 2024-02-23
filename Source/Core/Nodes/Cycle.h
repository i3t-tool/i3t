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

#include "Core/Nodes/Node.h"

namespace Core
{
constexpr auto I3T_CYCLE_STEP_DURATION_SECONDS = 0.5f;
constexpr auto I3T_CYCLE_STEP_DURATION_MINIMUM = 0.01f; // to avoid cycle STOP when 0.0f

constexpr size_t I3T_CYCLE_IN_FROM = 0;
constexpr size_t I3T_CYCLE_IN_TO = 1;
constexpr size_t I3T_CYCLE_IN_STEP = 2;
constexpr size_t I3T_CYCLE_IN_PLAY = 3;
constexpr size_t I3T_CYCLE_IN_PAUSE = 4;
constexpr size_t I3T_CYCLE_IN_STOP = 5;
constexpr size_t I3T_CYCLE_IN_PREV = 6;
constexpr size_t I3T_CYCLE_IN_NEXT = 7;

constexpr size_t I3T_CYCLE_OUT_VAL = 0;
constexpr size_t I3T_CYCLE_OUT_PLAY = 1;
constexpr size_t I3T_CYCLE_OUT_PAUSE = 2;
constexpr size_t I3T_CYCLE_OUT_STOP = 3;
constexpr size_t I3T_CYCLE_OUT_PREV = 4;
constexpr size_t I3T_CYCLE_OUT_NEXT = 5;
constexpr size_t I3T_CYCLE_OUT_BEGIN = 6; // PF
constexpr size_t I3T_CYCLE_OUT_END = 7;

class Cycle;

namespace Builder
{
Ptr<class Cycle> createCycle();
}

class Cycle : public Node
{
public:
	enum class EMode
	{
		Once,
		Repeat,
		PingPong
	};

public:
	Cycle() : Node(&g_CycleProperties) {}

	/**
	 * \brief Perform a regular cycle value update (if running). Called by the GraphManager::update(double tick)
	 * Adjusts the increment and calls updateValue(increment).
	 * \param deltaSeconds elapsed time
	 */
	void update(double deltaSeconds);

	void play();
	void pause();
	void stopAndReset();
	void stepBack();
	void stepNext();
	void rewind();
	void wind();

	EMode getMode()
	{
		return m_mode;
	}

	void setMode(EMode mode)
	{
		if (m_mode != mode)
		{ // \todo A better variant - use value changed in WorkspaceCycle.cpp
			m_mode = mode;
			m_directionMultiplier = 1.0f; ///< direction from m_from to m_to
		}
	}

	bool isRunning() const;
	float getFrom() const;
	float getTo() const;
	float getStep() const;
	float getManualStep() const;

	/**
	 * \param from start value
	 */
	void setFrom(float from);

	/**
	 * \param to end value
	 */
	void setTo(float to);

	/**
	 * \param v should be a loop increment
	 */
	void setStep(float v);

	/**
	 * \param v increment added to/subtracted from the cycle value after user
	 * action - click to Next/Prev button
	 */
	void setManualStep(float v);

	void setSmoothStep(bool smoothStep)
	{
		m_smoothStep = smoothStep;
	}

	bool getSmoothStep() const
	{
		return m_smoothStep;
	}

	void setStepDuration(float stepDuration)
	{
		m_stepDuration = std::clamp(stepDuration, I3T_CYCLE_STEP_DURATION_MINIMUM, FLT_MAX);
	}

	float getStepDuration() const
	{
		return m_stepDuration;
	}

	/**
	 * Update the inner state from all connected inputs (take values from the from, to, step and pulse inputs).
	 * \todo (PF) Why does it ignore the input pin????
	 */
	void updateValues(int inputIndex) override;

private:
	/**
	 * \brief Perform a single value update during the cycle loop (update()) or the manual step (stepBack and stepNext)
	 * according to the current \a m_mode.
	 *
	 * Compute newValue as \a value + \a increment.
	 * - if newValue is in interval <from, to> then set it unchanged
	 * - if newValue is out of bounds <from, to>, modify the newValue and use the modified newValue
	 *
	 * \param increment An increment to the current cycle value
	 */
	void updateValue(float increment);
	float m_from = 0.0f; ///< The initial cycle value (begin) - if here, fire the I3T_CYCLE_OUT_BEGIN pulse
	float m_to = 10.0f;  ///< The final value of the cycle (end0 - if here, fire the I3T_CYCLE_OUT_END pulse

	/// step after pressing of Prev or Next button
	float m_manualStep = 0.1f;
	/// increment per second
	float m_step = 0.2f;

	/// reverse the increment if (from > to) and flip in the PingPong mode
	float m_directionMultiplier = 1.0f;

	bool m_isRunning = false;

	float m_stepDuration = I3T_CYCLE_STEP_DURATION_SECONDS;

	float m_secondsSinceLastStep = 0.0f;

	bool m_smoothStep = true;

	EMode m_mode = EMode::Repeat;
};
} // namespace Core
