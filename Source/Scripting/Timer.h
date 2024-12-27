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

#define SOL_ALL_SAFETIES_ON 1
#include "sol/sol.hpp"

#include "Core/Defs.h"

class Timer
{
	friend class Chronos;

public:
	Timer() = default;
	Timer(double intervalSeconds, sol::protected_function callback, bool oneShot = false)
	    : m_intervalSeconds(intervalSeconds), m_callback(std::move(callback)), m_oneShot(oneShot)
	{}

	/// \param dt in seconds
	void tick(double dt)
	{
		if (m_lastTickSeconds >= m_intervalSeconds)
		{
			m_lastTickSeconds = 0;
			m_callback();

			if (m_oneShot)
			{
				m_isExpired = true;
			}
		}
		else
		{
			m_lastTickSeconds += dt;
		}
	}

private:
	double m_intervalSeconds = 0;
	double m_lastTickSeconds = 0;
	sol::protected_function m_callback;
	bool m_oneShot = false;
	bool m_isExpired = false;
};

class Chronos
{
public:
	void update(double deltaSeconds)
	{
		clearPendingTimers();

		for (auto& timer : m_timers)
		{
			timer->tick(deltaSeconds);

			if (timer->m_isExpired)
			{
				m_timersToDelete.emplace_back(timer);
			}
		}
	}

	Ptr<Timer> setTimer(double intervalSeconds, sol::protected_function callback)
	{
		auto timer = std::make_shared<Timer>(intervalSeconds, std::move(callback));
		m_timers.emplace_back(timer);

		return timer;
	}

	Ptr<Timer> setTimeout(double intervalSeconds, sol::protected_function callback)
	{
		auto timer = std::make_shared<Timer>(intervalSeconds, std::move(callback), true);
		m_timers.emplace_back(timer);

		return timer;
	}

	void clearTimer(Ptr<Timer> timer)
	{
		m_timersToDelete.emplace_back(timer);
	}

	void clearTimers()
	{
		m_timersToDelete = m_timers;
	}

private:
	void clearPendingTimers()
	{
		while (!m_timersToDelete.empty())
		{
			auto timer = m_timersToDelete.back();
			auto it = std::find(m_timers.begin(), m_timers.end(), timer);
			if (it != m_timers.end())
			{
				m_timers.erase(it);
			}

			m_timersToDelete.pop_back();
		}
	}

	std::vector<Ptr<Timer>> m_timers;
	std::vector<Ptr<Timer>> m_timersToDelete;
};
