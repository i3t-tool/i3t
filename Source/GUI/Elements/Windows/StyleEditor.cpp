#include "StyleEditor.h"

#include "imgui.h"

#include "Config.h"
#include "GUI/Shortcuts.h"
#include "Loader/ThemeLoader.h"

using namespace UI;

constexpr float DRAG_FLOAT_WIDTH = 100.0f;

void StyleEditor::render()
{
	ImGui::Begin(getID(), getShowPtr(), g_dialogFlags);

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
				I3T::getUI()->setTheme(I3T::getThemes()[n]);
				currentThemeIdx = (int) n;
			}

			// Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
			if (isSelected)
				ImGui::SetItemDefaultFocus();
		}
		ImGui::EndCombo();
	}
	ImGui::SameLine();


	if (ImGui::Button("Set as default"))
	{
			I3T::getUI()->setDefaultTheme(I3T::getThemes()[currentThemeIdx]);
	}
	ImGui::SameLine();

	// Reload themes from Data/themes
	if (GUI::Button("Reload"))
	{
		I3T::getUI()->reloadThemes();
	}


	// Save current theme to file.
	if (ImGui::Button("Save"))
	{
		auto path = std::string("Data/themes/") + curr.getName();
		saveTheme(Config::getAbsolutePath((const char*) path.c_str()) + ".yml", curr);
	}
	ImGui::SameLine();

	// Revert all changes.
	if (ImGui::Button("Revert"))
	{
		auto path = std::string("Data/themes/") + curr.getName() + ".yml";
		if (auto theme = loadTheme(Config::getAbsolutePath((const char*) path.c_str())))
		{
			I3T::getUI()->setTheme(*theme);
			auto& allThemes = I3T::getThemes();
			auto it = std::find_if(allThemes.begin(), allThemes.end(), [](Theme& theme) {
						return theme.getName() == I3T::getTheme().getName();
			});
			*it = *theme;
		}
	}


	showColors(curr);
	ImGui::Separator();

	showDimensions(curr);

	ImGui::End();
}

void UI::showColors(Theme& curr)
{
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

void UI::showDimensions(Theme& curr)
{
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
