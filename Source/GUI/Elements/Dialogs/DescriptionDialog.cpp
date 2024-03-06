/**
 * \file
 * \brief
 * \author Martin Herich
 * \copyright Copyright (C) 2016-2023 I3T team, Department of Computer Graphics
 * and Interaction, FEE, Czech Technical University in Prague, Czech Republic
 *
 * This file is part of I3T - An Interactive Tool for Teaching Transformations
 * http://www.i3t-tool.org
 *
 * GNU General Public License v3.0 (see LICENSE.txt or https://www.gnu.org/licenses/gpl-3.0.txt)
 */
#include "GUI/Elements/Dialogs/DescriptionDialog.h"

#define IMGUI_DEFINE_MATH_OPERATORS
#include "imgui.h"

#include "Commands/ApplicationCommands.h"

DescriptionDialog::DescriptionDialog()
{
	m_isEditState = false;
}

void DescriptionDialog::render()
{
	ImGui::Begin(setName("Description").c_str());

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
