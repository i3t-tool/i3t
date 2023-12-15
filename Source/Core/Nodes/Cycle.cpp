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
#include "Cycle.h"
#include "GraphManager.h"

namespace Core
{
Ptr<Cycle> Builder::createCycle()
{
	auto ret = std::make_shared<Cycle>();
	ret->init();
	ret->updateValues(-1);
	ret->stopAndReset();

	return ret;
}

void Cycle::update(double deltaSeconds)
{
	if (!m_isRunning)
	{
		return;
	}

	if (m_smoothStep)
	{
		const auto increment = static_cast<float>(deltaSeconds * m_step / m_stepDuration);
		updateValue(increment);
	}
	else if (m_secondsSinceLastStep >= m_stepDuration)
	{
		const auto increment = static_cast<float>(m_step);
		updateValue(increment);
		m_secondsSinceLastStep = 0.0f;
	}

	m_secondsSinceLastStep += deltaSeconds;
}

void Cycle::play()
{
	float currentValue = getData().getFloat();

	if (m_mode == EMode::Once)
	{
		// rewind to start (to m_from) after stop at m_to
		if ((m_from <= m_to && currentValue >= m_to) || (m_from > m_to && currentValue <= m_to))
		{
			setInternalValue(m_from);
		}
	}

	m_isRunning = true;
	pulse(I3T_CYCLE_OUT_PLAY);
}

void Cycle::pause()
{
	m_isRunning = false;

	pulse(I3T_CYCLE_OUT_PAUSE);
}

void Cycle::stopAndReset()
{
	m_isRunning = false;
	setInternalValue(m_from); // PF was missing

	pulse(I3T_CYCLE_OUT_STOP);
}

void Cycle::stepBack()
{
	updateValue((-1.0f) * m_manualStep);
	pulse(I3T_CYCLE_OUT_PREV);
}

void Cycle::stepNext()
{
	updateValue(m_manualStep);
	pulse(I3T_CYCLE_OUT_NEXT);
}

void Cycle::setFrom(float from)
{
	m_from = from;
}

void Cycle::setTo(float to)
{
	m_to = to;
}

void Cycle::setStep(float v)
{
	m_step = std::clamp(v, 0.0f, FLT_MAX);
}

void Cycle::setManualStep(float v)
{
	m_manualStep = v;
}

bool Cycle::isRunning() const
{
	return m_isRunning;
}

float Cycle::getFrom() const
{
	return m_from;
}

float Cycle::getTo() const
{
	return m_to;
}

float Cycle::getMultiplier() const
{
	return m_step;
}

float Cycle::getManualStep() const
{
	return m_manualStep;
}

void Cycle::updateValues(int inputIndex)
{
	if (m_inputs[I3T_CYCLE_IN_FROM].isPluggedIn())
	{
		float val = getInput(I3T_CYCLE_IN_FROM).data().getFloat();
		setFrom(val);
	}

	if (getInput(I3T_CYCLE_IN_TO).isPluggedIn())
	{
		float val = getInput(I3T_CYCLE_IN_TO).data().getFloat();
		setTo(val);
	}

	if (m_inputs[I3T_CYCLE_IN_STEP].isPluggedIn())
	{
		float val = getInput(I3T_CYCLE_IN_STEP).data().getFloat();
		setStep(val);
	}

	if (shouldPulse(I3T_CYCLE_IN_PLAY, inputIndex))
	{
		play();
	}
	else if (shouldPulse(I3T_CYCLE_IN_PAUSE, inputIndex))
	{
		pause();
	}
	if (shouldPulse(I3T_CYCLE_IN_STOP, inputIndex))
	{
		stopAndReset();
	}
	if (shouldPulse(I3T_CYCLE_IN_PREV, inputIndex))
	{
		stepBack();
	}
	if (shouldPulse(I3T_CYCLE_IN_NEXT, inputIndex))
	{
		stepNext();
	}
}

void Cycle::updateValue(float increment)
{
	const float currentValue = getData().getFloat();
	float newValue = currentValue + ((m_from <= m_to) ? 1.0f : -1.0f) * m_directionMultiplier * increment;

	// if out of bounds, clamp values to the range <m_from, m_to> or <m_to,
	// m_from> if(newValue < std::min(m_from, m_to) ||  newValue >
	// std::max(m_from, m_to) )  // probably more readable
	if (m_from <= m_to && (m_to < newValue || newValue < m_from) ||
	    m_to < m_from && (m_from < newValue || newValue < m_to))
	{
		switch (m_mode)
		{
		case EMode::Once:
			pause();

			// clamp
			if (m_from <= m_to)
			{
				newValue = newValue > m_to ? m_to : m_from;
			}
			else
			{
				newValue = newValue < m_to ? m_to : m_from;
			}

			pulse(I3T_CYCLE_OUT_END);

			break;
		case EMode::Repeat:
			// New iteration.

			if (m_from <= m_to)
			{
				newValue = newValue > m_to ? m_from : m_to;
			}
			else
			{
				newValue = newValue < m_to ? m_from : m_to;
			}

			pulse(I3T_CYCLE_OUT_END);

			break;
		case EMode::PingPong:
			if (m_from <= m_to) // and out of the range <m_from, m_to> or <m_to, m_from>
			{
				newValue = newValue > m_to ? m_to : m_from;
			}
			else
			{
				newValue = newValue > m_from ? m_from : m_to;
			}

			m_directionMultiplier *= -1.0f;

			pulse(I3T_CYCLE_OUT_END);

			break;
		}
	}

	setInternalValue(newValue);
}
} // namespace Core
