/**
 * \file AppLoopManager.cpp
 * \brief Manages the application main loop timing, including frame rate limiting.
 * \author Matvei Korikov <korikmat@fel.cvut.cz>
 * \copyright Copyright (C) 2016-2025 I3T team, Department of Computer Graphics
 * and Interaction, FEE, Czech Technical University in Prague, Czech Republic
 *
 * This file is part of I3T - An Interactive Tool for Teaching Transformations
 * http://www.i3t-tool.org
 *
 * Licensed under the GNU General Public License v3.0 (see LICENSE.txt or
 * https://www.gnu.org/licenses/gpl-3.0.txt)
 */

#ifdef _WIN32
#include <windows.h>
#pragma comment(lib, "winmm.lib")

/**
 * \brief Custom unhandled exception filter for Windows to restore the timer resolution.
 * \param exceptionPointers Pointer to exception information (unused).
 * \return EXCEPTION_CONTINUE_SEARCH to allow other handlers to run.
 */
LONG WINAPI AppUnhandledExceptionFilter(EXCEPTION_POINTERS* /*exceptionPointers*/)
{
	timeEndPeriod(1);
	return EXCEPTION_CONTINUE_SEARCH;
}
#endif

#include "AppLoopManager.h"
#include <thread>

AppLoopManager::AppLoopManager()
{
	init();
}

AppLoopManager::AppLoopManager(bool vsync, bool shouldLimitFPS, int targetFPS, bool shouldLimitFPSOnIdle,
                               int targetFPSOnIdle, int secondsBeforeIdle)
    : m_vsync(vsync), m_shouldLimitFPS(shouldLimitFPS), m_targetFPS(targetFPS),
      m_shouldLimitFPSOnIdle(shouldLimitFPSOnIdle), m_targetFPSOnIdle(targetFPSOnIdle),
      m_secondsBeforeIdle(secondsBeforeIdle)
{
	init();
}

void AppLoopManager::init()
{
	m_frameDuration = duration(1.0 / m_targetFPS);
	m_frameDurationOnIdle = duration(1.0 / m_targetFPSOnIdle);
	m_nextFrameTime = clock::now() + m_frameDuration;
	m_nextFrameTimeOnIdle = clock::now() + m_frameDurationOnIdle;
	m_lastTime = clock::now();

#ifdef _WIN32
	// Set custom exception filter to restore timer resolution
	SetUnhandledExceptionFilter(AppUnhandledExceptionFilter);
	timeBeginPeriod(1);
#endif
}

AppLoopManager::~AppLoopManager()
{
#ifdef _WIN32
	timeEndPeriod(1);
#endif
}

void AppLoopManager::startFrame()
{
	auto now = clock::now();
	m_deltaTime = now - m_lastTime;
	m_lastTime = now;
}

void AppLoopManager::sleepUntilNextFrame()
{
	std::this_thread::sleep_until(m_nextFrameTime);

	m_nextFrameTime += m_frameDuration;

	// Catch up if we are behind schedule
	while (clock::now() > m_nextFrameTime)
	{
		m_nextFrameTime += m_frameDuration;
	}
}

void AppLoopManager::sleepForNextFrame()
{
	auto remaining = m_nextFrameTime - clock::now();

	if (remaining > duration::zero())
	{
		std::this_thread::sleep_for(remaining);
	}

	m_nextFrameTime += m_frameDuration;

	// Catch up if lateness occurred
	while (clock::now() > m_nextFrameTime)
	{
		m_nextFrameTime += m_frameDuration;
	}
}

void AppLoopManager::setTargetFPS(int fps)
{
	if (m_targetFPS == fps)
		return;

	m_targetFPS = fps;
	m_frameDuration = duration(1.0 / m_targetFPS);
}

void AppLoopManager::setTargetFPSOnIdle(int fps)
{
	if (m_targetFPSOnIdle == fps)
		return;

	m_targetFPSOnIdle = fps;
	m_frameDurationOnIdle = duration(1.0 / m_targetFPSOnIdle);
}

void AppLoopManager::setSecondsBeforeIdle(int seconds)
{
	if (m_secondsBeforeIdle == seconds)
		return;

	m_secondsBeforeIdle = seconds;
}

double AppLoopManager::getFrameDurationOnIdle()
{
	if (!m_shouldLimitFPSOnIdle)
		return 0.0;

	static auto elapsedTime = duration(0.0);
	elapsedTime += m_deltaTime;

	auto now = clock::now();
	if (m_nextFrameTimeOnIdle - now > m_frameDurationOnIdle)
	{
		elapsedTime = duration(0.0);
		m_nextFrameTimeOnIdle = now + m_frameDurationOnIdle;
	}

	if (elapsedTime < std::chrono::seconds{m_secondsBeforeIdle})
		return 0.0;

	auto remaining = m_nextFrameTimeOnIdle - now;
	m_nextFrameTimeOnIdle += m_frameDurationOnIdle;

	// Catch up if behind schedule
	while (now > m_nextFrameTimeOnIdle)
	{
		m_nextFrameTimeOnIdle += m_frameDurationOnIdle;
	}

	return (remaining < duration::zero()) ? 0.0 : (remaining.count() / 1e9); // Convert nanoseconds to seconds
}