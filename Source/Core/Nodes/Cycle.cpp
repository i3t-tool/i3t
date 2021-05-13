#include "Cycle.h"
#include "GraphManager.h"

using namespace Core;
using namespace CycleInternals;

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
	spreadSignal(out_play);
}

void Cycle::stop()
{
	m_isRunning = false;
	spreadSignal(out_pause);
}

void Cycle::resetAndStop()
{
	m_isRunning = false;
	getInternalData().setValue(m_from);
	spreadSignal(out_stop);
}

void Cycle::stepBack()
{
	updateValue((-1.0f) * m_modeMultiplier * m_updateStep);
	spreadSignal(out_prev);
}

void Cycle::stepNext()
{
	updateValue(m_modeMultiplier * m_updateStep);
	spreadSignal(out_next);
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
	if (m_inputs[in_from].isPluggedIn())
		getInternalData(in_from).setValue(m_inputs[in_from].getStorage().getFloat());

	if (m_inputs[in_to].isPluggedIn())
		getInternalData(in_to).setValue(m_inputs[in_to].getStorage().getFloat());

	if (m_inputs[in_multiplier].isPluggedIn())
		getInternalData(in_multiplier).setValue(m_inputs[in_multiplier].getStorage().getFloat());

	switch (inputIndex)
	{
	case in_play:
		play();
		break;
	case in_pause:
		stop();
		break;
	case in_stop:
		resetAndStop();
		break;
	case in_prev:
		stepBack();
		break;
	case in_next:
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
