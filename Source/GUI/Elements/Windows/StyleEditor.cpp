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
	static int currentThemeIdx = 0;
	ImGui::SetNextItemWidth(2 * DRAG_FLOAT_WIDTH);
	if (ImGui::BeginCombo("Themes", curr.getName().c_str()))
	{
		for (size_t n = 0; n < I3T::getThemes().size(); ++n)
		{
			const bool isSelected = (currentThemeIdx == n);
			if (ImGui::Selectable(I3T::getThemes()[n].getName().c_str(), isSelected))
			{
				auto currIndex = Utils::indexOf(I3T::getThemes(), [&curr](Theme& t) { return t.getName() == curr.getName(); });
				if (n != currIndex)
				{
					I3T::getUI()->setTheme(I3T::getThemes()[n]);
					currentThemeIdx = (int)n;
				}
			}

			// Set the initial focus when opening the combo (scrolling + keyboard
			// navigation focus)
			if (isSelected)
				ImGui::SetItemDefaultFocus();
		}
		ImGui::EndCombo();
	}

	renderSaveRevertField();

	showColors();
	ImGui::Separator();

	showDimensions();

	ImGui::End();
}

void StyleEditor::renderSaveRevertField()
{
	auto& curr = I3T::getTheme();

	if (curr.getName() != "classic" && curr.getName() != "modern")
	{
		if (ImGui::Button("Overwrite"))
		{
			auto path = std::string("Data/themes/") + curr.getName() + ".yml";
			m_infoMessage = "Theme saved!";
			saveTheme(path, curr);
		}
		ImGui::SameLine();
	}

	// Reload themes from Data/themes
	if (GUI::Button("Reload from file"))
	{
		I3T::getUI()->reloadThemes();
	}

	ImGui::Separator();

	ImGui::TextUnformatted("Save current modifications to file.");

	/// \todo MH check for illegal characters.
	ImGui::SetNextItemWidth(2 * DRAG_FLOAT_WIDTH);
	ImGui::InputText("New theme name", &m_newThemeName);

	// Save current theme to file.
	if (ImGui::Button("Save as"))
	{
		auto path = std::string("Data/themes/") + m_newThemeName + ".yml";
		static std::regex invalidCharsRe(R"([\\\/\:\*\?\"\<\>\|])");

		if (m_newThemeName.empty())
		{
			m_infoMessage = "Theme name cannot be empty.";
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
			saveTheme(path, curr);
		}
	}
	ImGui::SameLine();

	// Revert all changes.
	if (ImGui::Button("Revert changes"))
	{
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

	ImGui::TextUnformatted(m_infoMessage.c_str());
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
