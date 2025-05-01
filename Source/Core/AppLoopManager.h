/**
 * \file AppLoopManager.h
 * \brief Declaration of AppLoopManager: handles application loop timing, FPS limits, and VSync status.
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

#pragma once

#include "Logger/Logger.h"
#include <chrono>

/**
 * \class AppLoopManager
 * \brief Manages the main application loop timing, including frame rate limiting and vertical sync status.
 *
 * Provides methods to start frames, sleep until or for the next frame, and adjust timing parameters
 * such as target FPS for active and idle modes.
 */
class AppLoopManager
{
	using clock = std::chrono::steady_clock;        ///< Clock type for high-resolution timing.
	using duration = std::chrono::duration<double>; ///< Duration type in seconds (double precision).

public:
	/**
	 * \brief Default constructor.
	 *
	 * Initializes manager with default settings (VSync enabled, 60 FPS active, 20 FPS idle).
	 */
	AppLoopManager();

	/**
	 * \brief Parameterized constructor.
	 *
	 * Creates manager with custom vsync and frame rate limiting settings.
	 *
	 * \param vsync Enable (true) or disable (false) vertical synchronization.
	 * \param shouldLimitFPS Enable (true) or disable (false) active frame rate limiting.
	 * \param targetFPS Target frames per second when active.
	 * \param shouldLimitFPSOnIdle Enable (true) or disable (false) idle frame rate limiting.
	 * \param targetFPSOnIdle Target frames per second when idle.
	 */
	AppLoopManager(bool vsync, bool shouldLimitFPS, int targetFPS, bool shouldLimitFPSOnIdle, int targetFPSOnIdle);

private:
	/**
	 * \brief Common initialization logic.
	 *
	 * Calculates initial frame durations and timestamps, applies VSync and platform-specific settings.
	 */
	void init();

public:
	/**
	 * \brief Destructor.
	 *
	 * Cleans up any platform-specific timing settings.
	 */
	~AppLoopManager();

	/**
	 * \brief Marks the start of a new frame.
	 *
	 * Updates \c m_deltaTime to the elapsed time since the last frame.
	 */
	void startFrame();

	/**
	 * \brief Sleeps until the next scheduled frame time.
	 *
	 * Uses sleep_until and adjusts the next frame timestamp to maintain a consistent frame rate.
	 */
	void sleepUntilNextFrame();

	/**
	 * \brief Sleeps for the remaining time until the next frame.
	 *
	 * Uses sleep_for and corrects for any timing drift.
	 */
	void sleepForNextFrame();

	/**
	 * \brief Retrieves the time elapsed during the last frame.
	 * \return Delta time in seconds.
	 */
	[[nodiscard]] double getDeltaTime() const
	{
		return m_deltaTime.count();
	}

	/**
	 * \brief Gets the current target FPS for active mode.
	 * \return Active mode frames per second.
	 */
	[[nodiscard]] int getTargetFPS() const
	{
		return m_targetFPS;
	}

	/**
	 * \brief Sets a new target FPS for active mode.
	 * \param fps Desired active FPS.
	 */
	void setTargetFPS(int fps);

	/**
	 * \brief Returns whether vertical sync is enabled.
	 * \return true if VSync is enabled, false otherwise.
	 */
	[[nodiscard]] bool isVsync() const
	{
		return m_vsync;
	}

	/**
	 * \brief Set VSync status, does not affect on real vsync settings.
	 * \param enable true to set enable VSync status, false to disable.
	 */
	void setVSync(bool enable)
	{
		if (m_vsync != enable)
		{
			m_vsync = enable;
		}
	}

	/**
	 * \brief Checks if active frame limiting is engaged.
	 * \return true if FPS limiting is enabled and VSync is off.
	 */
	[[nodiscard]] bool shouldLimitFPS() const
	{
		return m_shouldLimitFPS && !m_vsync;
	}

	/**
	 * \brief Enables or disables active FPS limiting.
	 * \param enable true to enable limiting, false to disable.
	 */
	void enableFPSLimit(bool enable)
	{
		if (m_shouldLimitFPS != enable)
			m_shouldLimitFPS = enable;
	}

	/**
	 * \brief Checks if idle frame limiting is enabled.
	 * \return true if idle FPS limiting is enabled.
	 */
	[[nodiscard]] bool shouldLimitFPSOnIdle() const
	{
		return m_shouldLimitFPSOnIdle;
	}

	/**
	 * \brief Enables or disables idle FPS limiting.
	 * \param enable true to enable, false to disable.
	 */
	void setShouldLimitFPSOnIdle(bool enable)
	{
		if (m_shouldLimitFPSOnIdle != enable)
			m_shouldLimitFPSOnIdle = enable;
	}

	/**
	 * \brief Sets a new target FPS for idle mode.
	 * \param fps Desired idle FPS.
	 */
	void setTargetFPSOnIdle(int fps);

	/**
	 * \brief Gets the current target FPS for idle mode.
	 * \return Idle mode frames per second.
	 */
	[[nodiscard]] int getTargetFPSOnIdle() const
	{
		return m_targetFPSOnIdle;
	}

	/**
	 * \brief Calculates time remaining until next idle frame.
	 *
	 * Updates internal timestamp and returns remaining seconds.
	 * \return Seconds until next idle frame, or 0.0 if limiting is disabled or overdue.
	 */
	[[nodiscard]] double getFrameDurationOnIdle();

private:
	bool m_vsync = true;           ///< Vertical sync enabled flag.
	bool m_shouldLimitFPS = false; ///< Active FPS limiting flag.
	int m_targetFPS{60};           ///< Target FPS when active.

	duration m_frameDuration{0};                              ///< Duration between active frames.
	std::chrono::time_point<clock, duration> m_nextFrameTime; ///< Next timestamp for active frame.
	std::chrono::time_point<clock> m_lastTime;                ///< Timestamp of last frame start.
	duration m_deltaTime{0};                                  ///< Time difference between last two frames.

	bool m_shouldLimitFPSOnIdle = true;                             ///< Idle FPS limiting flag.
	int m_targetFPSOnIdle{20};                                      ///< Target FPS when idle.
	duration m_frameDurationOnIdle{0};                              ///< Duration between idle frames.
	std::chrono::time_point<clock, duration> m_nextFrameTimeOnIdle; ///< Next timestamp for idle frame.
};
