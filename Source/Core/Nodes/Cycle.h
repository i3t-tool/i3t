#pragma once

#include "Core/Nodes/Node.h"

namespace Core
{
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

private:
	float m_from = 0.0f;
	float m_to = 10.0f;
	float m_manualStep = 0.1f; //< step after pressing of Prev or Next button
	float m_multiplier = 0.1f; //< current step for one tick + sign represents the step direction

	float m_directionMultiplier = 1.0f; //< reverse the increment if (from > to)
	                                    // and flip in the PingPong mode

	bool m_isRunning = false;

	EMode m_mode = EMode::Repeat;

public:
	Cycle() : Node(&g_CycleProperties) {}

	Ptr<Node> clone() override;

	void update(double seconds); //< seconds means time delta from the last update

	void play();
	void pause();
	void stopAndReset();
	void stepBack();
	void stepNext();

	EMode getMode() { return m_mode; }
	void setMode(EMode mode)
	{
		if (m_mode != mode)
		{ // \todo A better variant - use value changed in WorkspaceCycle.cpp
			m_mode = mode;
			m_directionMultiplier = 1.0f;
		}
	}

	void onCycleFinish();

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
	void setMultiplier(float v);

	/**
	 * \param v increment added to/subtracted from the cycle value after user
	 * action - click to Next/Prev button
	 */
	void setManualStep(float v);

	void updateValues(int inputIndex) override; //< update inner state from connected
	                                            // inputs (values and pulse inputs)

private:
	void updateValue(float increment);
};

FORCE_INLINE bool isCycle(const Ptr<Node>& node) { return node->getOperation()->keyWord == g_CycleProperties.keyWord; }
} // namespace Core
