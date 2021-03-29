#include "Cycle.h"
#include "GraphManager.h"

using namespace Core;

void Cycle::update(double seconds)
{
  if (m_isRunning)
  {
		float current = getData().getFloat();
		float newValue = current + ((float)seconds * m_multiplier);
		if (newValue > m_to)
    {
      newValue = fmod(newValue, m_updateStep);
		}
    setValue(newValue);
	}
}

void Cycle::play()
{
  m_isRunning = true;
}

void Cycle::stop()
{
  m_isRunning = false;
}

void Cycle::resetAndStop()
{

}

void Cycle::stepBack()
{

}

void Cycle::stepNext()
{

}

void Cycle::setFrom(float from)
{
	m_from = from;
}

void Cycle::setTo(float to)
{
	m_to = to;
}

void Cycle::setMultiplier(float v)
{
	m_updateStep = v;
}

void Cycle::setStep(float v)
{
	m_multiplier = v;
}

bool Cycle::isRunning()
{
	return m_isRunning;
}

float Cycle::getFrom()
{
	return m_from;
}

float Cycle::getTo()
{
	return m_to;
}

float Cycle::getMultiplier()
{
	return m_updateStep;
}

float Cycle::getStep()
{
	return m_multiplier;
}
