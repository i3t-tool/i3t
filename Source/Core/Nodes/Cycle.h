#pragma once

#include "Core/Nodes/Node.h"

namespace Core
{
class Cycle : public NodeBase
{
	float m_from = 0.0f;
	float m_to = 0.0f;
	float m_updateStep = 0.0f;
	float m_multiplier = 0.1f;

	bool m_isRunning = false;

public:
	Cycle() : NodeBase(&g_CycleProperties) {}
  void update(double s);

	void play();
	void stop();
	void resetAndStop();
	void stepBack();
	void stepNext();

	bool isRunning();
	float getFrom();
	float getTo();
	float getMultiplier();
	float getStep();

	/**
	 * \param from in seconds
	 */
	void setFrom(float from);

  /**
   * \param from in seconds
   */
	void setTo(float to);

  /**
   * \param from in seconds
   */
	void setMultiplier(float v);

  /**
   * \param from in seconds
   */
	void setStep(float v);
};
}
