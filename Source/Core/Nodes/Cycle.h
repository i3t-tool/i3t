#pragma once

#include "Core/Nodes/Node.h"

namespace Core
{
namespace CycleInternals
{
const int in_from = 0;
const int in_to = 1;
const int in_multiplier = 2;
const int in_play = 3;
const int in_pause = 4;
const int in_stop = 5;
const int in_prev = 6;
const int in_next = 7;

const int out_val = 0;
const int out_play = 1;
const int out_pause = 2;
const int out_stop = 3;
const int out_prev = 4;
const int out_next = 5;
const int out_cycleEnd = 6;
} // namespace CycleInternals

class Cycle : public NodeBase
{
public:
	enum class EMode
	{
		Once,
		Repeat,
		PingPong
	};

private:
	float m_from = 0.0f;
	float m_to = 10.0f;
	float m_updateStep = 0.1f;
	float m_multiplier = 0.1f;

	float m_modeMultiplier = 1.0f;

	bool m_isRunning = false;

	EMode m_mode = EMode::Repeat;

public:
	Cycle() : NodeBase(&g_CycleProperties) {}
	void update(double s);

	void play();
	void stop();
	void resetAndStop();
	void stepBack();
	void stepNext();

	EMode getMode() { return m_mode; }
	void setMode(EMode mode) { m_mode = mode; }

	void onCycleFinish();

	bool isRunning() const;
	float getFrom() const;
	float getTo() const;
	float getMultiplier() const;
	float getStep() const;

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

	void updateValues(int inputIndex) override;

private:
	void updateValue(float seconds);

	void setModeMultiplier();
};
} // namespace Core
