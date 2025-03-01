/**
 * \file
 * \brief Provides a set of static methods and data for collecting and managing application statistics.
 * \author Matvei Korikov <korikmat@fel.cvut.cz>
 * \copyright Copyright (C) 2016-2025 I3T team, Department of Computer Graphics
 * and Interaction, FEE, Czech Technical University in Prague, Czech Republic
 *
 * This file is part of I3T - An Interactive Tool for Teaching Transformations
 * http://www.i3t-tool.org
 *
 * GNU General Public License v3.0 (see LICENSE.txt or https://www.gnu.org/licenses/gpl-3.0.txt)
 */

#pragma once

#include <map>
#include <memory>
#include <string>
#include <vector>

#include "Timers.h"

/**
 * \class Statistics
 * \brief Static utility class for tracking performance metrics such as FPS, timers, and frame details.
 *
 * The `Statistics` class provides static members and methods to track and manage application performance metrics.
 * It includes nested classes for frame-specific and GPU-specific statistics.
 */
class Statistics
{
public:
	/**
	 * \brief Deleted constructor to prevent instantiation.
	 */
	Statistics() = delete;

	/**
	 * \brief Deleted destructor to prevent destruction.
	 */
	~Statistics() = delete;

	/**
	 * \brief Current frames per second.
	 */
	static float FPS;

	/**
	 * \brief Time elapsed between frames, in seconds.
	 */
	static float DeltaTime;

	/**
	 * \brief Map of all active timers.
	 * \details Each timer is identified by a string name.
	 */
	static std::map<std::string, TimerPtr> CPUTimers;
	static std::map<std::string, TimerPtr> GPUTimers;


	/**
	 * \brief Updates the internal state of the statistics system.
	 */
	static void update();

	/**
	 * \brief Starts a timer with the given name.
	 * \param name The name of the timer to start.
	 */
	static void startTimer(const std::string& name);

	/**
	 * \brief Stops a timer with the given name.
	 * \param name The name of the timer to stop.
	 */
	static void stopTimer(const std::string& name);

	static void startGPUTimer(const std::string& name);

	static void stopGPUTimer(const std::string& name);

	/**
	 * \class Frame
	 * \brief Nested class for frame-specific statistics.
	 */
	class Frame
	{
	public:
		/**
		 * \brief Deleted constructor to prevent instantiation.
		 */
		Frame() = delete;

		/**
		 * \brief Deleted destructor to prevent destruction.
		 */
		~Frame() = delete;

		/**
		 * \brief GPU frame time, in milliseconds.
		 */
		static int GPUTime;

		/**
		 * \brief CPU frame time, in milliseconds.
		 */
		static int CPUTime;

		/**
		 * \brief Identifier of the current frame.
		 */
		static int ID;
	};

	/**
	 * \class GPU
	 * \brief Nested class for GPU-specific statistics.
	 */
	class GPU
	{
	public:
		/**
		 * \brief Deleted constructor to prevent instantiation.
		 */
		GPU() = delete;

		/**
		 * \brief Deleted destructor to prevent destruction.
		 */
		~GPU() = delete;

		/**
		 * \brief Size of dedicated GPU memory, in megabytes.
		 */
		static int DedicatedMemory;

		/**
		 * \brief Total size of GPU memory, in megabytes.
		 */
		static int TotalMemory;

		/**
		 * \brief Size of allocated GPU memory, in megabytes.
		 */
		static int AllocatedMemory;

		/**
		 * \brief Size of available GPU memory, in megabytes.
		 */
		static int FreeMemory;

		/**
		 * \brief Size of evicted GPU memory, in megabytes.
		 */
		static int EvictedMemory;
	};
};
