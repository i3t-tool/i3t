#pragma once

#include <string>
#include <vector>

#include "Core/Defs.h"
#include "Core/Module.h"
#include "GUI/Elements/IWindow.h"
#include "GUI/Theme.h"
#include "GUI/WindowManager.h"
#include "State/Stateful.h"

class MainMenuBar;

constexpr const char* ImGui_GLSLVersion = "#version 140";

class UIModule final : public Module
{
	friend class Application;

	using Fonts = std::vector<ImFont*>;

public:
	UIModule() = default;
	~UIModule() override;

private:
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

	WindowManager& getWindowManager() { return m_windowManager; }

private:
	void buildDockspace();
	void queryCameraState();

private:
	MainMenuBar* m_menu;

	WindowManager m_windowManager;

	Theme* m_currentTheme;
	std::vector<Theme> m_allThemes;

	Fonts m_fonts;
};
