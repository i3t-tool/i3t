/**
 * \file	GUI/Elements/tutorialWindow.h
 *
 * Tutorial window.
 */
#pragma once

#include <string>

#include "GUI/Elements/IWindow.h"

#include "Tutorial/Tutorial.h"

class TutorialWindow : public IWindow
{
public:
  TutorialWindow(bool show);
  
  /**
   * \brief Calls load and parse for a tutorial from file and sets it as the current one to be shown.
   * \param filename Name of the .tut file to be loaded. All dependent files (such as pictures) should lie in the same folder.
   */
  void setTutorial(std::string filename);

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
  void renderTutorialHeader();
  void renderTutorialContent();
  void renderTutorialControls();
};
