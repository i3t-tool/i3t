#pragma once

#include <list>
#include <string>
#include <vector>

#include "Core/Defs.h"
#include "Core/Module.h"
#include "GUI/Elements/IWindow.h"
#include "GUI/Theme.h"
#include "GUI/WindowManager.h"
#include "State/Stateful.h"

class MainMenuBar;

class UIModule final : public Module
{
	friend class Application;

	using Fonts = std::vector<ImFont*>;
	using Runnable = std::function<void()>;

public:
	UIModule() = default;
	~UIModule() override;

private:
	void onInit() override;
	void onBeginFrame() override;
	void onClose() override;

public:
	Theme& getTheme()
	{
		auto* curr = m_currentTheme;
		return *curr;
	}
	std::vector<Theme>& getThemes()
	{
		return m_allThemes;
	}

	void loadThemes();
	void reloadThemes();

	void setTheme(const Theme& theme);

	Fonts& getFonts()
	{
		return m_fonts;
	}
	void loadFonts();

	WindowManager& getWindowManager()
	{
		return m_windowManager;
	}

	template <typename T> void openModal()
	{
		m_windowManager.openModal<T>();
	}

	template <typename TStrategy> void openConfirmModal()
	{
		m_windowManager.openConfirmModal<TStrategy>();
	}

private:
	void buildDockspace();

	MainMenuBar* m_menu;

	WindowManager m_windowManager;

	Theme* m_currentTheme;
	std::vector<Theme> m_allThemes;

	Fonts m_fonts;
};
