#include "WindowManager.h"

#define IMGUI_DEFINE_MATH_OPERATORS
#include "imgui.h"
#include "imgui_internal.h"

#include "Commands/ApplicationCommands.h"
#include "Core/Input/InputManager.h"
#include "Logger/Logger.h"
#include "Toolkit.h"

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
		float thickness = 1.0f;
		float size = 32.0f;

		GUI::drawCross({InputManager::m_mouseX, InputManager::m_mouseY}, ImGui::GetForegroundDrawList(), thickness,
		               size, ImColor(1.f, 0.f, 1.f, 1.0f));
		GUI::drawCross({ImGui::GetMousePos().x, ImGui::GetMousePos().y}, ImGui::GetForegroundDrawList(), thickness,
		               size, ImColor(1.f, 1.f, 0.f, 1.0f));
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

void WindowManager::updateWindowFocus()
{
	// Get window ids.
	ImGuiContext& g = *ImGui::GetCurrentContext();
	const char* hoveredWindowName = g.HoveredWindow ? g.HoveredWindow->Name : "";
	const char* navWindowName = g.NavWindow ? g.NavWindow->Name : "";
	const char* activeWindowName = g.ActiveIdWindow ? g.ActiveIdWindow->Name : "";

	// LOG_DEBUG("Hovered: {}, Active: {}, Nav: {}", hoveredWindowName, activeWindowName, navWindowName);

	auto hoveredWindowID = makeIDNice(hoveredWindowName); // Window the cursor is above (cam be a child window)
	auto navWindowID = makeIDNice(navWindowName);         // Currently focused window (should be a toplevel window)
	auto activeWindowID = makeIDNice(activeWindowName);   // Active = Currently dragging for example

	Ptr<IWindow> hoveredWindow = nullptr;
	bool allowFocusSwitchToHoveredWindow = !ImGui::IsAnyMouseDown();

	if (strlen(hoveredWindowName) != 0)
	{
		auto window = findAnyWindow(hoveredWindowID);
		if (window != nullptr)
		{
			hoveredWindow = window;
		}
		else
		{
			// LOG_DEBUG("Failed to find hovered window {}", hoveredWindowID);
		}
	}

	Ptr<IWindow> newFocusedWindow;
	const char* newFocusedWindowName = nullptr;

	if (hoveredWindow && allowFocusSwitchToHoveredWindow)
	{
		// Switch focus to hovered window
		newFocusedWindow = hoveredWindow;
		newFocusedWindowName = hoveredWindowName;
	}
	else
	{
		// Keep the focus on the currently focused window
		if (strlen(navWindowName) != 0)
		{
			// Otherwise just update the currently focused window
			auto window = findAnyWindow(navWindowID);
			if (window != nullptr)
			{
				newFocusedWindow = window;
				newFocusedWindowName = navWindowName;
			}
			else
			{
				// LOG_DEBUG("Failed to find nav window {}", navWindowID);
			}
		}
	}

	// Switch focus if necessary
	// LOG_DEBUG("New focused window: {}", (newFocusedWindowName ? newFocusedWindowName : "null"));
	if (newFocusedWindow != nullptr)
	{
		if (newFocusedWindow != getFocusedWindow())
		{
			focusWindow(newFocusedWindow);
		}
	}
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

void WindowManager::focusWindow(IWindow* window)
{
	focusWindow(findAnyWindow(window->getID()));
}

void WindowManager::focusWindow(Ptr<IWindow> window)
{
	ImGui::SetWindowFocus(window->getName());
	m_focusedWindow = window;
	SetFocusedWindowCommand::dispatch(window);
}

glm::vec2 WindowManager::getMousePositionForWindow(const IWindow* window)
{
	return GUI::convertCoordinates({InputManager::m_mouseX, InputManager::m_mouseY}, window->m_windowPos);
}

void WindowManager::openModal(UPtr<IWindow> modal)
{
	modal->show();
	m_modalStack.push_back(std::move(modal));
}
