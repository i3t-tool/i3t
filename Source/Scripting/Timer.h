/**
 * \file
 * \brief
 * \author Martin Herich <martin.herich@phire.cz>
 * \copyright Copyright (C) 2016-2024 I3T team, Department of Computer Graphics
 * and Interaction, FEE, Czech Technical University in Prague, Czech Republic
 *
 * This file is part of I3T - An Interactive Tool for Teaching Transformations
 * http://www.i3t-tool.org
 *
 * GNU General Public License v3.0 (see LICENSE.txt or https://www.gnu.org/licenses/gpl-3.0.txt)
 */
#pragma once

#include <vector>

#include "Core/Defs.h"

class Timer
{
public:
	Timer() = default;
	Timer(uint64_t intervalMs, std::function<void(void)> callback)
	    : m_intervalMs(intervalMs), m_callback(std::move(callback))
	{}

	void tick(double dt)
	{
		if (m_lastTickMs >= m_intervalMs)
		{
			m_lastTickMs = 0;
			m_callback();
		}
		else
		{
			m_lastTickMs += (uint64_t) (dt * 1000);
		}
	}

private:
	uint64_t m_intervalMs = 0;
	uint64_t m_lastTickMs = 0;
	std::function<void(void)> m_callback;
};

class Chronos
{
public:
	void update(double deltaSeconds)
	{
		for (auto& timer : m_timers)
		{
			timer->tick(deltaSeconds);
		}
	}

	Ptr<Timer> setTimer(uint64_t intervalMs, std::function<void(void)> callback)
	{
		auto timer = std::make_shared<Timer>(intervalMs, std::move(callback));
		m_timers.emplace_back(timer);

		return timer;
	}

	void clearTimer(Ptr<Timer> timer)
	{
		auto it = std::find(m_timers.begin(), m_timers.end(), timer);
		if (it != m_timers.end())
		{
			m_timers.erase(it);
		}
	}

private:
	std::vector<Ptr<Timer>> m_timers;
};
