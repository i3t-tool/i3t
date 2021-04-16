/**
 * \file	GUI/Elements/introWindow.h
 *
 * Intro window.
 */
#pragma once

#include "TutorialWindow.h"

#include <string>

#include "GUI/Elements/IWindow.h"

#include "Tutorial/Tutorial.h"

class IntroWindow : public IWindow
{
public:
  IntroWindow(bool show, TutorialWindow* tutorial_window);

  /**
   * \brief get all folders
   */
  void reloadTutorials();

  /**
   * \brief Renders the tutorial window using ImGui.
   */
  void render() override;

private:
  //std::vector<std::unique_ptr<Tutorial>> m_tutorials;
  std::vector<std::shared_ptr<TutorialHeader>> m_tutorial_headers;
  TutorialWindow* m_tutorial_window;
  void renderTutorials();
};
