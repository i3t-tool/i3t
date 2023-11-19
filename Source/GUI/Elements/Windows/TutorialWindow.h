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

#include "imgui.h"          // temporary
#include "imgui_markdown.h" // tmeporary

#include <string>

#include "GUI/Elements/IWindow.h"

#include "Tutorial/Tutorial.h"
#include "Tutorial/TutorialRenderer.h"

class TutorialWindow : public IWindow, private ITutorialRenderer
{
public:
	I3T_WINDOW(TutorialWindow)
	TutorialWindow(bool show);

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

	/**
	 * \brief Calls load and parse for a tutorial from file and sets it as the
	 * current one to be shown. \param path Path to the .tut file to be loaded.
	 * All dependent files (such as pictures) should lie in the same folder.
	 */
	void setTutorial(std::string path);

	/**
	 * \brief Sets which step of currently set tutorial should be shown.
	 * \param step_number A step number counted from 0.
	 * \return true if successful, false if number out of range or no currently
	 * set tutorial
	 */
	void setStep(int step_number);

	/**
	 * \brief Renders the tutorial window using ImGui.
	 */
	void render() override;

private:
	static std::shared_ptr<Tutorial> m_tutorial; // todo temporarily static because of img callback
	// static std::string m_current_dir;
	int m_currentStep;
	float m_progressBarAnimationPosition;
	ImGui::MarkdownConfig m_mdConfig; // todo temporary
	// std::string getAbsolutePathByTutorial(std::string filename);
	void renderTutorialHeader();
	void renderTutorialContent();
	void renderTutorialControls();
	// overrides todo jsou private ale parent classa je ma public, ocividne je to
	// dulezitejsi
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
