#include "StyleEditor.h"

#include "imgui.h"

#include "Config.h"
#include "GUI/Shortcuts.h"
#include "GUI/WindowManager.h"
#include "Loader/ThemeLoader.h"
#include "Utils/Filesystem.h"
#include "Utils/Other.h"

using namespace UI;

constexpr float DRAG_FLOAT_WIDTH = 100.0f;
constexpr size_t BUFF_LEN = 4096;

char g_newThemeName[BUFF_LEN];
char g_saveMessageBuff[BUFF_LEN];

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
	ImGui::SameLine();

	// Reload themes from Data/themes
	if (GUI::Button("Refresh"))
	{
		I3T::getUI()->reloadThemes();
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

	ImGui::TextUnformatted("Save current modifications to file.");

	/// \todo MH check for illegal characters.
	ImGui::SetNextItemWidth(2 * DRAG_FLOAT_WIDTH);
	ImGui::InputText("New theme name", g_newThemeName, BUFF_LEN);

	// Save current theme to file.
	if (ImGui::Button("Save"))
	{
		auto path = std::string("Data/themes/") + std::string(g_newThemeName) + ".yml";
		if (doesFileExists(path))
		{
			strcpy(g_saveMessageBuff, "Theme with this name already exists.");
		}
		else
		{
			strcpy(g_saveMessageBuff, "Theme saved!");
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

	ImGui::TextUnformatted(g_saveMessageBuff);
}

void UI::showColors()
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
		if (ImGui::ColorEdit4(str + I3T_PROPERTY_NAME_OFFSET, (float*)(&color)))
		{
			curr.apply();
		}
	}
}

void UI::showDimensions()
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
		if (ImGui::DragFloat(str + I3T_PROPERTY_NAME_OFFSET, &val, 1.0f, 0.0f, 0.0f, "%.0f"))
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
		if (ImGui::DragFloat2(str + I3T_PROPERTY_NAME_OFFSET, &val[0], 1.0f, 0.0f, 0.0f, "%.0f"))
		{
			curr.apply();
		}
	}
}
