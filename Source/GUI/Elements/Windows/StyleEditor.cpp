#include "StyleEditor.h"

#include <regex>

#include "imgui.h"
#include "misc/cpp/imgui_stdlib.h"

#include "Config.h"
#include "GUI/Shortcuts.h"
#include "GUI/WindowManager.h"
#include "Loader/ThemeLoader.h"
#include "Utils/Filesystem.h"
#include "Utils/Other.h"

namespace UI
{
constexpr float DRAG_FLOAT_WIDTH = 100.0f;

void StyleEditor::render()
{
	ImGui::Begin(getID(), getShowPtr(), g_dialogFlags);

	this->updateWindowInfo();

	auto& curr = I3T::getTheme();

	// Theme selector
	ImGui::SetNextItemWidth(2 * DRAG_FLOAT_WIDTH);
	if (ImGui::BeginCombo("Themes", curr.getName().c_str()))
	{
		for (size_t n = 0; n < I3T::getThemes().size(); ++n)
		{
			const bool isSelected = (m_currentThemeIdx == n);
			if (ImGui::Selectable(I3T::getThemes()[n].getName().c_str(), isSelected))
			{
				auto currIndex = Utils::indexOf(I3T::getThemes(), [&curr](Theme& t) { return t.getName() == curr.getName(); });
				if (n != currIndex)
				{
					I3T::getUI()->setTheme(I3T::getThemes()[n]);
					m_currentThemeIdx = (int)n;
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

	ImGui::Separator();

	renderSaveRevertField();

	showColors();
	ImGui::Separator();

	showDimensions();

	ImGui::End();
}

void StyleEditor::renderSaveRevertField()
{
	auto& curr = I3T::getTheme();

	if (ImGui::Button("Revert changes"))
	{
		revertChangesOnCurrentTheme();
	}
	ImGui::SameLine();

	if (GUI::Button("Reload all themes"))
	{
		// Reload themes from Data/themes
		I3T::getUI()->reloadThemes();
	}
	ImGui::SameLine();
	ImGui::Text("(Discards all unsaved changes)");

	ImGui::Separator();

	ImGui::TextUnformatted("Save current modifications to file (discards unsaved changes on other themes)");

	if (curr.getName() != "classic" && curr.getName() != "modern")
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
		auto path = std::string("Data/themes/") + m_newThemeName + ".yml";
		static std::regex invalidCharsRe(R"([\\\/\:\*\?\"\<\>\|])");

		if (m_newThemeName.empty())
		{
			m_infoMessage = "Theme name cannot be empty.";
		}
		else if (m_newThemeName == "classic" || m_newThemeName == "modern")
		{
			m_infoMessage = "Cannot overwrite default themes.";
		}
		else if (std::regex_search(m_newThemeName, invalidCharsRe))
		{
			m_infoMessage = "Theme name contains invalid characters.";
		}
		else if (doesFileExist(path))
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
	auto path = std::string("Data/themes/") + themeName + ".yml";

	saveTheme(path, curr);
	I3T::getUI()->reloadThemes();

	auto& themes = I3T::getThemes();

	// newly saved theme is now the current theme
	m_currentThemeIdx = Utils::indexOf(themes, [&curr, &themeName](Theme& t) { return t.getName() == themeName; });
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

	if (curr.getName() == "classic")
	{
		I3T::emplaceTheme(Theme::createDefaultClassic());
	}
	else if (curr.getName() == "modern")
	{
		I3T::emplaceTheme(Theme::createDefaultModern());
	}
	else
	{
		auto path = std::string("Data/themes/") + curr.getName() + ".yml";
		if (auto theme = loadTheme(path))
		{
			I3T::emplaceTheme(*theme);
		}
	}
}

void showColors()
{
	auto& curr = I3T::getTheme();

	GUI::Text("Colors", EFont::Header);
	ImGui::Separator();

	std::string lastCat;

	for (auto& [key, str] : Theme::getColorNames())
	{
		std::string currCat(str, I3T_PROPERTY_NAME_OFFSET);
		if (currCat != lastCat)
		{
			ImGui::TextUnformatted(Theme::getCategoryName(currCat));
			lastCat = currCat;
		}

		auto& color = curr.getColorsRef()[key];
		ImGui::SetNextItemWidth(4 * DRAG_FLOAT_WIDTH);

		const auto label = fmt::format("{}##{}", str + I3T_PROPERTY_NAME_OFFSET, (unsigned) key);
		if (ImGui::ColorEdit4(label.c_str(), (float*)(&color)))
		{
			curr.apply();
		}
	}
}

void showDimensions()
{
	auto& curr = I3T::getTheme();

	GUI::Text("Dimensions", EFont::Header);
	ImGui::Separator();

	std::string lastCat;

	for (auto& [key, str] : Theme::getSizeNames())
	{
		std::string currCat(str, I3T_PROPERTY_NAME_OFFSET);
		if (currCat != lastCat)
		{
			ImGui::TextUnformatted(Theme::getCategoryName(currCat));
			lastCat = currCat;
		}

		auto& val = curr.getSizesRef()[key];
		ImGui::SetNextItemWidth(DRAG_FLOAT_WIDTH);
		if (ImGui::DragFloat(str + I3T_PROPERTY_NAME_OFFSET, &val, 1.0f, 0.0f, FLT_MAX, "%.0f"))
		{
			curr.apply();
		}
	}
	ImGui::Separator();

	for (auto& [key, str] : Theme::getSizeVecNames())
	{
		std::string currCat(str, I3T_PROPERTY_NAME_OFFSET);
		if (currCat != lastCat)
		{
			ImGui::TextUnformatted(Theme::getCategoryName(currCat));
			lastCat = currCat;
		}

		auto& val = curr.getSizesVecRef()[key];
		ImGui::SetNextItemWidth(2 * DRAG_FLOAT_WIDTH);
		if (ImGui::DragFloat2(str + I3T_PROPERTY_NAME_OFFSET, &val[0], 1.0f, 0.0f, FLT_MAX, "%.0f"))
		{
			curr.apply();
		}
	}
}
}
