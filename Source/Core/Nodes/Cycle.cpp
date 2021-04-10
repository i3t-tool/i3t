#include "Cycle.h"
#include "GraphManager.h"

using namespace Core;
using namespace CycleInternals;

void Cycle::update(double seconds)
{
  if (m_isRunning)
  {
		updateValue(static_cast<float>(seconds) * m_multiplier);
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
  getInternalData().setValue(0.0f);
  spreadSignal(out_stop);
}

void Cycle::stepBack()
{
  updateValue(-m_updateStep);
  spreadSignal(out_prev);
}

void Cycle::stepNext()
{
  updateValue(m_updateStep);
  spreadSignal(out_next);
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
	if (m_inputs[in_from].isPluggedIn()) getInternalData(in_from).setValue(m_inputs[in_from].getStorage().getFloat());

  if (m_inputs[in_to].isPluggedIn()) getInternalData(in_to).setValue(m_inputs[in_to].getStorage().getFloat());

  if (m_inputs[in_multiplier].isPluggedIn()) getInternalData(in_multiplier).setValue(m_inputs[in_multiplier].getStorage().getFloat());

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
  if (newValue > m_to)
  {
    // New iteration.
    newValue = fmod(newValue, m_updateStep);
  }
  getInternalData().setValue(newValue);
}
