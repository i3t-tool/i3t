/**
 * \file        Source\GUI\WindowManager.h
 * \author      Dan Rakušan <rakusdan@fit.cvut.cz>
 * \date        14.12.2022
 *
 * Application window manager.
 * Handles window lifetime, visibility, focus and drawing.
 */

#pragma once

#include <map>
#include <string>
#include <vector>

#include "Core/Defs.h"
#include "GUI/Elements/IWindow.h"
#include "GUI/Theme.h"

static const ImGuiWindowFlags_ g_WindowFlags = static_cast<const ImGuiWindowFlags_>(0 | ImGuiWindowFlags_NoCollapse);
static constexpr ImGuiWindowFlags_ g_dialogFlags =
    static_cast<const ImGuiWindowFlags_>(0 | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoDocking);

class WindowManager
{
public:
	WindowManager() = default;

	void updateFocus();
	void draw();
	void clear();

	void addWindow(Ptr<IWindow> window);
	/**
	 * Hide and remove window.
	 *
	 * Called by HideWindowCommand.
	 * \param windowId identification of window to hide.
	 */
	void popWindow(const std::string& windowId);
	bool hasWindow(const std::string& id);

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

	template <typename T> Ptr<T> getWindowPtr() { return std::dynamic_pointer_cast<T>(findWindow<T>(m_dockableWindows)); }

	void showWindow(IWindow* window, bool show);
	void focusWindow(IWindow* window);

private:
	void updateWindowFocus();

	/**
	 * Not thread safe (using strtok).
	 */
	std::string makeIDNice(const char* ID);

	Ptr<IWindow> findAnyWindow(std::string ID);

	inline Ptr<IWindow> findWindow(const char* ID, const std::vector<Ptr<IWindow>>& dockableWindows)
	{
		for (const auto& w : dockableWindows)
		{
			const char* wID = w->getID();
			if (strcmp(wID, ID) == 0)
			{
				return w;
			}
		}
		return nullptr;
	}

	template <typename T> inline Ptr<IWindow> findWindow(const std::vector<Ptr<IWindow>>& dockableWindows)
	{
		checkWindowType<T>();

		auto result = findWindow(T::ID, dockableWindows);

		Debug::Assert(result != nullptr, "Unknown window, does your 'T' have I3T_WINDOW body?"
		                                 "Did you create your window instance?");

		return result;
	}

	template <typename T> constexpr inline void checkWindowType()
	{
		static_assert(std::is_base_of<IWindow, T>::value, "Type parameter must be derived from IWindow class.");
	}

private:
	/// Application subwindows/dockable windows such as Viewport, Node editor. A
	/// window can be showed or hidden. <b>DON'T</b> remove elements from this
	/// vector.
	std::vector<Ptr<IWindow>> m_dockableWindows;

	std::map<std::string, Ptr<IWindow>> m_windows;

	bool windowVisibilityChangedThisFrame{false};
	int pauseWindowFocusUpdate{0};
};