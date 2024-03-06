/**
 * \file
 * \brief
 * \authors Martin Herich <martin.herich@phire.cz>, Dan Rakušan <rakusan.dan@gmail.com>
 * \copyright Copyright (C) 2016-2023 I3T team, Department of Computer Graphics
 * and Interaction, FEE, Czech Technical University in Prague, Czech Republic
 *
 * This file is part of I3T - An Interactive Tool for Teaching Transformations
 * http://www.i3t-tool.org
 *
 * GNU General Public License v3.0 (see LICENSE.txt or https://www.gnu.org/licenses/gpl-3.0.txt)
 */
#include "IWindow.h"

#define IMGUI_DEFINE_MATH_OPERATORS
#include "imgui.h"

#include "GUI/Toolkit.h"
#include "GUI/WindowManager.h"
#include "Logger/Logger.h"

const char* IWindow::getName() const
{
	return imGuiName.c_str();
};

const std::string& IWindow::setName(const char* name)
{
	imGuiName = fmt::format("{}###{}", name, getID());

	return imGuiName;
}

void IWindow::updateWindowInfo()
{
	ImGuiIO io = ImGui::GetIO();

	// This returns slightly different results than GetWindowContentRegionMin, better to rely on ImGui to calculate this
	// rather than trying to guess menu bars etc with GetFrameHeight()
	//	m_windowPos.x = ImGui::GetWindowPos().x;
	//	m_windowPos.y = ImGui::GetWindowPos().y + ImGui::GetFrameHeight();

	// get positions of min max points of the window
	m_windowMin = GUI::imToGlm(ImGui::GetWindowContentRegionMin());
	m_windowMax = GUI::imToGlm(ImGui::GetWindowContentRegionMax());

	// change them to actual screen positions
	m_windowMin.x += ImGui::GetWindowPos().x;
	m_windowMin.y += ImGui::GetWindowPos().y;
	m_windowMax.x += ImGui::GetWindowPos().x;
	m_windowMax.y += ImGui::GetWindowPos().y;

	m_windowPos = m_windowMin;

	m_windowSize.x = static_cast<int>(abs(m_windowMax.x - m_windowMin.x));
	m_windowSize.y = static_cast<int>(abs(m_windowMax.y - m_windowMin.y));

	if (m_windowManager != nullptr)
	{
		m_windowManager->m_currentWindow = shared_from_this();
	}
	else
	{
		LOG_WARN("IWindow '{}' does not have a window manager assigned! (in updateWindowInfo())", imGuiName);
	}

	if (I3T::app().m_debugWindowManager)
	{
		ImGui::GetForegroundDrawList()->AddRectFilled(GUI::glmToIm(m_windowMin), GUI::glmToIm(m_windowMax),
		                                              ImColor(0.f, 0.f, 0.f, 0.35f));
		ImGui::GetForegroundDrawList()->AddRect(GUI::glmToIm(m_windowMin), GUI::glmToIm(m_windowMax),
		                                        ImColor(1.f, 0.f, 0.f, 1.f));
		// ImGui::GetForegroundDrawList()->AddRect(windowMin, ImVec2(windowMin.x + windowWidth, windowMin.y +
		// windowHeight), ImColor(0.f, 1.f, 0.f, 0.7f));

		glm::vec2 padding = glm::vec2(4.0f);
		ImColor color = ImColor(0.27f, 0.96f, 0.09f, 1.f);
		ImVec2 size = ImGui::CalcTextSize("Sample text");

		auto windowPos = fmt::format("Window pos: {}, {}", m_windowPos.x, m_windowPos.y);
		auto windowSize = fmt::format("Window size: {}, {}", m_windowSize.x, m_windowSize.y);
		auto windowMin = fmt::format("Window min: {}, {}", m_windowMin.x, m_windowMin.y);
		auto windowMax = fmt::format("Window max: {}, {}", m_windowMax.x, m_windowMax.y);

		glm::vec2 pos = WindowManager::getMousePositionForWindow(this);
		auto mousePos = fmt::format("Rel mousePos: {}, {}", pos.x, pos.y);

		auto windowManager = fmt::format("WindowManager: {}", fmt::ptr(m_windowManager));

		ImGui::GetForegroundDrawList()->AddText(GUI::glmToIm(m_windowMin + padding + glm::vec2(0, size.y * 0)), color,
		                                        windowPos.c_str());
		ImGui::GetForegroundDrawList()->AddText(GUI::glmToIm(m_windowMin + padding + glm::vec2(0, size.y * 1)), color,
		                                        windowSize.c_str());
		ImGui::GetForegroundDrawList()->AddText(GUI::glmToIm(m_windowMin + padding + glm::vec2(0, size.y * 2)), color,
		                                        windowMin.c_str());
		ImGui::GetForegroundDrawList()->AddText(GUI::glmToIm(m_windowMin + padding + glm::vec2(0, size.y * 3)), color,
		                                        windowMax.c_str());
		ImGui::GetForegroundDrawList()->AddText(GUI::glmToIm(m_windowMin + padding + glm::vec2(0, size.y * 4)), color,
		                                        mousePos.c_str());
		ImGui::GetForegroundDrawList()->AddText(GUI::glmToIm(m_windowMin + padding + glm::vec2(0, size.y * 5)), color,
		                                        windowManager.c_str());
	}
};

//

ModalWindow::ModalWindow(const std::string& title)
{
	setName(title.c_str());
}

void ModalWindow::render()
{
	if (!ImGui::IsPopupOpen(getName()))
	{
		ImGui::OpenPopup(getName());
	}

	// OnBeginFrame();

	// Always center this window when appearing
	ImVec2 center = ImGui::GetMainViewport()->GetWorkCenter();
	// center.x *= 0.5f;
	// center.y *= 0.5f;

	// ImVec2 parent_pos = ImGui::GetWindowPos();
	// ImVec2 parent_size = ImGui::GetWindowSize();
	// ImVec2 center(parent_pos.x + parent_size.x * 0.5f, parent_pos.y +
	// parent_size.y * 0.5f);
	ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));

	if (ImGui::BeginPopupModal(getName(), nullptr, 0 | ImGuiWindowFlags_AlwaysAutoResize))
	{
		onImGui();

		ImGui::EndPopup();
	}
}

void ModalWindow::open()
{
	if (!isVisible())
	{
		ImGui::OpenPopup(getName());
		show();
	}
}

void ModalWindow::close()
{
	if (isVisible())
	{
		ImGui::CloseCurrentPopup();
		hide();
	}
}
