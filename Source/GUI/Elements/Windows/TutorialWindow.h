/**
 * \file
 * \brief
 * \authors Miroslav Müller, Adam Loucký
 * \copyright Copyright (C) 2016-2023 I3T team, Department of Computer Graphics
 * and Interaction, FEE, Czech Technical University in Prague, Czech Republic
 *
 * This file is part of I3T - An Interactive Tool for Teaching Transformations
 * http://www.i3t-tool.org
 *
 * GNU General Public License v3.0 (see LICENSE.txt or https://www.gnu.org/licenses/gpl-3.0.txt)
 */
#pragma once

#include "imgui.h"
// temporary
#include "imgui_markdown.h" // tmeporary

#include <string>

#include "GUI/Elements/IWindow.h"

#include "Tutorial/Tutorial.h"
#include "Tutorial/TutorialRenderer.h"

class TutorialWindow : public IWindow, private ITutorialRenderer
{
public:
	I3T_WINDOW(TutorialWindow)

	explicit TutorialWindow(bool show);

	bool hasTutorial() const;

	void emptyTutorial();

	/**
	 * @brief Sets the passed tutorial as the current one
	 * @param tutorial Ptr to an already loaded tutorial
	 */
	void setTutorial(std::shared_ptr<Tutorial> tutorial);

	/**
	 * \brief Calls load and parse for a tutorial from file and sets it as the
	 * current one to be shown. \param header Ptr to an already loaded tutorial
	 * header object describing a specific tutorial.
	 */
	void setTutorial(std::shared_ptr<TutorialHeader>& header);

	void reloadTutorial();

	Ptr<Tutorial> getTutorial();

	/**
	 * \brief Renders the tutorial window using ImGui.
	 */
	void render() override;

private:
	float m_progressBarAnimationPosition;

	ImGui::MarkdownConfig m_mdConfig; // todo temporary

	void renderTutorialHeader();
	void renderTutorialContent();
	void renderTutorialControls();
	void renderExplanation(Explanation* explanation) override;
	void renderHeadline(Headline* headline) override;
	void renderChoiceTask(ChoiceTask* choice) override;
	void renderMultiChoiceTask(MultiChoiceTask* multiChoice) override;
	void renderInputTask(InputTask* input) override;
	void renderTask(Task* task) override;
	void renderHint(Hint* hint) override;

	// todo temporary rendering of images
	static inline ImGui::MarkdownImageData ImageCallback(ImGui::MarkdownLinkCallbackData data_);
};
