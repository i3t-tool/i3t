#pragma once

#include "Core/Nodes/Node.h"

namespace Core
{
inline constexpr size_t I3T_CYCLE_IN_FROM  = 0;
inline constexpr size_t I3T_CYCLE_IN_TO		 = 1;
inline constexpr size_t I3T_CYCLE_IN_MULT  = 2;
inline constexpr size_t I3T_CYCLE_IN_PLAY  = 3;
inline constexpr size_t I3T_CYCLE_IN_PAUSE = 4;
inline constexpr size_t I3T_CYCLE_IN_STOP	 = 5;
inline constexpr size_t I3T_CYCLE_IN_PREV	 = 6;
inline constexpr size_t I3T_CYCLE_IN_NEXT	 = 7;

inline constexpr size_t I3T_CYCLE_OUT_VAL   = 0;
inline constexpr size_t I3T_CYCLE_OUT_PLAY  = 1;
inline constexpr size_t I3T_CYCLE_OUT_PAUSE = 2;
inline constexpr size_t I3T_CYCLE_OUT_STOP	= 3;
inline constexpr size_t I3T_CYCLE_OUT_PREV	= 4;
inline constexpr size_t I3T_CYCLE_OUT_NEXT	= 5;
inline constexpr size_t I3T_CYCLE_OUT_END		= 6;

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
