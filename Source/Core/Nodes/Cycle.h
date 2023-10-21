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

constexpr size_t I3T_CYCLE_IN_FROM = 0;
constexpr size_t I3T_CYCLE_IN_TO = 1;
constexpr size_t I3T_CYCLE_IN_MULT = 2; /* JH is not step + instead of *  ??? */
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
constexpr size_t I3T_CYCLE_OUT_END = 6;

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

	void update(double deltaSeconds);

	void play();
	void pause();
	void stopAndReset();
	void stepBack();
	void stepNext();

	EMode getMode()
	{
		return m_mode;
	}

	void setMode(EMode mode)
	{
		if (m_mode != mode)
		{ // \todo A better variant - use value changed in WorkspaceCycle.cpp
			m_mode = mode;
			m_directionMultiplier = 1.0f;
		}
	}

	bool isRunning() const;
	float getFrom() const;
	float getTo() const;
	float getMultiplier() const;
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
	 * \param v should be a loop increment - \todo to be renamed to setStep
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
		m_stepDuration = std::clamp(stepDuration, 0.0f, FLT_MAX);
	}

	float getStepDuration() const
	{
		return m_stepDuration;
	}

	/**
	 * update inner state from connected inputs (values and pulse inputs)
	 */
	void updateValues(int inputIndex) override;

private:
	void updateValue(float increment);

	float m_from = 0.0f;
	float m_to = 10.0f;
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
