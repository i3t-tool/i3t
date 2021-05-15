#include "StyleEditor.h"

#include "imgui.h"

#include "Config.h"
#include "GUI/Shortcuts.h"
#include "Loader/ThemeLoader.h"

void StyleEditor::render()
{
	ImGui::Begin(getID(), getShowPtr());

	auto& curr = I3T::getTheme();


	static int currentThemeIdx = 0;
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

	if (GUI::Button("Reload"))
	{
		I3T::getUI()->reloadThemes();
	}

	if (ImGui::Button("Save"))
	{
		auto path = std::string("Data/themes/") + curr.getName();
		saveTheme(Config::getAbsolutePath((const char*) path.c_str()) + ".yml", curr);
	}
	ImGui::SameLine();

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


	GUI::Text("Colors", EFont::TitleSmall);
  ImGui::Separator();

	for (auto& [key, color] : curr.getColorsRef())
  {
		if (auto str = enumToStr(Theme::getColorNames(), key))
		{
			if (ImGui::ColorEdit4(*str, (float*)(&color)))
			{
				curr.apply();
			}
		}
	}
	ImGui::Separator();

	GUI::Text("Dimensions", EFont::TitleSmall);
	ImGui::Separator();

	for (auto& [key, val] : curr.getSizesRef())
	{
		if (auto str = enumToStr(Theme::getSizeNames(), key))
		{
			if (ImGui::DragFloat(*str, &val))
			{
				curr.apply();
			}
		}
	}

	for (auto& [key, val] : curr.getSizesVecRef())
	{
		if (auto str = enumToStr(Theme::getSizeVecNames(), key))
		{
			if (ImGui::DragFloat2(*str, &val[0]))
			{
				curr.apply();
			}
		}
	}

	ImGui::End();
}
