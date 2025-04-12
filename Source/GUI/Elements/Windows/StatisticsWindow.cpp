/**
 * \file
 * \brief Implementation of the StatisticsWindow class for rendering performance and GPU statistics in the GUI.
 * \author Matvei Korikov <korikmat@fel.cvut.cz>
 * \copyright Copyright (C) 2016-2025 I3T team, Department of Computer Graphics
 * and Interaction, FEE, Czech Technical University in Prague, Czech Republic
 *
 * This file is part of I3T - An Interactive Tool for Teaching Transformations
 * http://www.i3t-tool.org
 *
 * GNU General Public License v3.0 (see LICENSE.txt or https://www.gnu.org/licenses/gpl-3.0.txt)
 */
#include "StatisticsWindow.h"

#include "Commands/ApplicationCommands.h"
#include "GUI/Fonts/Icons.h"
#include "GUI/WindowManager.h"
#include "Localization/Localization.h"
#include "Utils/Statistics.h"

#include "GUI/Toolkit.h"
#include "imgui.h"

#include <imgui_internal.h>

/**
 * \brief Constructs a StatisticsWindow and initializes its title.
 */
StatisticsWindow::StatisticsWindow() : IWindow(ICON_T(ICON_I3T_DEBUG " ", "Statistics")) {}

/**
 * \brief Renders the statistics window with performance and GPU data.
 *
 * This method creates the GUI window, updates window information,
 * and draws its content. If the window is closed, it dispatches a
 * command to hide it.
 */
void StatisticsWindow::render()
{
	// Set the window size and create the GUI window
	ImVec2 windowSize = ImVec2(ImGui::GetFontSize() * 25, ImGui::GetFontSize() * 30);
	ImGui::SetNextWindowSize(windowSize, ImGuiCond_Once);
	GUI::dockTabStylePush();
	if (ImGui::Begin(getName(), getShowPtr()))
	{
		this->updateWindowInfo();
		GUI::dockTabStylePop();
		drawContent();
	}
	else
	{
		GUI::dockTabStylePop();
	}
	ImGui::End();

	// Dispatch a command to hide the window if it is no longer visible
	if (!isVisible())
	{
		HideWindowCommand::dispatch(ID);
	}
}

/**
 * \brief Draws the content of the statistics window.
 *
 * This method displays performance metrics such as FPS, frametime, timers,
 * and GPU memory usage using ImGui widgets.
 */
void StatisticsWindow::drawContent()
{
	// Display frame ID and FPS
	ImGui::Text(_t("Frame %d, FPS %.1f"), Statistics::Frame::ID, Statistics::FPS);

	char overlay[64];

	// Display and update the framerate plot
	static float framerates[500] = {};
	static int frameratesOffset = 0;
	static int maxFramerate = 0;
	static int maxFramerate200 = 0;
	maxFramerate = std::max<float>(maxFramerate, Statistics::FPS);
	maxFramerate200 = std::max<float>(maxFramerate200, Statistics::FPS);
	if (Statistics::Frame::ID % 200 == 0)
	{
		maxFramerate = maxFramerate200;
		maxFramerate200 = 0;
	}
	framerates[frameratesOffset] = Statistics::FPS;
	frameratesOffset = (frameratesOffset + 1) % IM_ARRAYSIZE(framerates);
	sprintf(overlay, _t("Framerate: %.3f"), Statistics::FPS);
	ImGui::PlotLines("", framerates, IM_ARRAYSIZE(framerates), frameratesOffset, overlay, 0.0f, maxFramerate * 2,
	                 ImVec2(-1.0f, IM_TRUNC(ImGui::GetFontSize() * 5.0f)));

	// Display and update the frametime plot
	static float deltaTimes[500] = {};
	static int deltaTimesOffset = 0;
	static int deltaMaxTime = 0;
	static int deltaMaxTime200 = 0;
	deltaMaxTime = std::max<float>(deltaMaxTime, Statistics::DeltaTime);
	deltaMaxTime200 = std::max<float>(deltaMaxTime200, Statistics::DeltaTime);
	if (Statistics::Frame::ID % 200 == 0)
	{
		deltaMaxTime = deltaMaxTime200;
		deltaMaxTime200 = 0;
	}
	deltaTimes[deltaTimesOffset] = Statistics::DeltaTime;
	deltaTimesOffset = (deltaTimesOffset + 1) % IM_ARRAYSIZE(deltaTimes);
	sprintf(overlay, _t("Frametime: %.3f"), Statistics::DeltaTime);
	ImGui::PlotLines("", deltaTimes, IM_ARRAYSIZE(deltaTimes), deltaTimesOffset, overlay, 0.0f, deltaMaxTime * 2,
	                 ImVec2(-1.0f, IM_TRUNC(ImGui::GetFontSize() * 5.0f)));

	// Display and update custom timers
	if (!Statistics::CPUTimers.empty())
	{
		ImGui::Text(_t("CPU Timers:"));
		for (auto& [name, timer] : Statistics::CPUTimers)
		{
			ImGui::Text("%f (avg. %f) %s", timer->get(), timer->getAverage(), name.c_str());

			ImGui::ProgressBar(timer->get() / Statistics::DeltaTime,
			                   ImVec2(-1.0f, IM_TRUNC(ImGui::GetFontSize() * 1.5f)));
		}
	}

	if (!Statistics::GPUTimers.empty())
	{
		ImGui::Text(_t("GPU Timers:"));
		for (auto& [name, timer] : Statistics::GPUTimers)
		{
			ImGui::Text("%f (avg. %f) %s", timer->get(), timer->getAverage(), name.c_str());
			ImGui::ProgressBar(timer->get() / Statistics::DeltaTime,
			                   ImVec2(-1.0f, IM_TRUNC(ImGui::GetFontSize() * 1.5f)));
		}
	}

	// Display GPU memory statistics
	ImGui::Text(_t("Total GPU memory %d MB"), Statistics::GPU::TotalMemory / 1024);
	ImGui::ProgressBar(Statistics::GPU::AllocatedMemory / static_cast<float>(Statistics::GPU::TotalMemory),
	                   ImVec2(IM_TRUNC(ImGui::GetFontSize() * 5.0f), 0.0f));
	ImGui::SameLine(0.0f, ImGui::GetStyle().ItemInnerSpacing.x);
	ImGui::Text(_t("%u MB alloc."), static_cast<unsigned int>(Statistics::GPU::AllocatedMemory / 1024));
	ImGui::ProgressBar(Statistics::GPU::FreeMemory / static_cast<float>(Statistics::GPU::TotalMemory),
	                   ImVec2(IM_TRUNC(ImGui::GetFontSize() * 5.0f), 0.0f));
	ImGui::SameLine(0.0f, ImGui::GetStyle().ItemInnerSpacing.x);
	ImGui::Text(_t("%u MB free."), static_cast<unsigned int>(Statistics::GPU::FreeMemory / 1024));
}