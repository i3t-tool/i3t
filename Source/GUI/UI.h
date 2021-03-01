#pragma once

#include <map>
#include <string>
#include <vector>

#include "Core/Defs.h"
#include "Core/Module.h"
#include "GUI/Elements/IWindow.h"
#include "Theme.h"

class MainMenuBar;

template <typename T>
constexpr inline void checkWindowType()
{
  static_assert(std::is_base_of<IWindow, T>::value, "Type parameter must be derived from IWindow class.");
}

template <typename T>
inline IWindow* findWindow(const std::vector<IWindow*>& dockableWindows)
{
  checkWindowType<T>();

  IWindow* result = nullptr;
  for (auto* w : dockableWindows)
    if (strcmp(w->getID(), T::ID) == 0)
      result = w;

  Debug::Assert(result != nullptr, "Unknown window, does your 'T' have I3T_WINDOW body?");

  return result;
}

class UI final : public Module
{
	friend class Application;

	UI() {}
  void init() override;
	void beginFrame() override;

	void buildDockspace();

public:
	Theme& getTheme()
  {
		return m_currentTheme;
	}

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

	/*
	template <typename T> void showDockableWindow()
  {

	}

	template <typename T> void hideDockableWindow()
  {

	}
	 */

	template <typename T> IWindow* getWindowPtr()
  {
    return findWindow<T>(m_dockableWindows);
	}

private:
  MainMenuBar* m_menu;

  /// Application subwindows/dockable windows such as Viewport, Node editor. A window can be showed or hidden.
  /// <b>DON'T</b> remove elements from this vector.
	std::vector<IWindow*> m_dockableWindows;

  std::map<std::string, std::unique_ptr<IWindow>> m_windows;

	Theme m_currentTheme;
};
