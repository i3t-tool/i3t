/**
 * \file
 * \brief
 * \authors Martin Herich <martin.herich@phire.cz>, Dan Rakušan <rakusan.dan@gmail.com>
 * \copyright Copyright (C) 2016-2023 I3T team, Department of Computer Graphics
 * and Interaction, FEE, Czech Technical University in Prague, Czech Republic
 *
 * This file is part of I3T - An Interactive Tool for Teaching Transformations
 * http://www.i3t-tool.org
 *
 * GNU General Public License v3.0 (see LICENSE.txt or https://www.gnu.org/licenses/gpl-3.0.txt)
 */
#pragma once

#include <list>
#include <map>
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

	using Fonts = std::unordered_map<std::string, ImFont*>;

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

	/**
	 * Set the theme to the default one based on the system settings (LightMode or DarkMode only on Windows).
	 */
	void setDefaultTheme();

	std::optional<Theme*> getThemeByName(const std::string& name) const;

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
