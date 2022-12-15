#include "WindowManager.h"

#include "imgui.h"
#include "imgui_internal.h"

#include "Commands/ApplicationCommands.h"
#include "Logger/Logger.h"
#include "Core/Application.h"

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
	for (auto element : m_dockableWindows)
	{
		if (element->isVisible())
		{
			element->render();
		}
		// if (InputController::isKeyJustPressed(Keys::f)) { printf("UP
		// %s\n",element->getID()); }
	}
	// if (InputController::isKeyJustPressed(Keys::f)) { printf("--- \n"); }
	// Render other windows.
	for (const auto& [id, w] : m_windows)
	{
		if (w->isVisible())
		{
			w->render();
		}
	}
}
void WindowManager::addWindow(Ptr<IWindow> window) { m_dockableWindows.push_back(window); }
void WindowManager::clear()
{
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

	// Log::debug("Hovered: {}, Active: {}, Nav: {}", hoveredWindowName, activeWindowName, navWindowName);

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
			// Log::debug("Failed to find hovered window {}", hoveredWindowID);
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
				// Log::debug("Failed to find nav window {}", navWindowID);
			}
		}
	}

	// Switch focus if necessary
	//Log::debug("New focused window: {}", (newFocusedWindowName ? newFocusedWindowName : "null"));
	if (newFocusedWindow != nullptr)
	{
		if (newFocusedWindow != InputManager::getFocusedWindow())
		{
			ImGui::SetWindowFocus(newFocusedWindowName);
			InputManager::setFocusedWindow(newFocusedWindow);
			SetFocusedWindowCommand::dispatch(newFocusedWindow);
		}
	}
}

Ptr<IWindow> WindowManager::findAnyWindow(std::string ID)
{
	Ptr<IWindow> window = findWindow(ID.c_str(), m_dockableWindows);

	if (m_windows.count(ID) != 0)
	{
		window = m_windows[ID];
	};
	return window;
}

void WindowManager::popWindow(const std::string& windowId)
{
	if (hasWindow(windowId))
		m_windows.erase(windowId);
}

bool WindowManager::hasWindow(const std::string& id) { return m_windows.count(id); }

void WindowManager::showWindow(IWindow* window, bool show)
{
	if (window == nullptr)
	{
		Log::error("Cannot show null window!");
		return;
	}
	*window->getShowPtr() = show;
	windowVisibilityChangedThisFrame = true;
}

void WindowManager::focusWindow(IWindow* window)
{
	Ptr<IWindow> ptr = findAnyWindow(window->getID());
	ImGui::SetWindowFocus(window->getName().c_str());
	SetFocusedWindowCommand::dispatch(ptr);
}