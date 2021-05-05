#include "StyleEditor.h"

#include <array>

#include "imgui.h"

#include "GUI/Shortcuts.h"

constexpr const char* colorKeyToString(EColor color)
{
	constexpr std::array colors = {
		"Text",
		"Border",
		"Window Background",
		"Popup Background",
		"Menu Bar Background",
		"Primary Color (tabs, tiles, ...)",
		"Active Color",
		"Tab Color",

			"Pulse Link",
			"Float Link",
			"Matrix Link",
			"Quaternion Link",
			"Vector 3 Link",
			"Vector 4 Link",
			"Screen Link",
	};

	auto index = static_cast<size_t>(color);

  return index < colors.size() ? colors[index] : "";
}

constexpr const char* sizeKeyToString(size_t index)
{
  constexpr std::array sizes = {
      "Frame Padding",

      "Node Rounding",
      "Float Width",
      "Float Margin",
      "Items X Spacing",
      "Items Y Spacing",
      "Active Color",
      "Tab Color",
  };

  return index < sizes.size() ? sizes[index] : "";
}

void StyleEditor::render()
{
	ImGui::Begin(getID(), getShowPtr());

	auto& curr = I3T::getTheme();

	GUI::Text("Colors", EFont::TitleSmall);
  ImGui::Separator();

	ImGui::Text("Globals");
	for (auto& [key, color] : curr.getColorsRef())
  {
		auto* label = colorKeyToString(key);

		if (key == EColor::PulseLink)
    {
			ImGui::Text("Node editor");
		}

		if (strlen(label))
		  if (ImGui::ColorEdit4(label, (float*)(&color)))
      {
        curr.apply();
			}
	}
	ImGui::Separator();

	GUI::Text("Dimensions", EFont::TitleSmall);
	ImGui::Separator();

	ImGui::Text("Global properties");

	auto& sizes = curr.getSizesRef();
	for (size_t i = 0L; i < sizes.size(); ++i)
  {
		if (i == (size_t) ESize::Nodes_Rounding)
    {
      ImGui::Text("Node Editor");
		}

		auto* label = sizeKeyToString(i);
		if (strlen(label))
			if (ImGui::DragFloat(label, &sizes[i]))
      {
				curr.apply();
			}
	}

	ImGui::End();
}
