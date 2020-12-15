/**
 * \file	GUI/Elements/tutorialWindow.h
 *
 * Tutorial window.
 */
#pragma once

#include <string>

#include "GUI/Elements/IWindow.h"

class Tutorial;

class TutorialWindow : public IWindow
{
public:
  TutorialWindow(bool show);
  void setTutorial(std::string filename);
  void render() override;

private:
  Tutorial* m_tutorial;
  int m_current_step;
  void renderTutorialContent();
  void renderTutorialControls();
};
