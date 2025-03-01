/**
 * \file
 * \brief Implementation of the Statistics class for collecting and managing performance metrics.
 * \author Matvei Korikov <korikmat@fel.cvut.cz>
 * \copyright Copyright (C) 2016-2025 I3T team, Department of Computer Graphics
 * and Interaction, FEE, Czech Technical University in Prague, Czech Republic
 *
 * This file is part of I3T - An Interactive Tool for Teaching Transformations
 * http://www.i3t-tool.org
 *
 * GNU General Public License v3.0 (see LICENSE.txt or https://www.gnu.org/licenses/gpl-3.0.txt)
 */

#include "Statistics.h"
#include "GLFW/glfw3.h"
#include "imgui.h"

// Initialize static members
float Statistics::FPS = 0.0f;
float Statistics::DeltaTime = 0.0f;
std::map<std::string, TimerPtr> Statistics::CPUTimers = {};
std::map<std::string, TimerPtr> Statistics::GPUTimers = {};

int Statistics::Frame::GPUTime = 0;
int Statistics::Frame::CPUTime = 0;
int Statistics::Frame::ID = 0;

int Statistics::GPU::DedicatedMemory = 0;
int Statistics::GPU::TotalMemory = 0;
int Statistics::GPU::AllocatedMemory = 0;
int Statistics::GPU::FreeMemory = 0;
int Statistics::GPU::EvictedMemory = 0;

/**
 * \brief Updates statistics including frame ID, FPS, and GPU memory usage.
 *
 * This function increments the frame ID and updates FPS and DeltaTime values
 * using ImGui's IO metrics. Additionally, it checks for GPU memory info support
 * and retrieves memory usage details if the necessary OpenGL extension is available.
 */
void Statistics::update()
{
	Frame::ID++; // Increment frame ID

	static float lastTime = glfwGetTime(); // Track time of the last update
	float currentTime = glfwGetTime();     // Get current time

	static bool isGPUInfoSupported = glfwExtensionSupported("GL_NVX_gpu_memory_info") == GL_TRUE;

	// Update GPU memory statistics every second
	if (currentTime - lastTime >= 1.0f)
	{
		lastTime = currentTime;
		if (isGPUInfoSupported)
		{
			glGetIntegerv(GL_GPU_MEMORY_INFO_CURRENT_AVAILABLE_VIDMEM_NVX, &GPU::FreeMemory);
			glGetIntegerv(GL_GPU_MEMORY_INFO_EVICTED_MEMORY_NVX, &GPU::EvictedMemory);
			glGetIntegerv(GL_GPU_MEMORY_INFO_DEDICATED_VIDMEM_NVX, &GPU::DedicatedMemory);
			glGetIntegerv(GL_GPU_MEMORY_INFO_TOTAL_AVAILABLE_MEMORY_NVX, &GPU::TotalMemory);
			GPU::AllocatedMemory = GPU::TotalMemory - GPU::FreeMemory;
		}
	}

	FPS = ImGui::GetIO().Framerate;                 // Update FPS
	DeltaTime = 1000.0f * ImGui::GetIO().DeltaTime; // Update DeltaTime in milliseconds
}

/**
 * \brief Starts a CPU timer for the given name.
 * \param name The name of the timer to start.
 *
 * If a timer with the specified name does not exist, it is created and started.
 */
void Statistics::startTimer(const std::string& name)
{
	if (!CPUTimers.contains(name))
	{
		CPUTimers[name] = std::make_shared<CPUTimer>(name); // Create new timer
	}
	CPUTimers[name]->start(); // Start the timer
}

/**
 * \brief Stops a CPU timer for the given name.
 * \param name The name of the timer to stop.
 *
 * If a timer with the specified name exists, it is stopped.
 */
void Statistics::stopTimer(const std::string& name)
{
	if (CPUTimers.contains(name))
	{
		CPUTimers[name]->stop(); // Stop the timer
	}
}

/**
 * \brief Starts a GPU timer for the given name.
 * \param name The name of the timer to start.
 *
 * If a timer with the specified name does not exist, it is created and started.
 */
void Statistics::startGPUTimer(const std::string& name)
{
	if (!GPUTimers.contains(name))
	{
		GPUTimers[name] = std::make_shared<GPUTimer>(name); // Create new timer
	}
	GPUTimers[name]->start(); // Start the timer
}

/**
 * \brief Stops a GPU timer for the given name.
 * \param name The name of the timer to stop.
 *
 * If a timer with the specified name exists, it is stopped.
 */
void Statistics::stopGPUTimer(const std::string& name)
{
	if (GPUTimers.contains(name))
	{
		GPUTimers[name]->stop(); // Stop the timer
	}
}