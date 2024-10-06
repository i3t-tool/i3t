/**
 * \file
 * \brief
 * \author Martin Herich <martin.herich@phire.cz>
 * \copyright Copyright (C) 2016-2023 I3T team, Department of Computer Graphics
 * and Interaction, FEE, Czech Technical University in Prague, Czech Republic
 *
 * This file is part of I3T - An Interactive Tool for Teaching Transformations
 * http://www.i3t-tool.org
 *
 * GNU General Public License v3.0 (see LICENSE.txt or https://www.gnu.org/licenses/gpl-3.0.txt)
 */
#include "StyleEditor.h"

#include <regex>

#define IMGUI_DEFINE_MATH_OPERATORS
#include "imgui.h"
#include "misc/cpp/imgui_stdlib.h"

#include "GUI/IconFonts/Icons.h"
#include "GUI/Theme/ThemeLoader.h"
#include "GUI/ThemeVariable.h"
#include "GUI/Toolkit.h"
#include "GUI/WindowManager.h"
#include "UserData.h"
#include "Utils/Other.h"
#include "Utils/Variant.h"

namespace UI
{
static void renderVariables();

constexpr float DRAG_FLOAT_WIDTH = 100.0f;

StyleEditor::StyleEditor() : IWindow(ICON_I3T_STYLE " Style Editor") {}

void StyleEditor::render()
{
	ImGui::Begin(getName(), getShowPtr(), g_dialogFlags);

	this->updateWindowInfo();

	auto& currThemeName = getUserData().customThemeName.empty() ? "default" : I3T::getTheme().getName();

	// Custom theme selector
	ImGui::SetNextItemWidth(2 * DRAG_FLOAT_WIDTH);
	if (ImGui::BeginCombo("Custom theme", currThemeName.c_str()))
	{
		for (size_t n = 0; n < I3T::getThemes().size(); ++n)
		{
			const bool isSelected = (m_currentThemeIdx == n);
			if (ImGui::Selectable(I3T::getThemes()[n].getName().c_str(), isSelected))
			{
				auto currIndex = Utils::indexOf(I3T::getThemes(), [&currThemeName](Theme& t) {
					return t.getName() == currThemeName;
				});
				if (n != currIndex)
				{
					I3T::getUI()->setTheme(I3T::getThemes()[n]);
					m_currentThemeIdx = (int) n;
				}
			}

			// Set the initial focus when opening the combo (scrolling + keyboard
			// navigation focus)
			if (isSelected)
			{
				ImGui::SetItemDefaultFocus();
			}
		}
		ImGui::EndCombo();
	}

	ImGui::SameLine();
	if (ImGui::Button("Use default theme"))
	{
		I3T::getUI()->setDefaultTheme();
	}

	ImGui::Separator();

	renderSaveRevertField();

	renderVariables();

	ImGui::End();
}

void renderVariables()
{
	auto& curr = I3T::getTheme();

	for (const auto& group : Theme::getVariables())
	{
		ImGui::TextUnformatted(group.name);
		ImGui::Separator();

		for (const auto& var : group.variables)
		{
			float valueMin = 0.0f, valueMax = 0.0f;
			if (var.range.has_value())
			{
				valueMin = var.range.value().x;
				valueMax = var.range.value().y;
			}

			std::visit(Detail::Overloaded{
			               [&](const EColor& color) {
				               auto& val = curr.getColorsRef()[color];
				               ImGui::SetNextItemWidth(4 * DRAG_FLOAT_WIDTH);
				               if (ImGui::ColorEdit4(var.name, (float*) (&val)))
				               {
					               curr.apply();
				               }
			               },
			               [&](const ESize& size) {
				               auto& val = curr.getSizesRef()[size];

				               ImGui::SetNextItemWidth(DRAG_FLOAT_WIDTH);

				               if (var.forceInt)
				               {
					               int intVal = (int) val;
					               if (ImGui::DragInt(var.name, &intVal, 1.0f, (int) valueMin, (int) valueMax, "%.0f"))
					               {
						               val = (float) intVal;
						               curr.apply();
					               }
				               }
				               else
				               {
					               if (ImGui::DragFloat(var.name, &val, 1.0f, valueMin, valueMax, "%.0f"))
					               {
						               curr.apply();
					               }
				               }
			               },
			               [&](const ESizeVec2& sizeVec) {
				               auto& val = curr.getSizesVecRef()[sizeVec];

				               ImGui::SetNextItemWidth(2 * DRAG_FLOAT_WIDTH);

				               if (var.forceInt)
				               {
					               int intVal[2] = {(int) val[0], (int) val[1]};
					               if (ImGui::DragInt2(var.name, intVal, 1.0f, (int) valueMin, (int) valueMax, "%.0f"))
					               {
						               val[0] = (float) intVal[0];
						               val[1] = (float) intVal[1];
						               curr.apply();
					               }
				               }
				               else
				               {
					               if (ImGui::DragFloat2(var.name, &val[0], 1.0f, valueMin, valueMax, "%.0f"))
					               {
						               curr.apply();
					               }
				               }
			               },
			           },
			           var.key);
		}
	}
}

void StyleEditor::renderSaveRevertField()
{
	auto& curr = I3T::getTheme();

	if (ImGui::Button("Revert changes"))
	{
		revertChangesOnCurrentTheme();
	}
	ImGui::SameLine();

	if (GUI::Button("Reload all Themes"))
	{
		// Reload Themes from Data/Themes
		I3T::getUI()->reloadThemes();
	}
	ImGui::SameLine();
	ImGui::Text("(Discards all unsaved changes)");

	ImGui::Separator();

	ImGui::TextUnformatted("Save current modifications to file (discards unsaved changes on other Themes)");

	// Classic theme cannot be overwritten.
	/// \todo Add list of default themes (LightMode and DarkMode) when they be finished.
	if (curr.getName() != I3T_CLASSIC_THEME_NAME)
	{
		if (ImGui::Button("Overwrite"))
		{
			saveCurrentTheme(curr.getName());
		}
		ImGui::SameLine();
	}

	// Save current theme to file.
	if (ImGui::Button("Save as"))
	{
		auto path = std::string("Data/Themes/") + m_newThemeName + ".yml";
		static std::regex invalidCharsRe(R"([\\\/\:\*\?\"\<\>\|])");

		if (m_newThemeName.empty())
		{
			m_infoMessage = "Theme name cannot be empty.";
		}
		else if (m_newThemeName == I3T_CLASSIC_THEME_NAME)
		{
			m_infoMessage = "Cannot overwrite default Themes.";
		}
		else if (std::regex_search(m_newThemeName, invalidCharsRe))
		{
			m_infoMessage = "Theme name contains invalid characters.";
		}
		else if (fs::exists(fs::path(path)))
		{
			m_infoMessage = "Theme with this name already exists.";
		}
		else
		{
			m_infoMessage = "Theme saved!";
			saveCurrentTheme();
		}
	}
	ImGui::SameLine();

	ImGui::SetNextItemWidth(2 * DRAG_FLOAT_WIDTH);
	ImGui::InputText("New theme name", &m_newThemeName);

	ImGui::TextUnformatted(m_infoMessage.c_str());
}

void StyleEditor::saveCurrentTheme(const std::string& name)
{
	auto& curr = I3T::getTheme();
	auto themeName = name;
	if (themeName.empty())
	{
		themeName = m_newThemeName;
	}
	auto path = std::string("Data/Themes/") + themeName + ".yml";

	saveTheme(path, curr);
	I3T::getUI()->reloadThemes();

	auto& themes = I3T::getThemes();

	// newly saved theme is now the current theme
	m_currentThemeIdx = Utils::indexOf(themes, [&curr, &themeName](Theme& t) {
		return t.getName() == themeName;
	});
	if (m_currentThemeIdx != -1)
	{
		I3T::getUI()->setTheme(themes[m_currentThemeIdx]);
	}

	m_infoMessage = "";
	m_newThemeName = "";
}

void StyleEditor::revertChangesOnCurrentTheme()
{
	auto& curr = I3T::getTheme();

	if (curr.getName() == I3T_CLASSIC_THEME_NAME)
	{
		I3T::emplaceTheme(Theme::createDefaultClassic());
	}
	else
	{
		auto path = std::string("Data/Themes/") + curr.getName() + ".yml";
		if (auto theme = loadTheme(path))
		{
			I3T::emplaceTheme(*theme);
		}
	}
}
} // namespace UI
