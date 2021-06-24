#include "Cycle.h"
#include "GraphManager.h"

using namespace Core;

void Cycle::update(double seconds)
{
	if (m_isRunning)
	{
		updateValue(m_modeMultiplier * static_cast<float>(seconds) * m_multiplier);
	}
}

void Cycle::play()
{
	m_isRunning = true;
	spreadSignal(I3T_CYCLE_OUT_PLAY);
}

void Cycle::stop()
{
	m_isRunning = false;
	spreadSignal(I3T_CYCLE_OUT_PAUSE);
}

void Cycle::resetAndStop()
{
	m_isRunning = false;
	getInternalData().setValue(m_from);
	spreadSignal(I3T_CYCLE_OUT_STOP);
}

void Cycle::stepBack()
{
	updateValue((-1.0f) * m_modeMultiplier * m_updateStep);
	spreadSignal(I3T_CYCLE_OUT_PREV);
}

void Cycle::stepNext()
{
	updateValue(m_modeMultiplier * m_updateStep);
	spreadSignal(I3T_CYCLE_OUT_NEXT);
}

void Cycle::setFrom(float from)
{
	m_from = from;
	setModeMultiplier();
}

void Cycle::setTo(float to)
{
	m_to = to;
	setModeMultiplier();
}

void Cycle::setMultiplier(float v)
{
	m_multiplier = abs(v);
}

void Cycle::setStep(float v)
{
	m_updateStep = v;
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
	return m_updateStep;
}

float Cycle::getStep() const
{
	return m_multiplier;
}

void Cycle::updateValues(int inputIndex)
{
	if (m_inputs[I3T_CYCLE_IN_FROM].isPluggedIn())
		getInternalData(I3T_CYCLE_IN_FROM).setValue(m_inputs[I3T_CYCLE_IN_FROM].getStorage().getFloat());

	if (m_inputs[I3T_CYCLE_IN_TO].isPluggedIn())
		getInternalData(I3T_CYCLE_IN_TO).setValue(m_inputs[I3T_CYCLE_IN_TO].getStorage().getFloat());

	if (m_inputs[I3T_CYCLE_IN_MULT].isPluggedIn())
		getInternalData(I3T_CYCLE_IN_MULT).setValue(m_inputs[I3T_CYCLE_IN_MULT].getStorage().getFloat());

	switch (inputIndex)
	{
	case I3T_CYCLE_IN_PLAY:
		play();
		break;
	case I3T_CYCLE_IN_PAUSE:
		stop();
		break;
	case I3T_CYCLE_IN_STOP:
		resetAndStop();
		break;
	case I3T_CYCLE_IN_PREV:
		stepBack();
		break;
	case I3T_CYCLE_IN_NEXT:
		stepNext();
		break;
	default:
		break;
	}
}

void Cycle::onCycleFinish()
{
}

void Cycle::updateValue(float seconds)
{
	float current = getData().getFloat();
	float newValue = current + ((float)seconds);

	if (m_from < m_to && (m_to < newValue || newValue < m_from) ||
	    m_to < m_from && (m_from < newValue || newValue < m_to))
	{
		switch (m_mode)
		{
		case EMode::Once:
			stop();
      if (m_modeMultiplier == 1.0f)
      {
        newValue = newValue > m_to ? m_to : m_from;
      }
      else if (m_modeMultiplier == -1.0f)
      {
        newValue = newValue < m_to ? m_to : m_from;
      }
			break;
		case EMode::Repeat:
			// New iteration.
			// newValue = m_from < m_to ? m_from : m_to; // + fmod(newValue, m_updateStep);
			if (m_modeMultiplier == 1.0f)
      {
				newValue = newValue > m_to ? m_from : m_to;
			}
			else if (m_modeMultiplier == -1.0f)
      {
        newValue = newValue < m_to ? m_from : m_to;;
			}
			break;
		case EMode::PingPong:
			m_modeMultiplier *= -1.0f;
			break;
		}
	}
	getInternalData().setValue(newValue);
}

void Cycle::setModeMultiplier()
{
	if (m_to < m_from)
		m_modeMultiplier = -1.0f;
	else
		m_modeMultiplier = 1.0f;
}
