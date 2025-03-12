/**
 * \file
 * \brief
 * \author Dan Rakušan <rakusan.dan@gmail.com>
 * \copyright Copyright (C) 2016-2023 I3T team, Department of Computer Graphics
 * and Interaction, FEE, Czech Technical University in Prague, Czech Republic
 *
 * This file is part of I3T - An Interactive Tool for Teaching Transformations
 * http://www.i3t-tool.org
 *
 * GNU General Public License v3.0 (see LICENSE.txt or https://www.gnu.org/licenses/gpl-3.0.txt)
 */
#include "WindowManager.h"

#include "imgui_internal.h"

#include "Commands/ApplicationCommands.h"
#include "Core/Input/InputManager.h"
#include "Logger/Logger.h"
#include "Toolkit.h"

void WindowManager::draw()
{
	// Render dockable windows
	for (auto element : m_dockableWindows)
	{
		if (element->isVisible())
		{
			element->render();
		}
	}

	// Render other windows.
	for (const auto& [id, w] : m_windows)
	{
		if (w->isVisible())
		{
			w->render();
		}
	}

	// Render dialog windows
	for (auto idx = 0; idx != m_modalStack.size(); ++idx)
	{
		auto& modal = m_modalStack[idx];

		if (!modal->isVisible())
		{
			m_modalStack.resize(idx);
			break;
		}

		modal->render();
	}

	// Draw debug cursors at mouse position
	if (I3T::app().m_debugWindowManager)
	{
		ImDrawList* drawList = ImGui::GetForegroundDrawList(ImGui::GetMainViewport());

		ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_AlwaysAutoResize |
		                                ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing |
		                                ImGuiWindowFlags_NoNav;
		const float PAD = 10.0f;
		const ImGuiViewport* viewport = ImGui::GetMainViewport();
		ImVec2 work_pos = viewport->WorkPos;
		ImVec2 work_size = viewport->WorkSize;
		ImGui::SetNextWindowPos({work_pos.x + work_size.x - PAD, work_pos.y + PAD}, ImGuiCond_Always, {1.0f, 0.0f});
		ImGui::SetNextWindowViewport(viewport->ID);

		ImVec4 titleBgCol = ImGui::GetStyleColorVec4(ImGuiCol_TitleBg);
		ImVec4 titleBgActiveCol = ImGui::GetStyleColorVec4(ImGuiCol_TitleBgActive);
		titleBgCol.w = 0.4f;
		titleBgActiveCol.w = 0.6f;
		ImGui::PushStyleColor(ImGuiCol_TitleBg, titleBgCol);
		ImGui::PushStyleColor(ImGuiCol_TitleBgActive, titleBgActiveCol);
		ImGui::SetNextWindowBgAlpha(0.35f);
		if (ImGui::Begin("WindowManager debug", &I3T::app().m_debugWindowManager, window_flags))
		{
			int idx = 41314;
			ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(2, 2));

			if (windowDebugTable("##dockable_windows_table"))
			{
				for (auto& window : m_dockableWindows)
				{
					windowDebugTableEntry(*window, idx);
				}
				ImGui::EndTable();
			}

			if (windowDebugTable("##other_windows_table"))
			{
				for (auto& [name, window] : m_windows)
				{
					windowDebugTableEntry(*window, idx);
				}
				ImGui::EndTable();
			}

			if (windowDebugTable("##modal_windows_table"))
			{
				for (auto& window : m_modalStack)
				{
					windowDebugTableEntry(*window, idx);
				}
				ImGui::EndTable();
			}

			ImGui::PopStyleVar();
		}
		ImGui::PopStyleColor(2);

		ImGuiContext& g = *ImGui::GetCurrentContext();

		ImGui::Text("Focused window:");
		ImGui::SameLine();
		ImGui::TextColored(ImGui::ColorConvertU32ToFloat4(IM_COL32(232, 197, 21, 255)), "%s",
		                   m_focusedWindow == nullptr ? "<null>" : m_focusedWindow->getName());

		ImGui::Text("Hovered window:");
		ImGui::SameLine();
		ImGui::TextUnformatted(windowDebugName(g.HoveredWindow));

		ImGui::Text("Nav window:");
		ImGui::SameLine();
		ImGui::TextUnformatted(windowDebugName(g.NavWindow));

		ImGui::Text("Active window:");
		ImGui::SameLine();
		ImGui::TextUnformatted(windowDebugName(g.ActiveIdWindow));

		ImGui::End();

		float thickness = 1.0f;
		float size = 32.0f;

		GUI::drawCross({InputManager::m_mouseX, InputManager::m_mouseY}, drawList, thickness, size,
		               ImColor(1.f, 0.f, 1.f, 1.0f));
		GUI::drawCross({ImGui::GetMousePos().x, ImGui::GetMousePos().y}, drawList, thickness, size,
		               ImColor(1.f, 1.f, 0.f, 1.0f));
	}
}

void WindowManager::addWindow(Ptr<IWindow> window)
{
	m_dockableWindows.push_back(window);
	window->m_windowManager = this;
}

void WindowManager::clear()
{
	for (auto& window : m_windows)
	{
		window.second->m_windowManager = nullptr;
	}
	for (auto& window : m_dockableWindows)
	{
		window->m_windowManager = nullptr;
	}
	m_windows.clear();
	m_dockableWindows.clear();
}

void WindowManager::updateFocus()
{
	// Handle window focusing
	// Issue arises when a new window is shown and the cursor happens to be hovering over the area where it shows up.
	// There is a frame delay until ImGui registers the cursor is hovering over the new window and the window that was
	// there originally is considered the hovered window. That caused the window focus to be switched back to it away
	// from the new window, even resulting in a situation where both windows kept switching focus inbetween them.
	// I couldn't find an elegant solution to this, so I just added a few frame delay (2 frames seems enough)
	// when the focus is not switched, so that the new window shows up and gets hover status.
	pauseWindowFocusUpdate = (windowVisibilityChangedThisFrame ? 5 : std::max(0, --pauseWindowFocusUpdate));
	windowVisibilityChangedThisFrame = false;
	if (!pauseWindowFocusUpdate)
	{
		updateWindowFocus();
	}
}

void WindowManager::updateWindowFocus()
{
	// Get window ids.
	ImGuiContext& g = *ImGui::GetCurrentContext();
	const char* hoveredWindowName = g.HoveredWindow ? g.HoveredWindow->Name : "";
	const char* navWindowName = g.NavWindow ? g.NavWindow->Name : "";
	const char* activeWindowName = g.ActiveIdWindow ? g.ActiveIdWindow->Name : "";

	auto hoveredWindowID = makeIDNice(hoveredWindowName); // Window the cursor is above (cam be a child window)
	auto navWindowID = makeIDNice(navWindowName);         // Currently focused window (should be a toplevel window)
	auto activeWindowID = makeIDNice(activeWindowName);   // Active = Currently dragging for example

	Ptr<IWindow> hoveredWindow = nullptr;
	bool allowFocusSwitchToHoveredWindow = !ImGui::IsAnyMouseDown();

	auto window = findImGuiWindow(g.HoveredWindow);
	if (window != nullptr)
	{
		hoveredWindow = window;
		// LOG_DEBUG("Found hovered window: {}", window->getID());
	}
	else
	{
		// LOG_DEBUG("Failed to find hovered window: {}", hoveredWindowID);
	}

	if (!allowFocusSwitchToHoveredWindow)
	{
		return;
	}

	if (hoveredWindow)
	{
		// Switch focus to the known hovered window, potentially the one thats focused already
		// But only if it has auto-focus on
		if (hoveredWindow->m_autoFocus)
		{
			focusWindow(hoveredWindow);
			// LOG_DEBUG("Auto-switching focus to known window: {}", hoveredWindow->getName());
			return;
		}

		// If auto-focus is off we need to focus the window when ImGui deems it needs focus
		// That is when the window is the ImGui navWindow
		if (ImGui::IsWindowChildOf(g.NavWindow, ImGui::FindWindowByName(hoveredWindow->getName()), true, false))
		{
			focusWindow(hoveredWindow, false);
			return;
		}
	}
	else
	{
		if (g.HoveredWindow != nullptr)
		{
			// Hovering above an unknown window
			if (m_focusedWindow != nullptr)
			{
				ImGuiWindow* focusedImGuiWindow = ImGui::FindWindowByName(m_focusedWindow->getName());
				if (ImGui::IsWindowChildOf(g.HoveredWindow, focusedImGuiWindow, true, false))
				{
					// If the unknown hovered window is a child of the known currently focused window
					// Keep focus on the currently focused window
					// LOG_DEBUG("Unknown window: {}, but keeping focus in previously focused window.",
					//          g.HoveredWindow->Name);
					return;
				}

				if (ImGui::IsWindowChildOf(g.NavWindow, focusedImGuiWindow, true, false))
				{
					// If the currently focused window is the ImGui focused (nav) window
					// (or the nav window is its child)
					// then keep focus
					return;
				}

				// Set focused window to null but let imgui handle focus on its own
				focusWindow(nullptr, false);
				// LOG_DEBUG("Unknown window: {} -> null focus", g.HoveredWindow->Name);
				return;
			}
			// Keep null focus
			// LOG_DEBUG("Keeping null focus: {}", g.HoveredWindow->Name);
			return;
		}
		else
		{
			// Hovering above a noninteractive area -> switch focus to null
			// LOG_DEBUG("Hovering over noninteractive area");
			focusWindow(nullptr, false);
		}
	}
}

bool WindowManager::windowDebugTable(const char* label)
{
	// ImGuiTableFlags_RowBg
	if (ImGui::BeginTable(label, 6, ImGuiTableFlags_BordersOuter | ImGuiTableFlags_Resizable))
	{
		ImGui::TableSetupScrollFreeze(0, 1);
		ImGui::TableSetupColumn("Title");
		ImGui::TableSetupColumn("Visible");
		ImGui::TableSetupColumn("Name");
		ImGui::TableSetupColumn("WManager");
		ImGui::TableSetupColumn("InfoUpdate");
		ImGui::TableSetupColumn("AutoFocus");
		ImGui::TableHeadersRow();

		return true;
	}
	else
	{
		return false;
	}
}

void WindowManager::windowDebugTableEntry(IWindow& window, unsigned idx)
{
	ImGui::PushID(idx++);

	ImGui::TableNextRow();
	if (!window.m_windowInfoUpdated) // Warn of window manager is null
		ImGui::TableSetBgColor(ImGuiTableBgTarget_RowBg0, IM_COL32(120, 98, 10, 255));
	if (window.m_windowManager == nullptr) // Warn that window info was never updated
		ImGui::TableSetBgColor(ImGuiTableBgTarget_RowBg0, IM_COL32(173, 14, 14, 255));
	ImGui::TableSetColumnIndex(0);
	ImGui::AlignTextToFramePadding();
	ImGui::TreeNodeEx(window.getTitle().c_str(),
	                  ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_Bullet | ImGuiTreeNodeFlags_NoTreePushOnOpen);
	ImGui::TableSetColumnIndex(1);
	ImGui::Text("%s", std::to_string(window.isVisible()).c_str());
	ImGui::TableSetColumnIndex(2);
	ImGui::Text("%s", window.getName());
	ImGui::TableSetColumnIndex(3);
	ImGui::Text("%s", std::to_string(window.m_windowManager != nullptr).c_str());
	ImGui::TableSetColumnIndex(4);
	ImGui::Text("%s", std::to_string(window.m_windowInfoUpdated).c_str());
	ImGui::TableSetColumnIndex(5);
	ImGui::Text("%s", std::to_string(window.m_autoFocus).c_str());
	ImGui::PopID();
}

const char* WindowManager::windowDebugName(ImGuiWindow* window)
{
	if (!window)
		return "<null>";
	if (strlen(window->Name) <= 0)
		return "<noname>";
	return window->Name;
}

Ptr<IWindow> WindowManager::findImGuiWindow(ImGuiWindow* window)
{
	if (window == nullptr)
		return nullptr;
	for (const auto& w : m_dockableWindows)
	{
		if (ImGui::IsWindowChildOf(window, ImGui::FindWindowByName(w->getName()), true, false))
		{
			return w;
		}
	}
	for (const auto& [id, w] : m_windows)
	{
		if (ImGui::IsWindowChildOf(window, ImGui::FindWindowByName(w->getName()), true, false))
		{
			return w;
		}
	}
	return nullptr;
}

Ptr<IWindow> WindowManager::findAnyWindow(std::string ID)
{
	Ptr<IWindow> window = findWindow(ID.c_str(), m_dockableWindows);

	if (window == nullptr)
	{
		auto it = m_windows.find(ID);
		if (it != m_windows.end())
		{
			window = it->second;
		}
	}
	return window;
}

void WindowManager::removeWindow(const std::string& windowId)
{
	auto it = m_windows.find(windowId);
	if (it != m_windows.end())
	{
		it->second->m_windowManager = nullptr;
		m_windows.erase(it);
	}
}

// TODO: (DR) This only checks m_windows and not dockable windows, that is not clear from its signature
bool WindowManager::hasWindow(const std::string& id)
{
	return m_windows.count(id);
}

void WindowManager::showWindow(IWindow* window, bool show)
{
	if (window == nullptr)
	{
		LOG_ERROR("Cannot show null window!");
		return;
	}
	*window->getShowPtr() = show;
	windowVisibilityChangedThisFrame = true;
}

void WindowManager::showWindow(Ptr<IWindow> window, bool show)
{
	showWindow(window.get(), show);
}

void WindowManager::focusWindow(Ptr<IWindow> window, bool updateImGuiFocus)
{
	if (window != m_focusedWindow)
	{
		if (updateImGuiFocus)
			ImGui::SetWindowFocus(window ? window->getName() : nullptr);
		m_focusedWindow = window;
		SetFocusedWindowCommand::dispatch(window);
	}
}

glm::vec2 WindowManager::getMousePositionForWindow(const IWindow* window)
{
	return GUI::convertCoordinates({InputManager::m_mouseX, InputManager::m_mouseY}, window->m_windowPos);
}

void WindowManager::openModal(UPtr<IWindow> modal)
{
	modal->m_windowManager = this;
	modal->show();
	m_modalStack.push_back(std::move(modal));
}

std::string WindowManager::makeIDNice(const char* ID)
{
	// Remove ## or ### from ImGui window name.
	// auto sanitizedWindowID = ID;
	// while (*sanitizedWindowID == '#')
	//  ++sanitizedWindowID;

	std::string IDCopy = std::string(ID);

	if (IDCopy.empty())
	{
		return IDCopy;
	}

	// Get fist part of "window/child-window" id.
	char* currID = std::strtok(const_cast<char*>(IDCopy.c_str()), "###");
	char* lastID;
	while (currID != nullptr)
	{
		lastID = currID;
		currID = std::strtok(nullptr, "###");
	}

	currID = std::strtok(const_cast<char*>(lastID), "/");

	return std::string(currID);
}

void WindowManager::loadLayout(std::string path)
{
	ImGui::LoadIniSettingsFromDisk(path.c_str());
}
