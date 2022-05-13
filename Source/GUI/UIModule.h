#pragma once

#include <map>
#include <string>
#include <vector>

#include "Core/Defs.h"
#include "Core/Module.h"
#include "GUI/Elements/IWindow.h"
#include "State/Stateful.h"
#include "Theme.h"

constexpr const char* ImGui_GLSLVersion = "#version 140";

static const ImGuiWindowFlags_ g_WindowFlags = static_cast<const ImGuiWindowFlags_>(0 | ImGuiWindowFlags_NoCollapse);
static constexpr ImGuiWindowFlags_ g_dialogFlags =
		static_cast<const ImGuiWindowFlags_>(0 | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoDocking);

class MainMenuBar;

template <typename T> constexpr inline void checkWindowType()
{
	static_assert(std::is_base_of<IWindow, T>::value, "Type parameter must be derived from IWindow class.");
}

inline Ptr<IWindow> findWindow(const char* ID, const std::vector<Ptr<IWindow>>& dockableWindows)
{
	Ptr<IWindow> result = nullptr;
	for (const auto& w : dockableWindows)
		if (strcmp(w->getID(), ID) == 0)
			result = w;

	return result;
}

template <typename T> inline Ptr<IWindow> findWindow(const std::vector<Ptr<IWindow>>& dockableWindows)
{
	checkWindowType<T>();

	auto result = findWindow(T::ID, dockableWindows);

	Debug::Assert(result != nullptr, "Unknown window, does your 'T' have I3T_WINDOW body?");

	return result;
}

class UIModule final : public Module
{
	friend class Application;

	using Fonts = std::vector<ImFont*>;

	UIModule() = default;
	~UIModule() override;
	void init() override;
	void beginFrame() override;
	void onClose() override;

public:
	Theme& getTheme()
	{
		auto* curr = m_currentTheme;
		return *curr;
	}
	std::vector<Theme>& getThemes() { return m_allThemes; }

	void loadThemes();
	void reloadThemes();

	void setTheme(const Theme& theme);

	Fonts& getFonts() { return m_fonts; }
	void loadFonts();

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

private:
	void setFocusedWindow();

	void buildDockspace();
	void queryCameraState();

private:
	MainMenuBar* m_menu;

	/// Application subwindows/dockable windows such as Viewport, Node editor. A window can be showed or hidden.
	/// <b>DON'T</b> remove elements from this vector.
	std::vector<Ptr<IWindow>> m_dockableWindows;

	std::map<std::string, Ptr<IWindow>> m_windows;

	Theme* m_currentTheme;
	std::vector<Theme> m_allThemes;

	Fonts m_fonts;
};
