/**
 * \file	GUI/Elements/tutorialWindow.h
 *
 * Tutorial window.
 */
#pragma once
#include <imgui.h> // temporary
#include "imgui_markdown.h" // tmeporary

#include <string>

#include "GUI/Elements/IWindow.h"

#include "Tutorial/Tutorial.h"
#include "Tutorial/TutorialRenderer.h"

class TutorialWindow : public IWindow, private ITutorialRenderer
{
public:
  TutorialWindow(bool show);
  
  /**
   * \brief Calls load and parse for a tutorial from file and sets it as the current one to be shown.
   * \param filename Name of the .tut file to be loaded. All dependent files (such as pictures) should lie in the same folder.
   */
  void setTutorial(TutorialHeader header);

  /**
   * \brief Sets which step of currently set tutorial should be shown.
   * \param step_number A step number counted from 0.
   * \return true if successful, false if number out of range or no currently set tutorial
   */
  bool setStep(int step_number);

  /**
   * \brief Renders the tutorial window using ImGui.
   */
  void render() override;

private:
  std::unique_ptr<Tutorial> m_tutorial;
  int m_current_step;
  ImGui::MarkdownConfig m_mdConfig; // todo temporary
  // std::string getAbsolutePathByTutorial(std::string filename);
  void renderTutorialHeader();
  void renderTutorialContent();
  void renderTutorialControls();
  // overrides todo jsou private ale parent classa je ma public, ocividne je to dulezitejsi
  void renderTextWidget(TWText* tw_text) override;
  void renderImageWidget(TWImage* tw_image) override;
  void renderAnimatedImageWidget(TWAnimatedImage* tw_animated_image) override;
  void renderTaskWidget(TWTask* tw_task) override;
  void renderHintWidget(TWHint* tw_hint) override;
  void renderSpacingWidget(TWSpacing* tw_hint) override;
};
