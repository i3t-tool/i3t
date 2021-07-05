#include "Cycle.h"
#include "GraphManager.h"

using namespace Core;

void Cycle::update(double seconds)
{
	if (m_isRunning)
	{
		updateValue( static_cast<float>(seconds) * m_multiplier);  // smooth step
	}
}

void Cycle::play()
{
	float currentValue = getData().getFloat();

	if(m_mode == EMode::Once) {
		//revind to start (to m_from) after stop at m_to
		if ( (m_from <= m_to && currentValue >= m_to) || (m_from > m_to && currentValue <= m_to) )
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
	setInternalValue(m_from); //PF was missing
	
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

void Cycle::setMultiplier(float v)  //\todo PF Change to setStep
{
	m_multiplier = abs(v);
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
	return m_multiplier;
}

float Cycle::getManualStep() const
{
	return m_manualStep;
}

void Cycle::updateValues(int inputIndex)
{
	if (m_inputs[I3T_CYCLE_IN_FROM].isPluggedIn())
	{
		float val = getIn(I3T_CYCLE_IN_FROM).getStorage().getFloat();
		setFrom(val);
		setInternalValue(val, I3T_CYCLE_IN_FROM);
	}

	if (getIn(I3T_CYCLE_IN_TO).isPluggedIn())
	{
		float val = getIn(I3T_CYCLE_IN_TO).getStorage().getFloat();
		setTo(val);
		setInternalValue(val, I3T_CYCLE_IN_TO);
	}

	if (m_inputs[I3T_CYCLE_IN_MULT].isPluggedIn())
	{
		float val = getIn(I3T_CYCLE_IN_MULT).getStorage().getFloat();
		setMultiplier(val);
		setInternalValue(val, I3T_CYCLE_IN_MULT);
	}

	if (getIn(I3T_CYCLE_IN_PLAY).isPluggedIn() && shouldPulse(I3T_CYCLE_IN_PLAY, inputIndex))
	{
		play();
	}
	else if (getIn(I3T_CYCLE_IN_PAUSE).isPluggedIn() && shouldPulse(I3T_CYCLE_IN_PAUSE, inputIndex))
	{
		pause();
	}
	if (getIn(I3T_CYCLE_IN_STOP).isPluggedIn() && shouldPulse(I3T_CYCLE_IN_STOP, inputIndex))
	{
		stopAndReset();
	}
	if (getIn(I3T_CYCLE_IN_PREV).isPluggedIn() && shouldPulse(I3T_CYCLE_IN_PREV, inputIndex))
	{
		stepBack();
	}
	if (getIn(I3T_CYCLE_IN_NEXT).isPluggedIn() && shouldPulse(I3T_CYCLE_IN_NEXT, inputIndex))
	{
		stepNext();
	}
}

void Cycle::onCycleFinish()  // \todo not used => remove?
{
}

void Cycle::updateValue(float increment)
{
	const float currentValue = getData().getFloat();
	float newValue = currentValue + ((m_from <= m_to) ? 1.0f : -1.0f) * m_directionMultiplier * increment;
	

	// if out of bounds, clamp values to the range <m_from, m_to> or <m_to, m_from>
	// if(newValue < std::min(m_from, m_to) ||  newValue > std::max(m_from, m_to) )  // probably more readable
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
			break;
		case EMode::Repeat:
			// New iteration.
			// newValue = m_from < m_to ? m_from : m_to; // + fmod(newValue, m_manualStep);

	    if (m_from <= m_to)
      {
				newValue = newValue > m_to ? m_from : m_to;
			}
			else
      {
        newValue = newValue < m_to ? m_from : m_to;;
			}
			break;
		case EMode::PingPong:
			
			//fprintf(stdout, "DirectionMultiplier =%3.f \n",m_directionMultiplier);
			
			if (m_from <= m_to) // and out of the range <m_from, m_to> or <m_to, m_from>
			{
				newValue = newValue > m_to ? m_to : m_from;
			}
			else
			{
				newValue = newValue > m_from ? m_from : m_to;
			}

			m_directionMultiplier *= -1.0f;
			
			break;
		}
	}
	
	setInternalValue(newValue);
}

