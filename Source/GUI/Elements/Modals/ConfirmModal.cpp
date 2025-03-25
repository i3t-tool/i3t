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
#include "ConfirmModal.h"

#include "imgui.h"

#include "Commands/ApplicationCommands.h"
#include "Core/Application.h"
#include "Core/Input/InputManager.h"
#include "GUI/UIModule.h"
#include "State/StateManager.h"


ConfirmModal::ConfirmModal(std::unique_ptr<ConfirmModalStrategy> strategy)
    : ModalWindow(strategy->m_title), strategy(std::move(strategy))
{
	this->strategy->m_modal = this;
	ConfirmModal::s_isOpen = true;
}

ConfirmModal::~ConfirmModal()
{
	ConfirmModal::s_isOpen = false;
}

void ConfirmModal::onImGui()
{
	strategy->beforeOpen();

	ImGui::TextUnformatted(strategy->m_message.c_str());
	ImGui::TextUnformatted("\n\n");
	ImGui::Separator();

	if (ImGui::Button(strategy->m_yesText.c_str(), ImVec2(4 * ImGui::GetFontSize(), 0)))
	{
		strategy->onYes();
	}
	ImGui::SetItemDefaultFocus();

	ImGui::SameLine();
	if (ImGui::Button(strategy->m_noText.c_str(), ImVec2(4 * ImGui::GetFontSize(), 0)))
	{
		strategy->onNo();
	}

	ImGui::SameLine();
	if (ImGui::Button(_t("Cancel"), ImVec2(6.25f * ImGui::GetFontSize(), 0)))
	{
		hide();
	}
}

class BeforeExitSceneStrategy : public ConfirmModalStrategy
{
	using Action = std::function<void()>;

public:
	BeforeExitSceneStrategy(Action action)
	    : ConfirmModalStrategy(BEFORE_EXIT_MODAL_TITLE, BEFORE_EXIT_MODAL_MESSAGE), m_action(action)
	{}

private:
	void onYes() override
	{
		InputManager::triggerAction("save", EKeyState::Pressed);

		if (App::getModule<StateManager>().isDirty())
		{
			LOG_INFO("Save canceled, aborting exit scene");
			m_modal->hide();
		}
		else
		{
			// save was successful
			m_action();
			m_modal->hide();
		}
	}

	void onNo() override
	{
		m_action();
		m_modal->hide();
	}

	void beforeOpen() override
	{
		if (!App::getModule<StateManager>().isDirty())
		{
			onNo(); // no need to save
			m_modal->hide();
		}
	}

	Action m_action;
};

void askBeforeExitScene(std::function<void()> action)
{
	if (!ConfirmModal::s_isOpen)
	{
		auto strategy = std::make_unique<BeforeExitSceneStrategy>(action);
		UPtr<IWindow> modal = std::make_unique<ConfirmModal>(std::move(strategy));

		App::get().getModule<UIModule>().getWindowManager().openModal(std::move(modal));
	}
}
