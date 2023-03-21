/**
 * \file        Source/GUI/WindowManager.h
 * \author      Dan Rakušan <rakusdan(at)fit.cvut.cz>
 * \date        14.12.2022
 *
 * Application window manager.
 */

#pragma once

#include <map>
#include <string>
#include <vector>

#include "glm/vec2.hpp"
#include "imgui.h"

#include "Core/Defs.h"
#include "GUI/Elements/IWindow.h"

static const ImGuiWindowFlags_ g_WindowFlags = static_cast<const ImGuiWindowFlags_>(0 | ImGuiWindowFlags_NoCollapse);
static constexpr ImGuiWindowFlags_ g_dialogFlags =
    static_cast<const ImGuiWindowFlags_>(0 | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoDocking);

/**
 * Application window manager.
 * Handles window lifetime, visibility, focus and drawing. As well as some utility methods.
 * Also holds a reference to the last drawn window (See below).
 *
 * Each window should call its IWindow::updateWindowInfo() method after its ImGui::Begin() call.
 * This method updates its window dimension/position information and marks itself as the current window of its assigned
 * WindowManager. With access to window manager any code can retrieve window information without an explicit reference
 * to the window being constructed.
 */
class WindowManager
{
	friend class IWindow;

private:
	/// Application subwindows/dockable windows such as Viewport, Node editor. A
	/// window can be showed or hidden. <b>DON'T</b> remove elements from this
	/// vector.
	std::vector<Ptr<IWindow>> m_dockableWindows;
	std::map<std::string, Ptr<IWindow>> m_windows;

	// Focus
	Ptr<IWindow> m_focusedWindow;
	bool windowVisibilityChangedThisFrame{false};
	int pauseWindowFocusUpdate{0};

	Ptr<IWindow> m_currentWindow = nullptr; ///< \see getCurrentWindow()

public:
	WindowManager() = default;

	/**
	 * Handles automatic window focus change.
	 */
	void updateFocus();

	/**
	 * Draw all managed windows
	 */
	void draw();

	/**
	 * Returns the last window that started being constructed (eg. the last window that called IWindow::updateWindowInfo()
	 * after its ImGui::Begin() call).
	 */
	Ptr<IWindow> getCurrentWindow() { return m_currentWindow; }

	/**
	 * Remove all managed windows
	 */
	void clear();

	/**
	 * Add a dockable window
	 */
	void addWindow(Ptr<IWindow> window);

	/**
	 * Hide and remove window.
	 *
	 * Called by HideWindowCommand.
	 * \param windowId identification of window to hide.
	 */
	void removeWindow(const std::string& windowId);

	bool hasWindow(const std::string& id);

	// TODO: (DR) Needs better docs, the difference between dockable and undockable windows is not clear
	//  and not accounted for very well in the docs nor the method signatures
	/**
	 * Create and show desired window.
	 *
	 * \tparam T window to create. T must be derived from IWindow class.
	 */
	template <typename T> void showUniqueWindow()
	{
		checkWindowType<T>();

		if (!hasWindow(T::ID))
		{
			m_windows.insert(std::pair(std::string(T::ID), std::make_unique<T>()));
			m_windows[T::ID]->show();
		}
	}

	/**
	 * Find a dockable window of type T and return a shared pointer to it.
	 * @tparam T
	 * @return
	 */
	template <typename T> Ptr<T> getWindowPtr() { return std::dynamic_pointer_cast<T>(findWindow<T>(m_dockableWindows)); }

	void showWindow(IWindow* window, bool show);
	void showWindow(Ptr<IWindow> window, bool show);
	void focusWindow(IWindow* window);
	void focusWindow(Ptr<IWindow> window);

	template <typename T> bool isFocused()
	{
		static_assert(std::is_base_of_v<IWindow, T>, "Template param must be derived from IWindow type.");

		if (m_focusedWindow)
			return strcmp(m_focusedWindow->getID(), T::ID) == 0;
		return false;
	}

	Ptr<IWindow> getFocusedWindow() { return m_focusedWindow; }

	/**
	 * Returns relative mouse position for a specified window.
	 * This is a utility method to fetch current mouse position from InputManager and then make that position
	 * relative to the specified window.
	 */
	static glm::vec2 getMousePositionForWindow(const IWindow* window);

private:
	void updateWindowFocus();

	/**
	 * Not thread safe (using strtok).
	 */
	std::string makeIDNice(const char* ID);

	Ptr<IWindow> findAnyWindow(std::string ID);

	inline Ptr<IWindow> findWindow(const char* ID, const std::vector<Ptr<IWindow>>& windows)
	{
		for (const auto& w : windows)
		{
			const char* wID = w->getID();
			if (strcmp(wID, ID) == 0)
			{
				return w;
			}
		}
		return nullptr;
	}

	template <typename T> inline Ptr<IWindow> findWindow(const std::vector<Ptr<IWindow>>& windows)
	{
		checkWindowType<T>();

		auto result = findWindow(T::ID, windows);

		I3T_ASSERT(result != nullptr, "Unknown window, does your 'T' have I3T_WINDOW body?"
		                              "Did you create your window instance?");

		return result;
	}

	template <typename T> constexpr inline void checkWindowType()
	{
		static_assert(std::is_base_of<IWindow, T>::value, "Type parameter must be derived from IWindow class.");
	}
};