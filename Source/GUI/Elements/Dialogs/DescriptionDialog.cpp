#include "GUI/Elements/Dialogs/DescriptionDialog.h"

#include <imgui.h>

#include "Commands/ApplicationCommands.h"
#include "Core/Application.h"

DescriptionDialog::DescriptionDialog()
{
	m_isEditState = false;
}

void DescriptionDialog::render()
{
	ImGui::Begin(getName("Description").c_str());

	if (m_isEditState)
	{
		ImGui::InputTextMultiline("Description##Form", m_descriptionText, IM_ARRAYSIZE(m_descriptionText),
		                          ImVec2(-FLT_MIN, ImGui::GetTextLineHeight() * 16), 0);
	}
	else
	{
		// strcpy_s(m_descriptionText, TabSpace::descritionForm->getText().c_str());
		ImGui::TextWrapped(m_descriptionText);
	}

	ImGui::Separator();

	if (ImGui::Button("OK"))
	{
		if (m_isEditState)
		{
			m_isEditState = false;
			// TabSpace::descritionForm->setText(std::string(m_descriptionText));
		}
		else
		{
			HideWindowCommand::dispatch(ID);
		}
	}

	// Do not show edit button in edit state.
	if (!m_isEditState)
	{
		ImGui::SameLine();
		if (ImGui::Button("Edit"))
		{
			m_isEditState = true;
		}
	}

	ImGui::End();
}
