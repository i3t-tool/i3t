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

constexpr const char* ImGui_GLSLVersion = "#version 140";

class UIModule final : public Module
{
	friend class Application;

	using Fonts = std::vector<ImFont*>;
	using Runnable = std::function<void()>;

public:
	UIModule() = default;
	~UIModule() override;

private:
	void init() override;
	void beginFrame() override;
	void onClose() override;

	/**
	 * Some calls can occur outside of the ImGui drawing context (between the NewFrame and Render methods), those kinds
	 * of calls cannot use some of the ImGui mechanisms, this list keeps track of lambdas submitted the previous frame
	 * and each of them is going to be run in the next frame during ImGui drawing.
	 * This way key events can use ImGui rendering code without exceptions.
	 */
	std::list<Runnable> queuedCalls;

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

	/**
	 * Schedules passed lambda/function to be run next frame after UI drawing but within UI (ImGui) context.
	 * @see queuedCalls
	 */
	void invokeLater(Runnable call)
	{
		queuedCalls.push_back(call);
	}

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
