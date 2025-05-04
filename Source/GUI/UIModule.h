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

#include <string>
#include <vector>

#include "rttr/registration.h"

#include "Core/Defs.h"
#include "Core/Module.h"
#include "Fonts/FontManager.h"
#include "GUI/Theme/Theme.h"
#include "GUI/WindowManager.h"
#include "State/Stateful.h"

class MainMenuBar;

struct UISettings
{
	bool useWindowMenuButtons{false};
	bool autoHideTabBars{false};
};

class UIModule final : public Module, public IStateful
{
	friend class Application;

public:
	UIModule() = default;
	~UIModule() override;


private:
	void onInit() override;
	void onBeforeFrame() override;
	void onBeginFrame() override;
	void onClose() override;
	static float getMainWindowDpiScaleFactor();

public:
	UISettings& getSettings();
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

	/**
	 * Applies a UI/DPI scaling factor to the current Theme.
	 * ImGuiStyle is reinitialized, the theme reapplied and fonts reloaded.
	 * @warning This method cannot be called during ImGui drawing, eg. between NewFrame() and EndFrame/Render()!
	 * Use applyUIScalingNextFrame() instead for changes within UI code itself.
	 * @param scale The new UI scaling factor.
	 */
	void applyUIScaling(float scale);

	/// @see applyUIScaling()
	void applyUIScalingNextFrame(float scale);

	/**
	 * Get the current UI scale, by default derived from main windows DPI scaling factor.
	 * UI scale can be changed using applyUIScaling() or applyUIScalingNextFrame().
	 */
	float getUiScale() const;

	FontManager& getFontManager()
	{
		return m_fontManager;
	}

	WindowManager& getWindowManager()
	{
		return m_windowManager;
	}

	template <typename T>
	void openModal()
	{
		m_windowManager.openModal<T>();
	}

	template <typename TStrategy>
	void openConfirmModal()
	{
		m_windowManager.openConfirmModal<TStrategy>();
	}

private:
	void buildDockspace();

	UISettings m_settings;

	float m_uiScale;
	float m_queuedUiScale = -1.0f;

	MainMenuBar* m_menu;

	WindowManager m_windowManager;
	FontManager m_fontManager;

	Theme* m_currentTheme;
	std::vector<Theme> m_allThemes;

private:
	// Save layout and tutorial info to the file scene.
	Memento saveScene(State::Scene* scene) override;
	// Load layout and tutorial info from the file scene.
	void loadScene(const Memento& memento, State::Scene* scene) override;
	void appendScene(const Memento& memento, State::Scene* scene) override {}
	void clearScene(bool newScene) override;

public:
	Memento saveGlobal() override;
	void loadGlobal(const Memento& memento) override;
	void clearGlobal() override;
};
